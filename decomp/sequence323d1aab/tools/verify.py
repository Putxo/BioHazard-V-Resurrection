#!/usr/bin/env python3
"""Verify exact local input and the closed set of audited loop instruction recipes."""
from __future__ import annotations
import argparse,csv,ctypes,hashlib,json,pathlib,re,struct,tempfile
ROOT=pathlib.Path(__file__).resolve().parents[1]
TARGET='323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815'
RECORDS='1a1a38167948b04b8cc60e15a43f41ee3e4f7e4286e214ce5e75de911f5d21e4'
class Recipe:
    def __init__(self):self.b=bytearray();self.labels={};self.jumps=[]
    def x(self,s):self.b.extend(bytes.fromhex(s))
    def u(self,n):self.b.extend(struct.pack('<I',n))
    def label(self,name):self.labels[name]=len(self.b)
    def jump(self,op,name):self.jumps.append((len(self.b),name));self.b.extend([op,0])
    def mem(self,op,reg,base,d):
        if not 0<=d<0x100000:raise ValueError('Unsupported displacement')
        mode=0 if d==0 else 1 if d<128 else 2
        self.b.extend([op,(mode<<6)|(reg<<3)|base])
        if mode:self.b.extend(d.to_bytes(1 if mode==1 else 4,'little'))
    def finish(self):
        for pos,name in self.jumps:
            delta=self.labels[name]-pos-2
            if not -128<=delta<128:raise ValueError('Branch out of range')
            self.b[pos+1]=delta&255
        return bytes(self.b)

def encode(kind,p):
    r=Recipe();x=r.x;m=r.mem;j=r.jump;l=r.label
    if kind=='COUNT':
        head,nxt=p;m(0x8b,1,1,head);x('33c085c9');j(0x74,'return');x('8da42400000000')
        l('loop');m(0x8b,1,1,nxt);x('83c00185c9');j(0x75,'loop');l('return');x('c3')
    elif kind=='AT':
        head,nxt=p;m(0x8b,0,1,head);x('33d285c0');j(0x74,'return');x('8b4c24048d4900')
        l('loop');x('3bd1');j(0x74,'return');m(0x8b,0,0,nxt);x('83c20185c0');j(0x75,'loop');l('return');x('c20400')
    elif kind=='TAIL':
        head,nxt=p;m(0x8b,0,1,head);x('85c0');j(0x74,'zero');l('loop');m(0x8b,1,0,nxt);x('85c9');j(0x74,'return')
        x('8bc185c0');j(0x75,'loop');l('zero');x('33c0');l('return');x('c3')
    elif kind=='SUM':
        nxt,field=p;m(0x8b,2,1,nxt);x('85d2');m(0x8b,0,1,field);j(0x74,'return');x('8d9b00000000')
        l('loop');m(0x03,0,2,field);m(0x8b,2,2,nxt);x('85d2');j(0x75,'loop');l('return');x('c3')
    elif kind=='ZERO_COUNT':
        x('33c03901');j(0x74,'return');l('loop');x('83c10483c001833900');j(0x75,'loop');l('return');x('c3')
    elif kind=='FIND_KEY':
        count,ptr=p;m(0x8b,2,1,count);x('5633c085d257');j(0x7e,'missing');m(0x8b,1,1,ptr);x('8b74240c')
        l('loop');x('8b393937');j(0x74,'return');x('83c00183c1043bc2');j(0x7c,'loop')
        l('missing');x('83c8ff');l('return');x('5f5ec20400')
    elif kind=='CLEAR':
        count,ptr=p;x('56');m(0x8b,6,1,count);x('33c085f657');j(0x7e,'missing');m(0x8b,2,1,ptr);x('8b7c240c')
        l('loop');x('393a');j(0x74,'found');x('83c00183c2043bc6');j(0x7c,'loop')
        l('missing');m(0x8b,2,1,ptr);x('83c8ff5fc70482000000005ec20400')
        l('found');m(0x8b,1,1,ptr);x('5fc70481000000005ec20400')
    elif kind=='PROPAGATE':
        flag,count,ptr,inner,field=p;x('568bf18a4c2408');m(0x38,1,6,flag);j(0x74,'return');x('57');m(0x8b,7,6,count)
        x('33d285ff');m(0x88,1,6,flag);j(0x76,'done');l('loop');m(0x8b,0,6,ptr);x('8b049085c0');j(0x74,'advance')
        m(0x8b,0,0,inner);x('85c0');j(0x74,'advance');m(0x88,1,0,field)
        l('advance');x('83c2013bd7');j(0x72,'loop');l('done');x('5f');l('return');x('5ec20400')
    elif kind=='FIXED_STORE':
        table,count,field=p;x('8a5424045681c1');r.u(table);x('be');r.u(count)
        l('loop');x('8b0185c0');j(0x74,'advance');m(0x88,2,0,field);l('advance');x('83c10483ee01');j(0x75,'loop');x('5ec20400')
    elif kind=='FIND_NODE':
        count,ptr,field=p;x('56');m(0x8b,6,1,count);x('33d285f657');j(0x76,'missing');m(0x8b,1,1,ptr);x('8b7c240c')
        l('loop');x('8b0185c0');j(0x74,'advance');m(0x39,7,0,field);j(0x74,'return');l('advance');x('83c20183c1043bd6');j(0x72,'loop')
        l('missing');x('33c0');l('return');x('5f5ec20400')
    elif kind=='PREVIOUS':
        count,ptr=p;m(0x8b,2,1,count);x('5633c085d257');j(0x76,'missing');m(0x8b,7,1,ptr);x('8b74240c8bcf')
        l('loop');x('3931');j(0x74,'found');x('83c00183c1043bc2');j(0x72,'loop');l('missing');x('5f33c05ec20400')
        l('found');x('85c0');j(0x76,'missing');x('8b4487fc5f5ec20400')
    else:raise ValueError('Unknown family')
    return r.finish()

