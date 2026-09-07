#!/usr/bin/env python3
"""Deterministic PE32 inventory for the locally supplied RE5 1.1.0 Dev target.

This tool never writes or copies the executable into the repository. It emits only
metadata derived from a user-supplied local file.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import pathlib
import struct
from collections import Counter

DIRECTORY_NAMES = [
    'EXPORT','IMPORT','RESOURCE','EXCEPTION','SECURITY','BASERELOC','DEBUG',
    'ARCHITECTURE','GLOBALPTR','TLS','LOAD_CONFIG','BOUND_IMPORT','IAT',
    'DELAY_IMPORT','COM_DESCRIPTOR','RESERVED'
]


def u16(data, off): return struct.unpack_from('<H', data, off)[0]
def u32(data, off): return struct.unpack_from('<I', data, off)[0]


def digest(path):
    h=hashlib.sha256()
    with open(path,'rb') as f:
        for chunk in iter(lambda:f.read(1024*1024), b''): h.update(chunk)
    return h.hexdigest()


def inventory(path: pathlib.Path):
    data=path.read_bytes()
    if len(data)<0x40 or data[:2]!=b'MZ': raise ValueError('not an MZ executable')
    pe=u32(data,0x3C)
    if pe+24>len(data) or data[pe:pe+4]!=b'PE\0\0': raise ValueError('missing PE signature')
    machine=u16(data,pe+4); nsects=u16(data,pe+6); timestamp=u32(data,pe+8); opt_size=u16(data,pe+20); characteristics=u16(data,pe+22)
    opt=pe+24
    if u16(data,opt)!=0x10B: raise ValueError('expected PE32 (not PE32+)')
    entry_rva=u32(data,opt+16); image_base=u32(data,opt+28); section_alignment=u32(data,opt+32); file_alignment=u32(data,opt+36)
    size_image=u32(data,opt+56); size_headers=u32(data,opt+60); checksum=u32(data,opt+64); subsystem=u16(data,opt+68); dllchars=u16(data,opt+70)
    ndirs=min(u32(data,opt+92),16)
    dirs=[(u32(data,opt+96+i*8),u32(data,opt+100+i*8)) for i in range(ndirs)]
    while len(dirs)<16: dirs.append((0,0))
    sec_off=opt+opt_size; sections=[]
    for i in range(nsects):
        o=sec_off+i*40; name=data[o:o+8].rstrip(b'\0').decode('latin1')
        vsize,rva,raw_size,raw_off=struct.unpack_from('<IIII',data,o+8); schars=u32(data,o+36)
        sections.append({'name':name,'rva':rva,'va':image_base+rva,'virtual_size':vsize,'raw_offset':raw_off,'raw_size':raw_size,'characteristics':schars})
    def rva_to_off(rva):
        if rva<size_headers: return rva
        for s in sections:
            if s['rva']<=rva<s['rva']+max(s['virtual_size'],s['raw_size']):
                delta=rva-s['rva']
                return s['raw_offset']+delta if delta<s['raw_size'] else None
        return None
    imports=[]
    imp_rva,_=dirs[1]
    o=rva_to_off(imp_rva) if imp_rva else None
    while o is not None:
        oft,_,_,name_rva,ft=struct.unpack_from('<IIIII',data,o)
        if not any(struct.unpack_from('<IIIII',data,o)): break
        no=rva_to_off(name_rva); end=data.find(b'\0',no); dll=data[no:end].decode('ascii','replace')
        to=rva_to_off(oft or ft); index=0
        while True:
            val=u32(data,to+index*4)
            if not val: break
            if val & 0x80000000:
                kind='ordinal'; symbol=str(val & 0xffff)
            else:
                ho=rva_to_off(val); kind='name'; symbol=data[ho+2:data.find(b'\0',ho+2)].decode('ascii','replace')
            imports.append({'dll':dll,'kind':kind,'symbol':symbol,'iat_rva':ft+index*4,'iat_va':image_base+ft+index*4})
            index+=1
        o+=20
    debug=[]
    dbg_rva,dbg_size=dirs[6]
    do=rva_to_off(dbg_rva) if dbg_rva else None
    if do is not None:
        for i in range(dbg_size//28):
            _,dts,major,minor,typ,size,addr,ptr=struct.unpack_from('<IIHHIIII',data,do+i*28)
            rec={'type':typ,'timestamp':dts,'major':major,'minor':minor,'size':size,'address_rva':addr,'raw_pointer':ptr}
            blob=data[ptr:ptr+size]
            if typ==2 and blob[:4]==b'RSDS' and len(blob)>=24:
                rec.update({'codeview_signature':'RSDS','guid':blob[4:20].hex(),'age':u32(blob,20),'pdb_path':blob[24:].split(b'\0',1)[0].decode('latin1','replace')})
            debug.append(rec)
    return {
      'file':{'size':len(data),'sha256':digest(path)},
      'pe':{'machine':machine,'section_count':nsects,'timestamp':timestamp,'characteristics':characteristics,'optional_magic':0x10B,'entry_rva':entry_rva,'entry_va':image_base+entry_rva,'image_base':image_base,'section_alignment':section_alignment,'file_alignment':file_alignment,'size_image':size_image,'size_headers':size_headers,'checksum':checksum,'subsystem':subsystem,'dll_characteristics':dllchars},
      'sections':sections,
      'directories':[{'index':i,'name':DIRECTORY_NAMES[i],'rva':r,'size':s,'present':bool(r or s)} for i,(r,s) in enumerate(dirs)],
      'imports':imports,
      'import_summary':dict(Counter(x['dll'] for x in imports)),
      'debug':debug,
    }


def main():
    ap=argparse.ArgumentParser(); ap.add_argument('binary'); ap.add_argument('--json',help='optional metadata output path'); args=ap.parse_args()
    result=inventory(pathlib.Path(args.binary))
    text=json.dumps(result,indent=2,sort_keys=True)
    if args.json: pathlib.Path(args.json).write_text(text+'\n',encoding='utf-8')
    else: print(text)

if __name__=='__main__': main()
