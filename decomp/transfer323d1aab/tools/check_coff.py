#!/usr/bin/env python3
"""Check real I386 object definitions and compare relocation-free body ranges."""
import argparse,hashlib,json,pathlib,re,struct
from verify import records

def inspect(path):
    data=pathlib.Path(path).read_bytes();machine,nsects,_,symoff,nsym,optsize,_=struct.unpack_from('<HHIIIHH',data)
    if machine!=0x14c:raise ValueError('Not I386 COFF')
    sections=[]
    for i in range(nsects):
        p=20+optsize+40*i;size,raw,reloc=struct.unpack_from('<III',data,p+16);nr=struct.unpack_from('<H',data,p+32)[0]
        sections.append((size,raw,[struct.unpack_from('<I',data,reloc+10*j)[0] for j in range(nr)]))
    names={};strings=symoff+18*nsym;i=0
    while i<nsym:
        p=symoff+18*i;name=data[p:p+8]
        if name[:4]==b'\0'*4:
            start=strings+struct.unpack_from('<I',name,4)[0];name=data[start:data.index(b'\0',start)]
        name=name.rstrip(b'\0').decode('ascii',errors='replace');value,section,typ,storage,aux=struct.unpack_from('<IhHBB',data,p+8)
        m=re.search(r'FUN_[0-9A-F]{8}',name)
        if m and section>0 and typ==0x20 and storage==2:
            if m[0] in names:raise ValueError('Duplicate definition')
            names[m[0]]=(section,value)
        i+=1+aux
    rows=records()
    if set(names)!={r['symbol'] for r in rows}:raise ValueError('Missing or extra definition')
    matched=[]
    for r in rows:
        section,value=names[r['symbol']];size,raw,relocs=sections[section-1];end=value+r['size']
        limit=min([pos for sec,pos in names.values() if sec==section and pos>value]+[size])
        if end<=limit and not any(value<=a<end for a in relocs) and hashlib.sha256(data[raw+value:raw+end]).hexdigest()==r['sha256']:matched.append(r)
    return dict(machine='I386',definitions=len(names),exact_bodies=len(matched),exact_instruction_bytes=sum(r['size'] for r in matched),object_sha256=hashlib.sha256(data).hexdigest(),game_linked=False)
if __name__=='__main__':
    ap=argparse.ArgumentParser();ap.add_argument('object');ap.add_argument('--json');a=ap.parse_args();text=json.dumps(inspect(a.object),indent=2)+'\n';print(text,end='')
    if a.json:pathlib.Path(a.json).write_text(text)
