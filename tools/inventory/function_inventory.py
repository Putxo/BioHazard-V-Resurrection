#!/usr/bin/env python3
"""Build the canonical RE5 function-candidate index from a local target EXE.

Evidence used in Phase 11:
  * PE entry point (CONFIRMED root)
  * decoded direct CALL targets inside .text
  * decoded instruction starts immediately after >=2 INT3 padding bytes

The tool deliberately does not treat arbitrary prologue-looking byte patterns,
JMP targets, or arbitrary .rdata/.data pointers as function starts. Those sources
produce too many basic-block/switch-table false positives. Later inventory/refine
passes may add/remove candidates with explicit history and evidence.

The output is a compact, versionable index. Semantic/tier metadata remains sparse
and is keyed by stable ID in database/functions.csv.
"""
from __future__ import annotations

import argparse, base64, collections, hashlib, json, pathlib, re, struct, subprocess, tempfile, zlib

MAGIC=b'RE5FIDX1'
INSTRUCTION_RE=re.compile(r'^\s*([0-9a-f]+):\s+(?:(?:[0-9a-f]{2})\s+)+\s*([a-z][a-z0-9.]*)\s*(.*)$')


def u16(data,o): return struct.unpack_from('<H',data,o)[0]
def u32(data,o): return struct.unpack_from('<I',data,o)[0]
def varint(n:int)->bytes:
    out=bytearray()
    while True:
        b=n&0x7f; n >>= 7
        if n: out.append(b|0x80)
        else: out.append(b); return bytes(out)


def parse_text(data:bytes):
    pe=u32(data,0x3c); opt=pe+24; image_base=u32(data,opt+28); entry_rva=u32(data,opt+16)
    nsects=u16(data,pe+6); opt_size=u16(data,pe+20); sec=opt+opt_size
    for i in range(nsects):
        o=sec+i*40; name=data[o:o+8].rstrip(b'\0').decode('latin1')
        if name=='.text':
            vsize,rva,raw_size,raw_off=struct.unpack_from('<IIII',data,o+8)
            return image_base, image_base+entry_rva, image_base+rva, vsize, raw_off, raw_size
    raise ValueError('missing .text')


def build(binary:pathlib.Path,objdump='objdump'):
    data=binary.read_bytes(); image_base,entry,text_va,vsize,raw_off,raw_size=parse_text(data); text_end=text_va+vsize
    inst=set(); calls=collections.Counter()
    with tempfile.NamedTemporaryFile(mode='w+',encoding='utf-8') as tmp:
        subprocess.run([objdump,'-d','-Mintel',str(binary)],stdout=tmp,check=True)
        tmp.flush(); tmp.seek(0)
        for line in tmp:
            m=INSTRUCTION_RE.match(line)
            if not m: continue
            va=int(m.group(1),16)
            if not text_va<=va<text_end: continue
            inst.add(va)
            if m.group(2)=='call':
                op=m.group(3).strip()
                if op.startswith('0x'):
                    try:
                        target=int(op.split()[0],16)
                    except ValueError: continue
                    if text_va<=target<text_end: calls[target]+=1
    raw=data[raw_off:raw_off+vsize]
    padding={}; i=0
    while i<len(raw):
        if raw[i]==0xcc:
            j=i+1
            while j<len(raw) and raw[j]==0xcc: j+=1
            if j-i>=2:
                va=text_va+j
                if va in inst: padding[va]=j-i
            i=j
        else: i+=1
    roots=sorted((set(padding)|set(calls)|{entry}) & inst)
    records=[]
    for i,va in enumerate(roots):
        nxt=roots[i+1] if i+1<len(roots) else text_end
        start=va-text_va; end=nxt-text_va
        while end>start and raw[end-1]==0xcc: end-=1
        size=max(1,end-start)
        cc=padding.get(va,0); call_count=calls.get(va,0)
        flags=(1 if cc else 0)|(2 if call_count else 0)|(4 if va==entry else 0)
        confidence='CONFIRMED' if va==entry else ('HIGH' if (cc and call_count) or call_count>=2 else 'MEDIUM')
        records.append((va,size,flags,min(call_count,255),min(cc,255),confidence))
    blob=bytearray(MAGIC)+struct.pack('<III',text_va,text_end,len(records)); prev=text_va
    for va,size,flags,calls8,pad8,_ in records:
        blob+=varint(va-prev)+varint(size)+bytes([flags,calls8,pad8]); prev=va
    packed=zlib.compress(bytes(blob),9); payload=base64.b85encode(packed).decode('ascii')
    conf=collections.Counter(r[5] for r in records)
    summary={'format':'RE5FIDX1','target_sha256':hashlib.sha256(data).hexdigest(),'count':len(records),'text_start':f'0x{text_va:08X}','text_end':f'0x{text_end:08X}','text_virtual_bytes':vsize,'estimated_function_bytes':sum(r[1] for r in records),'confidence':dict(conf),'padding_evidence':sum(bool(r[2]&1) for r in records),'direct_call_evidence':sum(bool(r[2]&2) for r in records),'entry_evidence':sum(bool(r[2]&4) for r in records),'compressed_bytes':len(packed),'base85_chars':len(payload)}
    return payload,summary


def main():
    ap=argparse.ArgumentParser(); ap.add_argument('binary'); ap.add_argument('--out',default='database/function_index'); ap.add_argument('--shard-chars',type=int,default=24000); ap.add_argument('--objdump',default='objdump'); args=ap.parse_args()
    payload,summary=build(pathlib.Path(args.binary),args.objdump); out=pathlib.Path(args.out); out.mkdir(parents=True,exist_ok=True)
    for old in out.glob('part-*.b85'): old.unlink()
    shards=[payload[i:i+args.shard_chars] for i in range(0,len(payload),args.shard_chars)]
    for i,chunk in enumerate(shards): (out/f'part-{i:03d}.b85').write_text(chunk,encoding='ascii')
    summary['shards']=[f'part-{i:03d}.b85' for i in range(len(shards))]
    (out/'manifest.json').write_text(json.dumps(summary,indent=2,sort_keys=True)+'\n',encoding='utf-8')
    print(json.dumps(summary,indent=2,sort_keys=True))

if __name__=='__main__': main()
