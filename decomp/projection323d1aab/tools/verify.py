#!/usr/bin/env python3
"""Exact-input and independent instruction-grammar checks; never launch the PE."""
from __future__ import annotations
import argparse,csv,hashlib,json,pathlib,struct,tempfile
ROOT=pathlib.Path(__file__).resolve().parents[1]
TARGET='323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815'
RECORDS='617105394246dbb47aadc7f3eacc3922ddec80dd8f6c8a5e10d240f26b8c5a31'
class Reader:
    def __init__(self,code):self.code=code;self.p=0;self.branches=[]
    def take(self,n):
        if self.p+n>len(self.code):raise ValueError('Truncated instruction')
        data=self.code[self.p:self.p+n];self.p+=n;return data
    def exact(self,data):
        if self.take(len(data))!=data:raise ValueError('Opcode/register/stack mismatch')
    def memory(self,opcode,reg,base,sib=False):
        self.exact(bytes(opcode));m=self.take(1)[0];mode=m>>6
        if (m>>3)&7!=reg or m&7!=(4 if sib else base) or mode==3:raise ValueError('Unexpected operand')
        if sib:self.exact(bytes([0x80|base]))
        d=0 if mode==0 else int.from_bytes(self.take(1 if mode==1 else 4),'little',signed=True)
        if not 0<=d<0x1000000:raise ValueError('Unsupported displacement')
        return d
    def branch(self,opcode):
        self.exact(bytes([opcode]));delta=int.from_bytes(self.take(1),'little',signed=True)
        self.branches.append(self.p+delta)
    def immediate(self):return int.from_bytes(self.take(4),'little')
    def done(self,target):
        if self.p!=len(self.code) or any(b!=target for b in self.branches):raise ValueError('Changed control flow')

def decode(variant,code):
    """Read actual operand bytes and branch destinations, independently of the encoder."""
    r=Reader(code)
    if variant==18:
        a=r.memory([0x8b],0,1);r.exact(b'\x85\xc0');r.branch(0x74)
        op=r.take(1)[0];r.exact(b'\xc0')
        if op not in (0x83,0x81):raise ValueError('Expected add immediate')
        b=int.from_bytes(r.take(1 if op==0x83 else 4),'little',signed=op==0x83)&0xffffffff
        r.exact(b'\xc3');target=r.p;r.exact(b'\xb8');d=r.immediate();r.exact(b'\xc3');params=[a,b,d]
    elif variant in (19,40,64):
        a=r.memory([0x8b],0,1);r.exact(b'\x85\xc0');r.branch(0x75 if variant==40 else 0x74)
        if variant==40:r.exact(b'\xc3');target=r.p
        b=r.memory([0x0f,0xb7] if variant==64 else [0x8b],0,0);r.exact(b'\xc3')
        if variant!=40:target=r.p;r.exact(b'\x33\xc0\xc3')
        params=[a,b,2 if variant==64 else 4]
    elif variant in (20,21,38):
        t=r.memory([0x8b],2,1);r.exact(b'\x85\xd2');r.branch(0x74)
        i=r.memory([0x8b],0,1);r.exact(b'\x85\xc0');r.branch(0x7c)
        c=r.memory([0x3b],0,2);r.branch(0x7d);e=r.memory([0x8b],0,2,True)
        r.exact(b'\x85\xc0');r.branch(0x74)
        f=r.memory([0x8b] if variant==38 else [0x0f,0xb6],0,0);mask=255
        if variant==38:
            r.exact(b'\x83\xe0');mask=int.from_bytes(r.take(1),'little',signed=True)&0xffffffff
        r.exact(b'\xc3');target=r.p
        if variant==21:r.exact(b'\x33\xc0');fallback=0
        else:r.exact(b'\xb8');fallback=r.immediate()
        r.exact(b'\xc3');params=[t,i,c,e,f,4 if variant==38 else 1,mask,fallback]
    elif variant==22:
        r.exact(b'\x83\xc8\xff');a=r.memory([0x39],0,1);r.branch(0x74)
        b=r.memory([0x8b],0,1);target=r.p;r.exact(b'\xc3');params=[a,b]
    elif variant==23:
        r.exact(b'\x8b\x44\x24\x08');c=r.memory([0x3b],0,1);r.branch(0x73)
        p=r.memory([0x8b],1,1);r.exact(b'\x8b\x54\x24\x04\x89\x14\x81');target=r.p
        r.exact(b'\xc2\x08\x00');params=[c,p]
    elif variant==28:
        r.exact(b'\x8b\x44\x24\x04');c=r.memory([0x39],0,1);r.branch(0x77)
        r.exact(b'\x33\xc0\xc2\x04\x00');target=r.p;p=r.memory([0x8b],1,1)
        r.exact(b'\x8b\x04\x81\xc2\x04\x00');params=[c,p]
    elif variant in (39,60):
        a=r.memory([0x8b],0,1);r.exact(b'\x85\xc0');r.branch(0x74)
        r.exact(bytes([0x8a if variant==39 else 0x8b,0x4c,0x24,4]))
        b=r.memory([0x88 if variant==39 else 0x89],1,0);target=r.p;r.exact(b'\xc2\x04\x00')
        params=[a,b,1 if variant==39 else 4]
    else:raise ValueError('Unsupported variant')
    r.done(target);return params

