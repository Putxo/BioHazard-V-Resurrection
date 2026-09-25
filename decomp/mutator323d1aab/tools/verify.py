#!/usr/bin/env python3
"""Check the recorded original-hash commitment and emit source-only definitions."""
import argparse,csv,ctypes,hashlib,json,pathlib,re,struct,tempfile
from model import KINDS,decode,encode
ROOT=pathlib.Path(__file__).resolve().parents[1]
TARGET='323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815'
MANIFEST='446fd5da6588942fe5ef40166b85ee392ce2c308ec7af22a2595e362e767a004'

def records():
    rows=list(csv.DictReader((ROOT/'records.csv').read_text().splitlines()));seen=set();receipt=''
    for r in rows:
        if not re.fullmatch(r'FUN_[0-9A-F]{8}',r['symbol']) or r['kind'] not in KINDS:raise ValueError('Invalid record')
        r['va']=int(r['symbol'][4:],16);r['p']=list(map(int,r['parameters'].split(';')))
        if r['va'] in seen or r['va']%16 or r['va']<0x410000:raise ValueError('Duplicate/unsupported start')
        seen.add(r['va']);code=encode(r['kind'],r['p'])
        if decode(r['kind'],code)!=r['p']:raise ValueError('Instruction contract mismatch')
        r['size']=len(code);r['sha256']=hashlib.sha256(code).hexdigest()
        receipt+=f"{r['symbol']},{r['size']},{r['sha256']}\n"
    if len(rows)!=422 or sorted(seen)!=[r['va'] for r in rows]:raise ValueError('Incorrect ordered set')
    if hashlib.sha256(receipt.encode()).hexdigest()!=MANIFEST or (ROOT/'original-body-manifest.sha256').read_text().strip()!=MANIFEST:raise ValueError('Recorded original hashes disagree')
    repo=ROOT.parent.parent
    for folder in (repo/'decomp').glob('*323d1aab'):
        if folder==ROOT:continue
        p=folder/'generated/catalog.json'
        if p.exists():other={x['va'] for x in json.loads(p.read_text())['records']}
        elif (folder/'records.csv').exists():other={int(x['symbol'][4:],16) for x in csv.DictReader((folder/'records.csv').read_text().splitlines())}
        else:continue
        if seen&other:raise ValueError('Already recovered address in '+folder.name)
    for name in ('functions.csv','claims.csv'):
        p=repo/'database'/name
        if p.exists() and seen&{int(x,16) for x in re.findall(r'FUN_([0-9A-Fa-f]{8})',p.read_text())}:raise ValueError('Existing canonical/claim address')
    return rows

def emit(out,rows):
    out=pathlib.Path(out);out.mkdir(parents=True,exist_ok=True)
    inc='// Generated from audited records.csv; one complete function per address.\n'
    for r in rows:inc+=f"RE5_{r['kind']}({r['symbol']}"+''.join(f', 0x{x:08X}U' for x in r['p'])+')\n'
    (out/'records.inc').write_text(inc)
    (out/'body-hashes.csv').write_text('symbol,size,sha256\n'+''.join(f"{r['symbol']},{r['size']},{r['sha256']}\n" for r in rows))

def reference(out,rows):
    out=pathlib.Path(out);emit(out,rows);text='.text\n'
    for r in rows:
        n='original_'+r['symbol'];code=encode(r['kind'],r['p'])
        text+=f'.balign 16\n.global {n}\n.type {n},@function\n{n}:\n.byte '+','.join(map(str,code))+f'\n.size {n},.-{n}\n'
    (out/'reference.S').write_text(text+'.section .note.GNU-stack,"",@progbits\n')

def verify_target(path,rows):
    data=pathlib.Path(path).read_bytes()
    if len(data)!=19977216 or hashlib.sha256(data).hexdigest()!=TARGET:raise ValueError('Wrong input size/SHA-256')
    pe=struct.unpack_from('<I',data,0x3c)[0];opt=pe+24
    if data[:2]!=b'MZ' or data[pe:pe+4]!=b'PE\0\0' or struct.unpack_from('<H',data,pe+4)[0]!=0x14c or struct.unpack_from('<H',data,opt)[0]!=0x10b or struct.unpack_from('<I',data,opt+28)[0]!=0x400000:raise ValueError('Not expected I386 PE32')
    sec=opt+struct.unpack_from('<H',data,pe+20)[0];text=None
    for i in range(struct.unpack_from('<H',data,pe+6)[0]):
        o=sec+40*i
        if data[o:o+8].rstrip(b'\0')==b'.text':text=struct.unpack_from('<IIII',data,o+8)
    if text!=(16185403,4096,16185856,1024):raise ValueError('Text mapping mismatch')
    receipt=''
    for r in rows:
        o=r['va']-0x400c00;code=data[o:o+r['size']]
        if decode(r['kind'],code)!=r['p'] or code!=encode(r['kind'],r['p']) or data[o-2:o]!=b'\xcc\xcc' or data[o+r['size']]!=0xcc:raise ValueError('Original body/boundary mismatch')
        receipt+=f"{r['symbol']},{len(code)},{hashlib.sha256(code).hexdigest()}\n"
    if hashlib.sha256(receipt.encode()).hexdigest()!=MANIFEST:raise ValueError('Original manifest mismatch')
    calls=list(csv.DictReader((ROOT/'call-sites.csv').read_text().splitlines()));starts={r['va'] for r in rows}
    for r in calls:
        va=int(r['call_va'],0);dst=int(r['target_va'],0);o=va-0x400c00
        if dst not in starts or data[o]!=0xe8 or va+5+struct.unpack_from('<i',data,o+1)[0]!=dst:raise ValueError('CALL mismatch')
    print('PASS exact local target:',len(rows),'bodies;',sum(r['size'] for r in rows),'bytes;',len(calls),'direct CALL sites')

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
                if not n:raise ValueError('Undecodable instruction')
                p+=n;total+=1
    finally:lib.LLVMDisasmDispose(ctx)
    print('PASS independent LLVM I386 decoding:',total,'instructions')

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--exe');ap.add_argument('--emit');ap.add_argument('--self-test',action='store_true');ap.add_argument('--llvm-library');a=ap.parse_args();rows=records()
    if a.self_test:
        for r in rows:
            changed=bytearray(encode(r['kind'],r['p']));changed[-1]^=255
            try:decode(r['kind'],changed)
            except ValueError:pass
            else:raise ValueError('Changed stack cleanup accepted')
        with tempfile.NamedTemporaryFile() as f:
            f.write(b'wrong input');f.flush()
            try:verify_target(f.name,rows)
            except ValueError:pass
            else:raise ValueError('Wrong executable accepted')
    if a.exe:verify_target(a.exe,rows)
    if a.emit:emit(a.emit,rows)
    if a.llvm_library:llvm_decode(a.llvm_library,rows)
    print('PASS original-hash commitment / instruction contracts / address set; canonical promotion NOT performed')
if __name__=='__main__':main()