def records():
    data=(ROOT/'records.csv').read_bytes()
    if hashlib.sha256(data).hexdigest()!=RECORDS:raise ValueError('Changed manifest')
    rows=list(csv.DictReader(data.decode().splitlines()));seen=set();inc='// Audited parameters: original addresses, not executable payloads.\n'
    for r in rows:
        r['va']=int(r['symbol'][4:],16);r['p']=[int(x) for x in r['parameters'].split(';') if x];r['size']=int(r['size'])
        if r['va'] in seen or r['va']%16 or r['va']<0x410000:raise ValueError('Invalid/duplicate address')
        seen.add(r['va']);code=encode(r['kind'],r['p'])
        if len(code)!=r['size'] or hashlib.sha256(code).hexdigest()!=r['sha256']:raise ValueError('Original-hash mismatch: '+r['symbol'])
        inc+=f"RE5_{r['kind']}({r['symbol']}"+''.join(f', 0x{x:08X}U' for x in r['p'])+')\n'
    if len(rows)!=42 or inc!=(ROOT/'include/records.inc').read_text():raise ValueError('Source definition mismatch')
    repo=ROOT.parent.parent
    for mod in ['leaf','scalar','accessor','guarded','projection']:
        folder=repo/'decomp'/f'{mod}323d1aab';p=folder/'generated/catalog.json'
        if p.exists():other={x['va'] for x in json.loads(p.read_text())['records']}
        elif (folder/'records.csv').exists():other={int(x['symbol'][4:],16) for x in csv.DictReader((folder/'records.csv').read_text().splitlines())}
        else:continue
        if seen&other:raise ValueError('Existing body: '+mod)
    for table in ['functions.csv','claims.csv']:
        p=repo/'database'/table
        if p.exists() and seen&{int(x,16) for x in re.findall(r'FUN_([0-9A-F]{8})',p.read_text())}:raise ValueError('Existing canonical/claim row')
    return rows

