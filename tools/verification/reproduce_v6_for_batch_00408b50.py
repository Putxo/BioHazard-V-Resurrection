#!/usr/bin/env python3
"""Reproduce the frozen candidate-union criteria from the exact local EXE.

This is an independent reconstruction of sets, not a claim that a previously
unavailable CSV was read byte-for-byte. It never changes the canonical freeze,
counts candidates as implementations, or uploads input bytes.
"""
from __future__ import annotations
import argparse, collections, hashlib, json, pathlib, struct, subprocess
from verify_batch_00408b50 import SHA, SIZE, RANGES, pe_sections, require

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--exe',type=pathlib.Path,required=True)
    ap.add_argument('--report',type=pathlib.Path,required=True)
    ap.add_argument('--objdump',default='objdump')
    args=ap.parse_args(); data=args.exe.read_bytes(); sections=pe_sections(data)
    _,base,length,offset,_=next(s for s in sections if s[0]=='.text')
    raw=data[offset:offset+length]
    padding={base+i for i in range(0,length,16) if i>=2 and raw[i-1] in (0xCC,0x90)
             and raw[i-2] in (0xCC,0x90) and raw[i] not in (0xCC,0x90)}
    calls=collections.Counter(); sites={va:[] for va in RANGES}; decoded_rows=0
    with subprocess.Popen([args.objdump,'-d','-j','.text','-Mintel',str(args.exe)],stdout=subprocess.PIPE,text=True) as proc:
        for line in proc.stdout:
            parts=line.split('\t')
            if len(parts)<3: continue
            try: address=int(parts[0].strip().rstrip(':'),16)
            except ValueError: continue
            if not base<=address<base+length: continue
            decoded_rows+=1
            ins=parts[-1].split()
            if len(ins)<2 or ins[0]!='call' or not ins[1].startswith('0x'): continue
            try: target=int(ins[1],16)
            except ValueError: continue
            if base<=target<base+length:
                calls[target]+=1
                if target in sites: sites[target].append(address)
        require(proc.wait()==0, 'objdump failed')
    # The V6 pointer-sequence set includes PE headers and ALL non-.text raw
    # sections, including .rsrc. They remain candidate evidence, not symbols.
    segments=[(0,1024)]+[(off,raw_size) for name,_,_,off,raw_size in sections if name!='.text']
    pointers=set()
    for off,size in segments:
        run=[]
        for (word,) in struct.iter_unpack('<I',data[off:off+size]):
            if base<=word<base+length: run.append(word)
            else:
                if len(run)>=2: pointers.update(run)
                run=[]
        if len(run)>=2: pointers.update(run)
    pe=struct.unpack_from('<I',data,0x3C)[0]
    entry=0x400000+struct.unpack_from('<I',data,pe+24+16)[0]
    direct_roots=set(calls)|{entry}
    roots=direct_roots|pointers|padding
    require((len(calls),len(direct_roots),len(pointers),len(padding),len(roots))==(19194,19195,40912,69076,79782),
            'Frozen aggregate cross-check differs; do not publish changed denominator')
    # The recorded CSV ordering anchors refer to the direct-root group's lines,
    # counting its header. Record, but do not pretend an original CSV was read.
    sorted_direct=sorted(direct_roots)
    anchors={0x408620:137,0x408630:138,0x408640:139,0x4086A0:140,
             0x408700:141,0x408AF0:142,0x408B10:143,0x408B30:144}
    require(all(sorted_direct.index(va)+2==line for va,line in anchors.items()), 'Known row anchors disagree')
    members=[]
    for va in RANGES:
        require(va in roots,'Batch address outside reproduced universe')
        evidence=[]
        if va in direct_roots: evidence.append('direct CALL target' if va in calls else 'PE entry point')
        if va in pointers: evidence.append('aligned sequence of >=2 code pointers outside .text')
        if va in padding: evidence.append('16-byte-aligned start after >=2 INT3/NOP bytes')
        members.append({'stable_id':f'FUN_{va:08X}','va':va,'rva':va-0x400000,
                        'direct_calls':calls[va],'direct_call_sites':[f'0x{x:08X}' for x in sites[va]],
                        'direct_group_csv_line_with_header':sorted_direct.index(va)+2 if va in direct_roots else None,
                        'evidence':evidence})
    report={'status':'PASS','input_sha256':SHA,'input_size':SIZE,
            'method':'Independent set reconstruction under frozen V6 criteria; original local CSV unavailable in this session.',
            'decoded_objdump_rows':decoded_rows,'actual_direct_call_targets':len(calls),
            'direct_roots_including_PE_entry':len(direct_roots),'aligned_pointer_sequence_targets':len(pointers),
            'aligned_post_padding_starts':len(padding),'candidate_union':len(roots),
            'sorted_union_u32le_sha256':hashlib.sha256(b''.join(struct.pack('<I',x) for x in sorted(roots))).hexdigest(),
            'known_direct_group_row_anchors_match':True,'canonical_denominator_changed':False,'members':members}
    args.report.write_text(json.dumps(report,indent=2)+'\n')
    print('PASS: independently reconstructed 79,782 candidates; all 14 batch addresses are members')
if __name__=='__main__': main()
