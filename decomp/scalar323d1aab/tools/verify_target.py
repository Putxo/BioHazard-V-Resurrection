#!/usr/bin/env python3
"""Check this user's exact local input and each instruction recipe without execution."""
from __future__ import annotations
import argparse,base64,hashlib,json,pathlib,struct,zlib
from generate import ROOT,load_records,body
SHA='323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815'
def read_target(path):
    data=pathlib.Path(path).read_bytes()
    if len(data)!=19977216 or hashlib.sha256(data).hexdigest()!=SHA:raise ValueError('Wrong input: expected authorized attached EXE size and SHA-256')
    pe=struct.unpack_from('<I',data,0x3c)[0];opt=pe+24
    if data[:2]!=b'MZ' or data[pe:pe+4]!=b'PE\0\0' or struct.unpack_from('<H',data,pe+4)[0]!=0x14c or struct.unpack_from('<H',data,opt)[0]!=0x10b:raise ValueError('Not I386 PE32')
    base=struct.unpack_from('<I',data,opt+28)[0];section=opt+struct.unpack_from('<H',data,pe+20)[0]
    for i in range(struct.unpack_from('<H',data,pe+6)[0]):
        p=section+40*i
        if data[p:p+8].rstrip(b'\0')==b'.text':
            size,rva,raw_size,raw=struct.unpack_from('<IIII',data,p+8)
            if base!=0x400000 or raw+raw_size>len(data) or size>raw_size:raise ValueError('Unexpected mapping')
            return data,base+rva,size,raw
    raise ValueError('No .text section')
def llvm_decode(records,data,library):
    import ctypes
    lib=ctypes.CDLL(library)
    for name in ['LLVMInitializeX86TargetInfo','LLVMInitializeX86Target','LLVMInitializeX86TargetMC','LLVMInitializeX86AsmPrinter','LLVMInitializeX86Disassembler']:getattr(lib,name)()
    lib.LLVMCreateDisasm.argtypes=[ctypes.c_char_p,ctypes.c_void_p,ctypes.c_int,ctypes.c_void_p,ctypes.c_void_p];lib.LLVMCreateDisasm.restype=ctypes.c_void_p
    context=lib.LLVMCreateDisasm(b'i386-pc-windows-msvc',None,0,None,None)
    if not context:raise ValueError('No I386 disassembler')
    lib.LLVMSetDisasmOptions.argtypes=[ctypes.c_void_p,ctypes.c_uint64];lib.LLVMSetDisasmOptions(context,6)
    lib.LLVMDisasmInstruction.argtypes=[ctypes.c_void_p,ctypes.c_void_p,ctypes.c_uint64,ctypes.c_uint64,ctypes.c_char_p,ctypes.c_size_t];lib.LLVMDisasmInstruction.restype=ctypes.c_size_t
    decoded=[]
    try:
        for r in records:
            code=data[r['file_offset']:r['file_offset']+r['size']];at=0;last=''
            while at<len(code):
                raw=ctypes.create_string_buffer(code[at:]);out=ctypes.create_string_buffer(256)
                size=lib.LLVMDisasmInstruction(context,raw,len(code)-at,r['va']+at,out,len(out))
                if not size:raise ValueError(f'Undecodable I386 instruction at {r["va"]+at:08X}')
                last=' '.join(out.value.decode('ascii').split())
                assert not last.startswith(('j','call','int'))
                decoded.append([r['va']+at,size,last]);at+=size
            assert at==len(code) and last.startswith('ret')
    finally:
        lib.LLVMDisasmDispose.argtypes=[ctypes.c_void_p];lib.LLVMDisasmDispose(context)
    return {'instructions':len(decoded),'trace_sha256':hashlib.sha256(json.dumps(decoded,separators=(',',':')).encode()).hexdigest()}
def verify(path,llvm_library=None):
    data,low,sz,raw=read_target(path);records=load_records()
    for r in records:
        off=raw+r['va']-low;code=body(r)
        assert low<=r['va']<r['va']+len(code)<=low+sz
        assert off==r['file_offset']
        assert data[off:off+len(code)]==code,r['symbol']
        assert data[off-2:off]==b'\xcc\xcc' and data[off+len(code):off+len(code)+2]==b'\xcc\xcc',r['symbol']
    call_rows=json.loads(zlib.decompress(base64.b85decode(''.join((ROOT/'metadata/calls.b85').read_text().split()))))
    n=0;vas={r['va'] for r in records}
    for target,sites in call_rows:
        assert target in vas
        for site in sites:
            off=raw+site-low;assert data[off]==0xe8 and ((site+5+struct.unpack_from('<i',data,off+1)[0])&0xffffffff)==target;n+=1
    result={'input_sha256':SHA,'input_size':len(data),'body_hashes_verified':len(records),'padding_verified':len(records),'direct_calls_verified':n,'original_pe_executed':False}
    if llvm_library:result['independent_llvm_i386']=llvm_decode(records,data,llvm_library)
    print(json.dumps(result,indent=2));return result
if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--exe',type=pathlib.Path,required=True);p.add_argument('--llvm-library');a=p.parse_args()
    try:verify(a.exe,a.llvm_library)
    except (OSError,ValueError,AssertionError) as e:raise SystemExit(str(e))
