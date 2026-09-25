#!/usr/bin/env python3
"""Verify complete whitelisted control-flow bodies; never execute the input EXE."""
import argparse,csv,ctypes,hashlib,pathlib,struct,tempfile
ROOT=pathlib.Path(__file__).resolve().parents[1]
TARGET='323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815'
MANIFEST='dc02e3f7241b27db41238e82442b802437c6dd51fd76021bae6720c359f12b46'
class Reader:
    def __init__(self,b):self.b=b;self.p=0;self.branches=[]
    def take(self,n):
        if self.p+n>len(self.b):raise ValueError('Truncated instruction')
        x=self.b[self.p:self.p+n];self.p+=n;return x
    def exact(self,x):
        if self.take(len(x))!=x:raise ValueError('Unexpected opcode/register/stack contract')
    def memory(self,opcode,reg,base,sib=False):
        self.exact(bytes([opcode]));m=self.take(1)[0];mode=m>>6
        if ((m>>3)&7)!=reg or (m&7)!=(4 if sib else base) or mode==3:raise ValueError('Memory operand mismatch')
        if sib:self.exact(bytes([0x80|base])) # scale 4, index EAX
        d=0 if mode==0 else int.from_bytes(self.take(1 if mode==1 else 4),'little',signed=True)
        if not 0<=d<=0x100000:raise ValueError('Unsupported displacement')
        return d
    def branch(self,opcode):
        self.exact(bytes([opcode]));rel=int.from_bytes(self.take(1),'little',signed=True)
        self.branches.append(self.p+rel)
    def zero(self):
        if self.take(2) not in (b'\x33\xc0',b'\x31\xc0'):raise ValueError('Expected zero EAX')
    def bitop(self,reg):
        opcode=self.b[self.p]
        if opcode not in (0x81,0x83):raise ValueError('Expected immediate bit operation')
        d=self.memory(opcode,reg,1)
        v=int.from_bytes(self.take(1 if opcode==0x83 else 4),'little',signed=opcode==0x83)&0xffffffff
        return d,v

def decode(kind,code):
    r=Reader(code)
    if kind=='flag':
        r.exact(bytes.fromhex('807c240400'));r.branch(0x74);d,s=r.bitop(1)
        r.exact(bytes.fromhex('c20400'));target=r.p;d2,c=r.bitop(4)
        if d!=d2:raise ValueError('Branch destinations differ')
        r.exact(bytes.fromhex('c20400'));params=[d,s,c]
    elif kind=='array':
        r.exact(bytes.fromhex('8b442404'));c=r.memory(0x3b,0,1);r.branch(0x72)
        r.zero();r.exact(bytes.fromhex('c20400'));target=r.p;p=r.memory(0x8b,1,1)
        r.exact(bytes.fromhex('8b0481c20400'));params=[c,p]
    elif kind=='nested':
        t=r.memory(0x8b,2,1);r.exact(bytes.fromhex('85d2'));r.branch(0x74)
        i=r.memory(0x8b,0,1);r.exact(bytes.fromhex('85c0'));r.branch(0x7c)
        c=r.memory(0x3b,0,2);r.branch(0x7d);e=r.memory(0x8b,0,2,True)
        r.exact(bytes.fromhex('85c0'));r.branch(0x74);f=r.memory(0x8b,0,0)
        r.exact(b'\xc3');target=r.p;r.zero();r.exact(b'\xc3');params=[t,i,c,e,f]
    else:raise ValueError('Unknown family')
    if r.p!=len(code) or any(x!=target for x in r.branches):raise ValueError('Incomplete or changed control flow')
    return params