def encode(variant,p):
    def mem(op,reg,base,d,sib=False):
        mode=0 if d==0 else 1 if d<128 else 2
        return bytes(op)+bytes([(mode<<6)|(reg<<3)|(4 if sib else base)])+(bytes([0x80|base]) if sib else b'')+(b'' if mode==0 else d.to_bytes(1 if mode==1 else 4,'little'))
    out=bytearray();jumps=[]
    def add(b):out.extend(b)
    def branch(op):jumps.append(len(out));add(bytes([op,0]))
    if variant==18:
        a,b,d=p;add(mem([0x8b],0,1,a)+b'\x85\xc0');branch(0x74)
        add((b'\x83\xc0'+bytes([b])) if b<128 else b'\x81\xc0'+struct.pack('<I',b))
        add(b'\xc3');target=len(out);add(b'\xb8'+struct.pack('<I',d)+b'\xc3')
    elif variant in (19,40,64):
        a,b,w=p;add(mem([0x8b],0,1,a)+b'\x85\xc0');branch(0x75 if variant==40 else 0x74)
        if variant==40:add(b'\xc3');target=len(out)
        add(mem([0x0f,0xb7] if w==2 else [0x8b],0,0,b)+b'\xc3')
        if variant!=40:target=len(out);add(b'\x33\xc0\xc3')
    elif variant in (20,21,38):
        t,i,c,e,f,w,m,d=p;add(mem([0x8b],2,1,t)+b'\x85\xd2');branch(0x74)
        add(mem([0x8b],0,1,i)+b'\x85\xc0');branch(0x7c)
        add(mem([0x3b],0,2,c));branch(0x7d);add(mem([0x8b],0,2,e,True)+b'\x85\xc0');branch(0x74)
        add(mem([0x8b] if w==4 else [0x0f,0xb6],0,0,f))
        if variant==38:add(b'\x83\xe0'+bytes([m&255]))
        add(b'\xc3');target=len(out);add((b'\x33\xc0' if variant==21 else b'\xb8'+struct.pack('<I',d))+b'\xc3')
    elif variant==22:
        a,b=p;add(b'\x83\xc8\xff'+mem([0x39],0,1,a));branch(0x74)
        add(mem([0x8b],0,1,b));target=len(out);add(b'\xc3')
    elif variant==23:
        c,pointer=p;add(b'\x8b\x44\x24\x08'+mem([0x3b],0,1,c));branch(0x73)
        add(mem([0x8b],1,1,pointer)+b'\x8b\x54\x24\x04\x89\x14\x81');target=len(out);add(b'\xc2\x08\x00')
    elif variant==28:
        c,pointer=p;add(b'\x8b\x44\x24\x04'+mem([0x39],0,1,c));branch(0x77)
        add(b'\x33\xc0\xc2\x04\x00');target=len(out);add(mem([0x8b],1,1,pointer)+b'\x8b\x04\x81\xc2\x04\x00')
    elif variant in (39,60):
        a,b,w=p;add(mem([0x8b],0,1,a)+b'\x85\xc0');branch(0x74)
        add(bytes([0x8a if w==1 else 0x8b,0x4c,0x24,4])+mem([0x88 if w==1 else 0x89],1,0,b));target=len(out);add(b'\xc2\x04\x00')
    else:raise ValueError('Unsupported recipe')
    for j in jumps:
        delta=target-j-2
        if not -128<=delta<128:raise ValueError('Branch range')
        out[j+1]=delta&255
    return bytes(out)