def verify_input(path,rows):
    data=pathlib.Path(path).read_bytes()
    if len(data)!=19977216 or hashlib.sha256(data).hexdigest()!=TARGET:raise ValueError('Wrong input size/SHA-256')
    pe=struct.unpack_from('<I',data,0x3c)[0];opt=pe+24
    if data[:2]!=b'MZ' or data[pe:pe+4]!=b'PE\0\0' or struct.unpack_from('<H',data,pe+4)[0]!=0x14c or struct.unpack_from('<H',data,opt)[0]!=0x10b or struct.unpack_from('<I',data,opt+28)[0]!=0x400000:raise ValueError('Not exact PE32 layout')
    sec=opt+struct.unpack_from('<H',data,pe+20)[0];text=None
    for i in range(struct.unpack_from('<H',data,pe+6)[0]):
        o=sec+40*i
        if data[o:o+8].rstrip(b'\0')==b'.text':text=struct.unpack_from('<IIII',data,o+8)
    if text!=(16185403,4096,16185856,1024):raise ValueError('Text mapping mismatch')
    for r in rows:
        o=r['va']-0x400c00;code=encode(r['kind'],r['p'])
        if data[o:o+len(code)]!=code or data[o-2:o]!=b'\xcc\xcc' or data[o+len(code)]!=0xcc:raise ValueError('Original body/boundary mismatch')
    calls=list(csv.DictReader((ROOT/'call-sites.csv').read_text().splitlines()))
    for r in calls:
        va=int(r['call_va'],0);to=int(r['target_va'],0);o=va-0x400c00
        if data[o]!=0xe8 or (va+5+struct.unpack_from('<i',data,o+1)[0])&0xffffffff!=to:raise ValueError('CALL mismatch')
    print(f'PASS exact local input: {len(rows)} loop bodies / {sum(r["size"] for r in rows)} bytes / {len(calls)} CALL sites')

def emit_reference(directory,rows):
    out=pathlib.Path(directory);out.mkdir(parents=True,exist_ok=True);text='.text\n'
    for r in rows:
        name='original_'+r['symbol'];code=encode(r['kind'],r['p'])
        text+=f'.balign 16\n.global {name}\n.type {name},@function\n{name}:\n.byte '+','.join(map(str,code))+f'\n.size {name},.-{name}\n'
    text+='.section .note.GNU-stack,"",@progbits\n';(out/'reference.S').write_text(text)

def llvm_decode(library,rows):
    lib=ctypes.CDLL(library)
    for n in ('TargetInfo','Target','TargetMC','AsmPrinter','Disassembler'):getattr(lib,'LLVMInitializeX86'+n)()
    lib.LLVMCreateDisasm.argtypes=[ctypes.c_char_p,ctypes.c_void_p,ctypes.c_int,ctypes.c_void_p,ctypes.c_void_p];lib.LLVMCreateDisasm.restype=ctypes.c_void_p
    ctx=lib.LLVMCreateDisasm(b'i386-pc-windows-msvc',None,0,None,None)
    if not ctx:raise ValueError('LLVM initialization failed')
    lib.LLVMDisasmInstruction.argtypes=[ctypes.c_void_p,ctypes.c_void_p,ctypes.c_uint64,ctypes.c_uint64,ctypes.c_char_p,ctypes.c_size_t];lib.LLVMDisasmInstruction.restype=ctypes.c_size_t
    lib.LLVMDisasmDispose.argtypes=[ctypes.c_void_p];total=0
    try:
        for r in rows:
            code=encode(r['kind'],r['p']);p=0
            while p<len(code):
                out=ctypes.create_string_buffer(256);buf=ctypes.create_string_buffer(code[p:]);n=lib.LLVMDisasmInstruction(ctx,buf,len(code)-p,r['va']+p,out,len(out))
                if not n:raise ValueError('Undecodable I386 instruction')
                p+=n;total+=1
    finally:lib.LLVMDisasmDispose(ctx)
    if total!=765:raise ValueError('Independent instruction count mismatch')
    print('PASS LLVM independent I386 decoding:',total,'instructions')

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--exe');ap.add_argument('--self-test',action='store_true');ap.add_argument('--llvm-library');a=ap.parse_args();rows=records()
    if a.self_test:
        with tempfile.NamedTemporaryFile() as f:
            f.write(b'wrong input');f.flush()
            try:verify_input(f.name,rows)
            except ValueError:pass
            else:raise ValueError('Wrong-input rejection failed')
        for r in rows:
            code=bytearray(encode(r['kind'],r['p']));code[-1]^=255
            if hashlib.sha256(code).hexdigest()==r['sha256']:raise ValueError('Changed return accepted')
    if a.exe:verify_input(a.exe,rows)
    if a.llvm_library:llvm_decode(a.llvm_library,rows)
    print('PASS 42 original-hash recipes and source definitions; no canonical promotion')
if __name__=='__main__':main()
