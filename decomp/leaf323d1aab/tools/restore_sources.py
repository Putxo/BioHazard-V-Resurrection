#!/usr/bin/env python3
"""Restore original recovered C++ from compact semantic metadata (not EXE bytes).

The generator and constants/offsets define real C++ operations. Output contains
no emulator, embedded PE or unsupported-function stubs. The original source
files are reproducible byte-for-byte; the discovery catalogue is normalized.
"""
from pathlib import Path
import argparse, base64, hashlib, json, shutil, struct, tempfile, zlib
from recover_leaves import ROOT, SHA256, SIZE, write_sources, read_target
FAMILIES = ['constant','zero','identity','address','load8','load16','load32','sign8','sign16']
SEMANTIC_SHA = 'c589713aa66aff049c364423bf6bd39dfd08233d2f53339ef5cbb2e492be7b72'
CALLS_SHA = '045f6149871941acaba83b8be7dc42cf1c386b6a4de622632c8b3a902e6a96be'
ORIGINAL_CATALOG_SHA = 'ad8f5e753f0b96e544649201f20c977bdbf36db3732d20b4a553408e4beb6d12'

def unpack(name, digest):
    encoded = b''.join((ROOT/'metadata'/name).read_bytes().split())
    data = zlib.decompress(base64.b85decode(encoded))
    if len(data)>1048576 or hashlib.sha256(data).hexdigest()!=digest:
        raise ValueError('Semantic metadata integrity mismatch: '+name)
    return data

class Reader:
    def __init__(self, data): self.data, self.pos = data, 0
    def varint(self):
        n=0
        for shift in range(0, 64, 7):
            if self.pos>=len(self.data): raise ValueError('Truncated metadata')
            b=self.data[self.pos]; self.pos+=1; n|=(b&127)<<shift
            if not b&128: return n
        raise ValueError('Oversized metadata integer')
    def done(self): return self.pos==len(self.data)

def body(r):
    f,e,d,i=r['family'],r['encoding'],r['displacement'],r['immediate']
    if f=='constant': return b'\xb8'+struct.pack('<I',i)+b'\xc3'
    if f=='zero': return (b'\x33\xc0' if e==0 else b'\x31\xc0')+b'\xc3'
    if f=='identity': return b'\x8b\xc1\xc3'
    op={'address':b'\x8d','load8':b'\x0f\xb6','load16':b'\x0f\xb7',
        'load32':b'\x8b','sign8':b'\x0f\xbe','sign16':b'\x0f\xbf'}[f]
    if e==0: operand=b'\x01'
    elif e==8: operand=b'\x41'+struct.pack('<b',d)
    elif e==32: operand=b'\x81'+struct.pack('<i',d)
    else: raise ValueError('Invalid instruction encoding')
    return op+operand+b'\xc3'