def records():
    data=(ROOT/'records.csv').read_bytes()
    if hashlib.sha256(data).hexdigest()!=RECORDS:raise ValueError('Changed records')
    rows=list(csv.DictReader(data.decode().splitlines()));names=set();inc='// Exact-attachment semantic operands; each row defines a distinct C++ function.\n'
    kinds={18:'ADD',19:'LOAD',40:'LOAD',64:'LOAD',20:'NEST',21:'NEST',38:'NEST',22:'SENTINEL',23:'ARRAY_SET',28:'ARRAY_GET',39:'STORE',60:'STORE'}
    for r in rows:
        r['va']=int(r['symbol'][4:],16);r['variant']=int(r['variant']);r['p']=list(map(int,r['parameters'].split(';')));r['size']=int(r['size'])
        if r['symbol'] in names or r['va']%16 or r['va']<0x410000 or kinds[r['variant']]!=r['kind']:raise ValueError('Invalid entry')
        names.add(r['symbol']);code=encode(r['variant'],r['p'])
        if len(code)!=r['size'] or hashlib.sha256(code).hexdigest()!=r['sha256'] or decode(r['variant'],code)!=r['p']:raise ValueError('Recipe/semantic mismatch: '+r['symbol'])
        inc+='RE5_'+r['kind']+'('+r['symbol']+', '+', '.join(f'0x{x:08X}U' for x in r['p'])+')\n'
    if len(rows)!=196 or (ROOT/'include/records.inc').read_text()!=inc:raise ValueError('Definition set mismatch')
    repo=ROOT.parent.parent
    for mod in ['leaf','scalar','accessor','guarded']:
        folder=repo/'decomp'/f'{mod}323d1aab';p=folder/'generated/catalog.json'
        if p.exists():other={f"FUN_{x['va']:08X}" for x in json.loads(p.read_text())['records']}
        elif (folder/'records.csv').exists():other={x['symbol'] for x in csv.DictReader((folder/'records.csv').read_text().splitlines())}
        else:continue
        if names&other:raise ValueError('Overlapping batch: '+mod)
    for table in ['functions.csv','claims.csv']:
        p=repo/'database'/table
        if p.exists():
            import re
            if names&set(re.findall(r'FUN_[0-9A-F]{8}',p.read_text())):raise ValueError('Existing canonical/claim entry')
    return rows

def verify_input(path,rows):
    data=pathlib.Path(path).read_bytes()
    if len(data)!=19977216 or hashlib.sha256(data).hexdigest()!=TARGET:raise ValueError('Wrong input size/SHA-256')
    pe=struct.unpack_from('<I',data,0x3c)[0];opt=pe+24
    if data[:2]!=b'MZ' or data[pe:pe+4]!=b'PE\0\0' or struct.unpack_from('<H',data,pe+4)[0]!=0x14c or struct.unpack_from('<H',data,opt)[0]!=0x10b or struct.unpack_from('<I',data,opt+28)[0]!=0x400000:raise ValueError('PE32 contract')
    sec=opt+struct.unpack_from('<H',data,pe+20)[0]
    text=None
    for n in range(struct.unpack_from('<H',data,pe+6)[0]):
        pos=sec+40*n
        if data[pos:pos+8].rstrip(b'\0')==b'.text':text=struct.unpack_from('<IIII',data,pos+8)
    if text!=(16185403,4096,16185856,1024):raise ValueError('Text mapping')
    for r in rows:
        o=r['va']-0x400c00;code=data[o:o+r['size']]
        if hashlib.sha256(code).hexdigest()!=r['sha256'] or code!=encode(r['variant'],r['p']) or decode(r['variant'],code)!=r['p']:raise ValueError('Original body mismatch')
        if data[o-2:o]!=b'\xcc\xcc' or data[o+r['size']]!=0xcc:raise ValueError('Boundary mismatch')
    calls=list(csv.DictReader((ROOT/'call-sites.csv').read_text().splitlines()))
    for r in calls:
        va=int(r['call_va'],0);target=int(r['target_va'],0);o=va-0x400c00
        if data[o]!=0xe8 or (va+5+struct.unpack_from('<i',data,o+1)[0])&0xffffffff!=target:raise ValueError('CALL mismatch')
    print(f'PASS exact local input: {len(rows)} bodies, {sum(r["size"] for r in rows)} bytes, {len(calls)} CALL sites')

def emit_reference(directory,rows):
    """Build-only, hash-checked instruction fixtures; no generated binary is committed."""
    directory=pathlib.Path(directory);directory.mkdir(parents=True,exist_ok=True)
    text='.text\n'
    for r in rows:
        code=encode(r['variant'],r['p']);name='original_'+r['symbol']
        text+=f'.balign 16\n.global {name}\n.type {name},@function\n{name}:\n.byte '+','.join(str(x) for x in code)+f'\n.size {name},.-{name}\n'
    text+='.section .note.GNU-stack,"",@progbits\n'
    (directory/'reference.S').write_text(text)

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--exe');ap.add_argument('--self-test',action='store_true');ap.add_argument('--emit-reference');a=ap.parse_args();rows=records()
    if a.self_test:
        for r in rows:
            changed=bytearray(encode(r['variant'],r['p']));changed[-1]^=255
            try:decode(r['variant'],changed)
            except ValueError:pass
            else:raise ValueError('Changed return accepted')
        with tempfile.NamedTemporaryFile() as f:
            f.write(b'wrong executable');f.flush()
            try:verify_input(f.name,rows)
            except ValueError:pass
            else:raise ValueError('Wrong input accepted')
    if a.exe:verify_input(a.exe,rows)
    if a.emit_reference:emit_reference(a.emit_reference,rows)
    print('PASS 196 recipes, hashes, independently decoded contracts and C++ records; no canonical tier promotion')
if __name__=='__main__':main()