def encode(kind,p):
    """Canonical instruction recipe; expected original hashes are stored separately."""
    def mem(op,reg,base,d,sib=False):
        mode=0 if d==0 else 1 if d<128 else 2
        return bytes([op,(mode<<6)|(reg<<3)|(4 if sib else base)])+(bytes([0x80|base]) if sib else b'')+(b'' if mode==0 else d.to_bytes(1 if mode==1 else 4,'little'))
    def bitop(reg,d,v):
        short=v<128 or v>=0xffffff80
        return mem(0x83 if short else 0x81,reg,1,d)+(v& (255 if short else 0xffffffff)).to_bytes(1 if short else 4,'little')
    ret=bytes.fromhex('c20400')
    if kind=='flag':
        d,a,b=p;yes=bitop(1,d,a)+ret
        return bytes.fromhex('807c240400')+bytes([0x74,len(yes)])+yes+bitop(4,d,b)+ret
    if kind=='array':
        c,ptr=p
        return bytes.fromhex('8b442404')+mem(0x3b,0,1,c)+bytes.fromhex('720533c0c20400')+mem(0x8b,1,1,ptr)+bytes.fromhex('8b0481')+ret
    if kind=='nested':
        t,i,c,e,f=p
        parts=[mem(0x8b,2,1,t),bytes.fromhex('85d2'),0x74,mem(0x8b,0,1,i),bytes.fromhex('85c0'),0x7c,mem(0x3b,0,2,c),0x7d,mem(0x8b,0,2,e,True),bytes.fromhex('85c0'),0x74,mem(0x8b,0,0,f),b'\xc3']
        out=bytearray();branches=[]
        for part in parts:
            if isinstance(part,int):branches.append(len(out));out.extend([part,0])
            else:out.extend(part)
        for at in branches:out[at+1]=len(out)-(at+2)
        return bytes(out)+bytes.fromhex('33c0c3')
    raise ValueError('Unknown family')

def records():
    data=(ROOT/'records.csv').read_bytes()+(ROOT/'body-hashes.csv').read_bytes()
    if hashlib.sha256(data).hexdigest()!=MANIFEST:raise ValueError('Changed manifest')
    with (ROOT/'records.csv').open(newline='') as f:rows=list(csv.DictReader(f))
    with (ROOT/'body-hashes.csv').open(newline='') as f:hashes={r['symbol']:r['sha256'] for r in csv.DictReader(f)}
    if len(hashes)!=len(rows):raise ValueError('Missing or duplicate body hashes')
    inc='// Audited exact-target addresses and semantic parameters. No original bytes.\n';seen=set()
    for r in rows:
        va=int(r['symbol'][4:],16);code=encode(r['kind'],list(map(int,r['parameters'].split(';'))));params=decode(r['kind'],code)
        r['instruction_hex']=code.hex();r['sha256']=hashes[r['symbol']]
        if va in seen or va%16 or va<0x410000:raise ValueError('Duplicate or unsupported start')
        seen.add(va)
        if params!=list(map(int,r['parameters'].split(';'))) or len(code)!=int(r['size']):raise ValueError('Incorrect recipe')
        if hashlib.sha256(code).hexdigest()!=r['sha256']:raise ValueError('Body hash mismatch')
        inc+='RE5_'+r['kind'].upper()+'('+r['symbol']+', '+', '.join(f'0x{x:08X}U' for x in params)+')\n'
    if len(rows)!=127 or (ROOT/'include/records.inc').read_text()!=inc:raise ValueError('Source records mismatch')
    repo=ROOT.parent.parent
    for catalog in ['decomp/leaf323d1aab/generated/catalog.json','decomp/scalar323d1aab/generated/catalog.json']:
        p=repo/catalog
        if p.exists():
            import json
            if seen&{x['va'] for x in json.loads(p.read_text())['records']}:raise ValueError('Already recovered body')
    p=repo/'database/functions.csv'
    if p.exists():
        with p.open(newline='') as f:
            if seen&{int(x['va'],0) for x in csv.DictReader(f) if x.get('va')}:raise ValueError('Already registered function')
    return rows

