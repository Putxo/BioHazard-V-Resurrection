#!/usr/bin/env python3
"""Derive globals/XREF/string/region/export inventory from the local RE5 Dev target.

Requires an objdump text file produced locally from the exact target. The dump itself
is intentionally not stored in Git.
"""
from __future__ import annotations
import argparse, collections, hashlib, json, pathlib, re, struct


def main():
    ap=argparse.ArgumentParser(); ap.add_argument('binary'); ap.add_argument('objdump_text'); ap.add_argument('--out',default='database/inventory_remaining.generated.json'); args=ap.parse_args()
    p=pathlib.Path(args.binary); data=p.read_bytes(); u16=lambda o:struct.unpack_from('<H',data,o)[0]; u32=lambda o:struct.unpack_from('<I',data,o)[0]
    pe=u32(0x3c); opt=pe+24; base=u32(opt+28); n=u16(pe+6); sh=opt+u16(pe+20); secs={}
    for i in range(n):
        o=sh+i*40; name=data[o:o+8].split(b'\0')[0].decode('latin1'); vs,rva,rs,ro=struct.unpack_from('<IIII',data,o+8); secs[name]=(base+rva,vs,ro,rs)
    text,rdata,dsec=secs['.text'],secs['.rdata'],secs['.data']; lineaddr=re.compile(r'^\s*([0-9a-fA-F]+):'); hexaddr=re.compile(r'0x([0-9a-fA-F]{6,8})')
    grefs=collections.Counter(); rdrefs=collections.Counter()
    with pathlib.Path(args.objdump_text).open(errors='ignore') as f:
        for line in f:
            lm=lineaddr.match(line)
            if not lm: continue
            src=int(lm.group(1),16)
            if not text[0]<=src<text[0]+text[1]: continue
            for m in hexaddr.finditer(line):
                va=int(m.group(1),16)
                if dsec[0]<=va<dsec[0]+dsec[1]: grefs[va]+=1
                elif rdata[0]<=va<rdata[0]+rdata[1]: rdrefs[va]+=1
    strings=[]; kw=re.compile(r'(player|enemy|stage|event|network|camera|render|sound|movie|save|item|weapon|coop|split|option|input|pad|mission|scenario|rank|damage|collision|physics|animation|resource|system|menu|ui|online)',re.I)
    for name,s in (('.rdata',rdata),('.data',dsec)):
        buf=data[s[2]:s[2]+s[3]]
        for m in re.finditer(rb'[\x20-\x7e]{4,}',buf): strings.append((s[0]+m.start(),m.group().decode('latin1','replace'),name))
    relevant=[x for x in strings if kw.search(x[1])]
    numdirs=u32(opt+92); export_rva=u32(opt+96) if numdirs else 0
    region_size=0x10000; regions=(text[1]+region_size-1)//region_size
    out={'target_sha256':hashlib.sha256(data).hexdigest(),'exports':0 if export_rva==0 else None,'globals':{'referenced_data_addresses':len(grefs),'code_xrefs':sum(grefs.values())},'rdata_references':{'referenced_addresses':len(rdrefs),'code_xrefs':sum(rdrefs.values())},'strings':{'ascii_total':len(strings),'relevant_keyword_matches':len(relevant)},'regions':{'region_size':region_size,'count':regions}}
    pathlib.Path(args.out).write_text(json.dumps(out,indent=2,sort_keys=True)+'\n'); print(json.dumps(out,indent=2,sort_keys=True))
if __name__=='__main__': main()