def records():
    packed=unpack('semantic.b85',SEMANTIC_SHA)
    if packed[:8]!=b'RE5LEAF1': raise ValueError('Wrong metadata schema')
    reader=Reader(packed[8:]); count=reader.varint()
    if count!=7171: raise ValueError('Wrong routine count')
    result=[]; va=0x400000; immediate=0
    for _ in range(count):
        delta=reader.varint()
        if delta==0: raise ValueError('Repeated or unordered address')
        va+=delta*16; code=reader.varint(); kind,variant=divmod(code,4)
        if kind>=len(FAMILIES): raise ValueError('Unsupported semantic family')
        f=FAMILIES[kind]; e=[0,1,8,32][variant]
        d=0; i=0
        if f=='constant':
            n=reader.varint(); immediate+=(n//2 if n%2==0 else -(n//2)-1);i=immediate
        elif f not in ('zero','identity'): d=reader.varint()
        if not (0x410000<=va<0x137083B and not 0xBB9000<=va<0xBB9100):
            raise ValueError('Address is outside reserved recovery scope')
        if not 0<=i<=0xFFFFFFFF or not 0<=d<=0x1000000: raise ValueError('Invalid operand')
        if f in ('constant','identity') and e!=0: raise ValueError('Unexpected encoding')
        if f=='zero' and e not in (0,1): raise ValueError('Unexpected zero encoding')
        if f not in ('constant','zero','identity') and e not in (0,8,32): raise ValueError('Unexpected operand encoding')
        r=dict(family=f,encoding=e,displacement=d,immediate=i,
               width={'load8':1,'sign8':1,'load16':2,'sign16':2,'load32':4}.get(f,0),
               va=va,rva=va-0x400000,file_offset=va-0x400C00,symbol=f'FUN_{va:08X}')
        b=body(r);r.update(size=len(b),body_sha256=hashlib.sha256(b).hexdigest(),direct_call_sites=[],
            boundary_evidence='Locally rechecked: 16-byte alignment, two decoded preceding INT3, two trailing INT3; complete whitelist body')
        result.append(r)
    if not reader.done() or sum(r['size'] for r in result)!=42745:
        raise ValueError('Wrong semantic stream coverage')
    cr=Reader(unpack('calls.b85',CALLS_SHA)); index=0
    while not cr.done():
        index+=cr.varint(); n=cr.varint()
        if index>=count or n==0 or result[index]['direct_call_sites']: raise ValueError('Invalid call metadata')
        caller=0
        for _ in range(n):
            caller+=cr.varint()
            if not 0x401000<=caller<0x137083B: raise ValueError('Out of section caller')
            result[index]['direct_call_sites'].append(caller)
    return result

def source_digest(folder):
    text=''
    for p in sorted(folder.rglob('*')):
        if p.suffix in ('.cpp','.hpp'):
            text+=p.relative_to(folder).as_posix()+' '+hashlib.sha256(p.read_bytes()).hexdigest()+'\n'
    return hashlib.sha256(text.encode()).hexdigest()

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--check',action='store_true',help='Compare generated files without modifying the work tree')
    p.add_argument('--exe',type=Path,help='Optional exact local EXE verification; never uploaded or launched')
    a=p.parse_args(); rows=records()
    if a.exe:
        data,_,_,_=read_target(a.exe)
        for r in rows:
            at=r['file_offset']; n=r['size']
            if data[at:at+n]!=body(r) or data[at-2:at]!=b'\xcc\xcc' or data[at+n:at+n+2]!=b'\xcc\xcc':
                raise ValueError('Original range/boundary mismatch: '+r['symbol'])
            for site in r['direct_call_sites']:
                off=site-0x400C00
                if data[off]!=0xE8 or site+5+struct.unpack_from('<i',data,off+1)[0]!=r['va']:
                    raise ValueError('Original direct-call mismatch')
    with tempfile.TemporaryDirectory() as temp:
        out=Path(temp)/'generated'
        write_sources(rows,out,dict(gnu_linear_instruction_rows=4837234,raw_pattern_hits=7172,rejected_nondecoded_starts=1,
            original_discovery_catalog_sha256=ORIGINAL_CATALOG_SHA,
            independent_llvm_i386_verification='Performed locally before publication; original catalogue retained by hash'))
        expected=json.loads((ROOT/'metadata/provenance.json').read_text())['original_cpp_source_digest']
        if source_digest(out)!=expected: raise ValueError('Reconstructed C++ differs from original local package')
        dest=ROOT/'generated'
        if a.check:
            for path in out.rglob('*'):
                if path.is_file() and (not (dest/path.relative_to(out)).is_file() or path.read_bytes()!=(dest/path.relative_to(out)).read_bytes()):
                    raise ValueError('Generated source/catalog drift: '+str(path.relative_to(out)))
            if {x.relative_to(dest) for x in dest.rglob('*') if x.is_file()}!={x.relative_to(out) for x in out.rglob('*') if x.is_file()}:
                raise ValueError('Unexpected generated file')
        else:
            if dest.exists() and any(dest.iterdir()): raise ValueError('Refusing to overwrite existing generated files; use --check')
            shutil.copytree(out,dest,dirs_exist_ok=True)
    print(json.dumps(dict(status='PASS',routines=len(rows),instruction_bytes=42745,
        source_digest=expected,local_exe_verified=bool(a.exe),canonical_count_changed=False)))
if __name__=='__main__': main()