def target(path):
    data=pathlib.Path(path).read_bytes()
    if len(data)!=19977216 or hashlib.sha256(data).hexdigest()!=TARGET:raise ValueError('Wrong input size/SHA-256')
    pe=struct.unpack_from('<I',data,0x3c)[0];opt=pe+24
    if data[:2]!=b'MZ' or data[pe:pe+4]!=b'PE\0\0' or struct.unpack_from('<H',data,pe+4)[0]!=0x14c or struct.unpack_from('<H',data,opt)[0]!=0x10b:raise ValueError('Not I386 PE32')
    if struct.unpack_from('<I',data,opt+28)[0]!=0x400000:raise ValueError('Image base mismatch')
    sec=opt+struct.unpack_from('<H',data,pe+20)[0]
    for n in range(struct.unpack_from('<H',data,pe+6)[0]):
        o=sec+40*n
        if data[o:o+8].rstrip(b'\0')==b'.text':
            size,rva,rawsize,raw=struct.unpack_from('<IIII',data,o+8)
            if rva!=0x1000 or raw!=0x400 or size!=16185403 or raw+rawsize>len(data):raise ValueError('Text mapping mismatch')
            return data
    raise ValueError('Missing text section')

def verify_input(path,rows):
    data=target(path);calls=0
    for r in rows:
        va=int(r['symbol'][4:],16);off=va-0x400c00;code=bytes.fromhex(r['instruction_hex'])
        if data[off:off+len(code)]!=code or data[off-2:off]!=b'\xcc\xcc' or data[off+len(code)]!=0xcc:raise ValueError('Body/boundary mismatch')
        for s in filter(None,r['direct_calls'].split(';')):
            caller=int(s,0);o=caller-0x400c00
            if data[o]!=0xe8 or (caller+5+struct.unpack_from('<i',data,o+1)[0])&0xffffffff!=va:raise ValueError('CALL reference mismatch')
            calls+=1
    print('PASS exact local EXE: 127 complete bodies / 4222 instruction bytes /',calls,'CALL references')

def llvm_decode(rows,library):
    lib=ctypes.CDLL(library)
    for n in ('TargetInfo','Target','TargetMC','AsmPrinter','Disassembler'):getattr(lib,'LLVMInitializeX86'+n)()
    lib.LLVMCreateDisasm.argtypes=[ctypes.c_char_p,ctypes.c_void_p,ctypes.c_int,ctypes.c_void_p,ctypes.c_void_p];lib.LLVMCreateDisasm.restype=ctypes.c_void_p
    ctx=lib.LLVMCreateDisasm(b'i386-pc-windows-msvc',None,0,None,None)
    if not ctx:raise ValueError('LLVM I386 initialization failed')
    lib.LLVMDisasmInstruction.argtypes=[ctypes.c_void_p,ctypes.c_void_p,ctypes.c_uint64,ctypes.c_uint64,ctypes.c_char_p,ctypes.c_size_t];lib.LLVMDisasmInstruction.restype=ctypes.c_size_t
    lib.LLVMDisasmDispose.argtypes=[ctypes.c_void_p]
    count=0
    try:
        for r in rows:
            code=bytes.fromhex(r['instruction_hex']);pos=0;n=0
            while pos<len(code):
                b=ctypes.create_string_buffer(code[pos:]);out=ctypes.create_string_buffer(256)
                size=lib.LLVMDisasmInstruction(ctx,b,len(code)-pos,int(r['symbol'][4:],16)+pos,out,len(out))
                if not size:raise ValueError('Independent I386 decoding failed')
                pos+=size;n+=1
            if n!={'flag':6,'array':8,'nested':15}[r['kind']]:raise ValueError('Independent instruction boundaries disagree')
            count+=n
    finally:lib.LLVMDisasmDispose(ctx)
    print('PASS independent LLVM I386 decoding:',count,'instructions')

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--exe');ap.add_argument('--llvm');ap.add_argument('--self-test',action='store_true');a=ap.parse_args()
    rows=records()
    if a.self_test:
        with tempfile.NamedTemporaryFile() as f:
            f.write(b'not the authorized executable');f.flush()
            try:target(f.name)
            except ValueError:pass
            else:raise ValueError('Wrong-input rejection failed')
        for r in rows:
            changed=bytearray.fromhex(r['instruction_hex']);changed[-1]^=0xff
            try:decode(r['kind'],changed)
            except (ValueError,IndexError):pass
            else:raise ValueError('Changed return accepted')
    if a.exe:verify_input(a.exe,rows)
    if a.llvm:llvm_decode(rows,a.llvm)
    print('PASS manifest / semantic parameters / source declarations; 127 unique bodies; canonical promotion NOT performed')
if __name__=='__main__':main()
