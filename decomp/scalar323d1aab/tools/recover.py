#!/usr/bin/env python3
"""Source-first recovery with decoded INT3 boundaries; does not infer original source types."""
from pathlib import Path
import sys,re,struct,json,hashlib,collections,subprocess
from verify_target import read_target,SHA as SHA256
import argparse
p=argparse.ArgumentParser(description="Discover complete whitelisted scalar routines from the exact local target; never emit unsupported stubs")
p.add_argument('--exe',type=Path,required=True);p.add_argument('--out',type=Path,required=True)
args=p.parse_args()
if args.out.exists():raise SystemExit('Refusing to overwrite an existing discovery report')
exe=args.exe
data,low,textsize,raw=read_target(exe)
prior=set()  # Whitelist byte patterns are disjoint from the published leaf patterns.
patterns=[]
def add(f,p,**kw):patterns.append((f,re.compile(p,re.S),kw))
def esc(x):return re.escape(x)
add('noop',esc(b'\xc3'),stack_pop=0)
add('noop',rb'\xc2(?P<pop>.{2})')
add('zero8',rb'(?P<variant>\x30|\x32)\xc0\xc3')
add('constant8',rb'\xb0(?P<imm>.)\xc3')
for enc,mr,d in [(0,1,b''),(8,0x41,rb'(?P<disp>.)'),(32,0x81,rb'(?P<disp>.{4})')]:
    add('load8',esc(bytes([0x8a,mr]))+d+esc(b'\xc3'),encoding=enc)
    add('store_imm8',esc(bytes([0xc6,mr]))+d+rb'(?P<imm>.)'+esc(b'\xc3'),encoding=enc)
    add('store_imm32',esc(bytes([0xc7,mr]))+d+rb'(?P<imm>.{4})'+esc(b'\xc3'),encoding=enc)
    for width,maskop,mask in [(8,b'\x24',rb'(?P<imm>.)'),(32,b'\x83\xe0',rb'(?P<imm>.)'),(32,b'\x25',rb'(?P<imm>.{4})')]:
        add('bitfield'+str(width),esc(bytes([0x8b,mr]))+d+rb'\xc1\xe8(?P<shift>.)'+esc(maskop)+mask+esc(b'\xc3'),encoding=enc,mask_form=maskop.hex())
    for width,load,store in [(32,b'\x8b\x44\x24\x04',b'\x89'),(8,b'\x8a\x44\x24\x04',b'\x88'),(8,b'\x0f\xb6\x44\x24\x04',b'\x88'),(16,b'\x66\x8b\x44\x24\x04',b'\x66\x89'),(16,b'\x0f\xb7\x44\x24\x04',b'\x66\x89')]:
        add('store_arg'+str(width),esc(load+store+bytes([mr]))+d+esc(b'\xc2\x04\x00'),encoding=enc,load_form=load.hex())
for v,a,b in [(0,b'\xa1',b'\xa3'),(1,b'\x8b\x05',b'\xa3'),(2,b'\xa1',b'\x89\x05'),(3,b'\x8b\x05',b'\x89\x05')]:
    add('global_copy32',esc(a)+rb'(?P<src>.{4})'+esc(b)+rb'(?P<dst>.{4})'+esc(b'\xc3'),variant=v)
add('global_store32',rb'\xc7\x05(?P<dst>.{4})(?P<imm>.{4})\xc3')
pe=struct.unpack_from('<I',data,0x3c)[0];imageend=0x400000+struct.unpack_from('<I',data,pe+24+56)[0]
found={};rejects=collections.Counter()
for va in range(max(low,0x410000),low+textsize,16):
    if va in prior or 0xBB9000<=va<0xBB9100:continue
    off=raw+va-low
    if data[off-2:off]!=b'\xcc\xcc':continue
    for family,p,kw in patterns:
        m=p.match(data[off:off+32])
        if not m or data[off+m.end():off+m.end()+2]!=b'\xcc\xcc':continue
        r=dict(family=family,**kw,va=va,rva=va-0x400000,file_offset=off,size=m.end(),body_sha256=hashlib.sha256(data[off:off+m.end()]).hexdigest())
        g=m.groupdict();r['displacement']=int.from_bytes(g['disp'],'little',signed=True) if g.get('disp') else 0
        if not 0<=r['displacement']<=0x1000000:rejects['displacement']+=1;continue
        for k in ('imm','src','dst'):
            if k in g:r[k]=int.from_bytes(g[k],'little')
        if family=='zero8':r['variant']=g['variant'][0]
        if g.get('pop'):
            r['stack_pop']=int.from_bytes(g['pop'],'little')
            if r['stack_pop']%4 or r['stack_pop']>64:rejects['unsupported_stack_pop']+=1;continue
        if g.get('shift'):r['shift']=int.from_bytes(g['shift'],'little')
        if family=='bitfield32' and r['mask_form']=='83e0':r['imm']=int.from_bytes(g['imm'],'little',signed=True)&0xffffffff
        if any(not 0x400000<=r[k]<=imageend-4 for k in ('src','dst') if k in r):rejects['global_address']+=1;continue
        found[va]=r;break
print('RAW',len(found),collections.Counter(r['family'] for r in found.values()),'reject',rejects,flush=True)
line_re=re.compile(r'^\s*([0-9a-f]+):\s+(.*)$');call_re=re.compile(r'^call\s+0x([0-9a-f]+)\s*$')
roots=set();calls=collections.defaultdict(list);prev=None;pad=0;count=0
proc=subprocess.Popen(['objdump','-d','--no-show-raw-insn','-Mintel',str(exe)],stdout=subprocess.PIPE,text=True)
for line in proc.stdout:
    m=line_re.match(line)
    if not m:continue
    va=int(m[1],16);asm=m[2].strip()
    if not low<=va<low+textsize:continue
    count+=1
    if va in found and pad>=2 and prev==va-1:roots.add(va)
    c=call_re.match(asm)
    if c:
        target=int(c[1],16);off=raw+va-low
        if target in found and data[off]==0xe8 and ((va+5+struct.unpack_from('<i',data,off+1)[0])&0xffffffff)==target:calls[target].append(va)
    pad=(pad+1 if prev==va-1 else 1) if asm=='int3' else 0;prev=va
assert proc.wait()==0
records=[]
for va in sorted(roots):
    r=found[va];r['direct_call_sites']=calls.get(va,[]);r['symbol']=f'FUN_{va:08X}';records.append(r)
catalog=dict(input_sha256=SHA256,record_count=len(records),instruction_bytes=sum(r['size'] for r in records),families=dict(collections.Counter(r['family'] for r in records)),scan=dict(gnu_rows=count,raw_hits=len(found),nondecoded_rejected=len(found)-len(records),rejected=dict(rejects)),records=records)
args.out.write_text(json.dumps(catalog,indent=2)+'\n')
print(json.dumps({k:v for k,v in catalog.items() if k!='records'},indent=2),flush=True)
