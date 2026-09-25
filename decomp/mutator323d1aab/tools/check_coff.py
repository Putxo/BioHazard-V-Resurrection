#!/usr/bin/env python3
"""Inspect the actual I386 object; padding and relocated bodies never count as matches."""
import argparse,hashlib,json,pathlib,re,struct
from verify import records

def inspect(path):
    b=pathlib.Path(path).read_bytes();machine,count,_,syms,num,opt,_=struct.unpack_from('<HHIIIHH',b)
    if machine!=0x14c:raise ValueError('Not I386 COFF')
    sections=[]
    for i in range(count):
        at=20+opt+40*i;size,raw,reloc=struct.unpack_from('<III',b,at+16);n=struct.unpack_from('<H',b,at+32)[0]
        sections.append((size,raw,[struct.unpack_from('<I',b,reloc+10*j)[0] for j in range(n)]))
    names={};strings=syms+18*num;i=0
    while i<num:
        at=syms+18*i;name=b[at:at+8]
        if name[:4]==b'\0'*4:
            start=strings+struct.unpack_from('<I',name,4)[0];name=b[start:b.index(b'\0',start)]
        name=name.rstrip(b'\0').decode('ascii',errors='replace');value,section,typ,storage,aux=struct.unpack_from('<IhHBB',b,at+8)
        m=re.search(r'FUN_[0-9A-F]{8}',name)
        if m and section>0 and typ==0x20 and storage==2:
            if m[0] in names:raise ValueError('Duplicate named definition')
            names[m[0]]=(section,value)
        i+=1+aux
    rows=records()
    if set(names)!={r['symbol'] for r in rows}:raise ValueError('Missing/extra named definitions')
    matched=[]
    for r in rows:
        section,value=names[r['symbol']];size,raw,relocs=sections[section-1];end=value+r['size']
        if end<=size and not any(value<=a<end for a in relocs) and hashlib.sha256(b[raw+value:raw+end]).hexdigest()==r['sha256']:matched.append(r)
    return dict(machine='I386',definitions=len(names),exact_bodies=len(matched),exact_bytes=sum(r['size'] for r in matched),object_sha256=hashlib.sha256(b).hexdigest(),game_linked=False)
if __name__=='__main__':
    ap=argparse.ArgumentParser();ap.add_argument('object');ap.add_argument('--json');a=ap.parse_args();text=json.dumps(inspect(a.object),indent=2)+'\n';print(text,end='')
    if a.json:pathlib.Path(a.json).write_text(text)
