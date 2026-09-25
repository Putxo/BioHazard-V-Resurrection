#!/usr/bin/env python3
"""Compile recovered C++ to I386 COFF and compare every original leaf body.

Objects are emitted only to the explicit build directory, never to the source
package. Matching excludes compiler alignment padding and does not imply a
linked game or authorize promotion of the canonical project metadata.
"""
from pathlib import Path
import argparse, concurrent.futures, hashlib, json, re, struct, subprocess
from verify_target import read_target, ROOT

def symbols(path):
    data=path.read_bytes()
    machine,ns,stamp,ptr,count,optional,flags=struct.unpack_from('<HHIIIHH',data,0)
    if machine!=0x14C or optional!=0 or ptr+count*18+4>len(data):
        raise ValueError(f'Not a supported I386 COFF object: {path.name}')
    string_table=ptr+count*18
    sections={}
    for i in range(ns):
        base=20+optional+40*i
        size,raw,reloc=struct.unpack_from('<III',data,base+16)
        nrel=struct.unpack_from('<H',data,base+32)[0]
        if raw+size>len(data) or reloc+nrel*10>len(data):
            raise ValueError('Out of range COFF section')
        relocations=[struct.unpack_from('<I',data,reloc+j*10)[0] for j in range(nrel)]
        sections[i+1]=(data[raw:raw+size],relocations)
    result={}
    i=0
    while i<count:
        off=ptr+18*i
        name_raw=data[off:off+8]
        if name_raw[:4]==b'\0'*4:
            at=string_table+struct.unpack_from('<I',name_raw,4)[0]
            end=data.index(b'\0',at)
            name=data[at:end].decode('ascii')
        else: name=name_raw.rstrip(b'\0').decode('ascii')
        value,section,typ,storage,aux=struct.unpack_from('<IhHBB',data,off+8)
        match=re.search(r'FUN_([0-9A-F]{8})',name)
        if match and section>0 and (typ & 0x20) and storage==2:
            va=int(match[1],16)
            if va in result: raise ValueError('Duplicate compiled function')
            section_data,relocations=sections[section]
            result[va]=(section_data[value:], [x-value for x in relocations if x>=value])
        i+=1+aux
    return result

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--exe',type=Path,required=True)
    p.add_argument('--build',type=Path,required=True)
    p.add_argument('--compiler',default='clang++')
    p.add_argument('--report',type=Path,required=True)
    a=p.parse_args()
    original,_,_,_=read_target(a.exe)
    source_root=ROOT.resolve()
    build_root=a.build.resolve()
    if build_root==source_root or source_root in build_root.parents:
        raise SystemExit('Use a build directory outside the deliverable source tree')
    a.build.mkdir(parents=True,exist_ok=True)
    flags=['--target=i686-pc-windows-msvc','-std=c++17','-O2','-ffreestanding',
           '-fno-exceptions','-fno-rtti','-Wall','-Wextra','-Wpedantic','-Werror',
           '-I'+str(ROOT/'generated/include')]
    sources=sorted((ROOT/'generated/src').glob('scalars_*.cpp'))
    def compile_one(source):
        target=a.build/(source.stem+'.obj')
        subprocess.run([a.compiler,*flags,'-c',str(source),'-o',str(target)],check=True,capture_output=True,text=True)
        return target
    with concurrent.futures.ThreadPoolExecutor(max_workers=4) as pool:
        objects=list(pool.map(compile_one,sources))
    compiled={}
    for obj in objects:
        for va,result in symbols(obj).items():
            if va in compiled: raise ValueError('Duplicate compiled definition across files')
            compiled[va]=result
    catalog=json.loads((ROOT/'generated/catalog.json').read_text())
    rows=catalog['records']
    if set(compiled)!={r['va'] for r in rows}: raise ValueError('Compiled symbol coverage mismatch')
    comparison=[]
    for r in rows:
        body,relocations=compiled[r['va']]
        n=r['size']; offset=r['file_offset']
        has_relocation=any(0<=x<n for x in relocations)
        same=not has_relocation and body[:n]==original[offset:offset+n]
        comparison.append(dict(symbol=r['symbol'],va=r['va'],original_instruction_bytes=n,
                               exact_body_match=same,relocation_in_body=has_relocation,
                               compiled_prefix_sha256=hashlib.sha256(body[:n]).hexdigest()))
    report=dict(test='windows_i386_coff_body_comparison',input_sha256=catalog['input_sha256'],
                compiler=subprocess.check_output([a.compiler,'--version'],text=True).splitlines()[0],
                compile_options=flags[:-1]+['-Igenerated/include'],
                compiled_functions=len(compiled),exact_body_matches=sum(x['exact_body_match'] for x in comparison),
                exact_body_bytes=sum(x['original_instruction_bytes'] for x in comparison if x['exact_body_match']),
                padding_included=False,game_linked=False,game_executed=False,
                canonical_matched_tier_changed=False,comparison=comparison,
                objects=[{'name':obj.name,'sha256':hashlib.sha256(obj.read_bytes()).hexdigest()} for obj in objects])
    a.report.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({k:v for k,v in report.items() if k not in ('comparison','objects')},indent=2))
if __name__=='__main__': main()
