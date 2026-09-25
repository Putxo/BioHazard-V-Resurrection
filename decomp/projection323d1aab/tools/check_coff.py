#!/usr/bin/env python3
"""Inspect compiled I386 COFF definitions against locally recorded original hashes."""
import argparse,hashlib,json,pathlib,re,struct
from verify import records

def inspect(path):
    data=pathlib.Path(path).read_bytes();machine,nsects,_,symoff,nsym,optsize,_=struct.unpack_from('<HHIIIHH',data)
    if machine!=0x14c:raise ValueError('Expected I386 COFF')
    sections=[]
    for i in range(nsects):
        s=20+optsize+40*i;size,raw,reloc=struct.unpack_from('<III',data,s+16);nr=struct.unpack_from('<H',data,s+32)[0]
        sections.append((size,raw,[struct.unpack_from('<I',data,reloc+10*j)[0] for j in range(nr)]))
    names={};strings=symoff+18*nsym;i=0
    while i<nsym:
        s=symoff+18*i;name=data[s:s+8]
        if name[:4]==b'\0'*4:
            start=strings+struct.unpack_from('<I',name,4)[0];end=data.index(b'\0',start);name=data[start:end]
        name=name.rstrip(b'\0').decode('ascii',errors='replace')
        value,section,typ,storage,aux=struct.unpack_from('<IhHBB',data,s+8)
        m=re.search(r'FUN_[0-9A-F]{8}',name)
        if m and section>0 and typ==0x20 and storage==2:
            if m[0] in names:raise ValueError('Duplicate definition')
            names[m[0]]=(section,value)
        i+=1+aux
    rows=records()
    if set(names)!={r['symbol'] for r in rows}:raise ValueError('Missing/extra named definitions')
    exact=[]
    for r in rows:
        section,value=names[r['symbol']];size,raw,relocations=sections[section-1];end=value+r['size']
        if end<=size and not any(value<=a<end for a in relocations) and hashlib.sha256(data[raw+value:raw+end]).hexdigest()==r['sha256']:exact.append(r)
    return {'machine':'I386','named_definitions':len(names),'exact_bodies':len(exact),'exact_instruction_bytes':sum(r['size'] for r in exact),'object_sha256':hashlib.sha256(data).hexdigest(),'not_a_linked_game':True}
if __name__=='__main__':
    ap=argparse.ArgumentParser();ap.add_argument('object');ap.add_argument('--json');args=ap.parse_args();result=inspect(args.object);text=json.dumps(result,indent=2)+'\n';print(text,end='')
    if args.json:pathlib.Path(args.json).write_text(text)
