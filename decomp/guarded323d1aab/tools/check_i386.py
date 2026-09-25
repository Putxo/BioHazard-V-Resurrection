#!/usr/bin/env python3
"""Check actual I386 COFF definitions against independently recorded body hashes."""
from pathlib import Path
import argparse, hashlib, json, re, struct
from verify import get_rows

def symbols(path):
    data=Path(path).read_bytes()
    if len(data)<20:raise ValueError('Truncated COFF header')
    machine,ns,stamp,ptr,count,optional,flags=struct.unpack_from('<HHIIIHH',data)
    if machine!=0x14c or optional or ptr+count*18+4>len(data):raise ValueError('Expected ordinary I386 COFF object')
    string_table=ptr+count*18;sections={}
    for i in range(ns):
        base=20+40*i
        if base+40>len(data):raise ValueError('Truncated section header')
        size,raw,reloc=struct.unpack_from('<III',data,base+16)
        nrel=struct.unpack_from('<H',data,base+32)[0]
        if raw+size>len(data) or reloc+nrel*10>len(data):raise ValueError('Out-of-range section')
        sections[i+1]=(data[raw:raw+size],[struct.unpack_from('<I',data,reloc+j*10)[0] for j in range(nrel)])
    result={};i=0
    while i<count:
        off=ptr+18*i;rawname=data[off:off+8]
        if rawname[:4]==b'\0'*4:
            at=string_table+struct.unpack_from('<I',rawname,4)[0]
            if at<string_table+4 or at>=len(data):raise ValueError('Invalid symbol name')
            name=data[at:data.index(b'\0',at)].decode('ascii')
        else:name=rawname.rstrip(b'\0').decode('ascii')
        value,section,typ,storage,aux=struct.unpack_from('<IhHBB',data,off+8)
        match=re.search(r'FUN_([0-9A-F]{8})',name)
        if match and section>0 and typ&0x20 and storage==2:
            symbol='FUN_'+match[1]
            if symbol in result or section not in sections:raise ValueError('Duplicate or invalid definition')
            code,relocations=sections[section]
            if value>=len(code):raise ValueError('Definition outside section')
            result[symbol]=(code[value:],[x-value for x in relocations if x>=value])
        i+=1+aux
    return result

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('object');p.add_argument('--report');a=p.parse_args()
    rows=get_rows();compiled=symbols(a.object)
    if set(compiled)!={r['symbol'] for r in rows}:raise ValueError('Incomplete compiled symbol coverage')
    checks=[]
    for r in rows:
        code,relocations=compiled[r['symbol']];size=int(r['size'])
        actual=hashlib.sha256(code[:size]).hexdigest()
        exact=len(code)>=size and not any(0<=x<size for x in relocations) and actual==r['sha256']
        checks.append({'symbol':r['symbol'],'original_bytes':size,'exact_body_match':exact,'compiled_prefix_sha256':actual})
    report={'i386_definitions':len(compiled),'exact_body_matches':sum(x['exact_body_match'] for x in checks),
            'exact_body_bytes':sum(x['original_bytes'] for x in checks if x['exact_body_match']),
            'padding_included':False,'native_execution':False,'game_linked':False,'canonical_tier_changed':False,'checks':checks}
    if a.report:Path(a.report).write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({k:v for k,v in report.items() if k!='checks'},indent=2))
if __name__=='__main__':main()
