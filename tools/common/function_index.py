from __future__ import annotations

import base64, json, pathlib, struct, zlib

ROOT=pathlib.Path(__file__).resolve().parents[2]
INDEX=ROOT/'database'/'function_index'


def _varint(data:bytes,pos:int):
    value=0; shift=0
    while True:
        b=data[pos]; pos+=1; value|=(b&0x7f)<<shift
        if not b&0x80: return value,pos
        shift+=7


def manifest():
    p=INDEX/'manifest.json'
    return json.loads(p.read_text(encoding='utf-8')) if p.exists() else None


def records():
    m=manifest()
    if not m: return []
    payload=''.join((INDEX/name).read_text(encoding='ascii') for name in m['shards'])
    data=zlib.decompress(base64.b85decode(payload.encode('ascii')))
    if data[:8]!=b'RE5FIDX1': raise ValueError('invalid function-index magic')
    text_start,text_end,count=struct.unpack_from('<III',data,8); pos=20; prev=text_start; out=[]
    for _ in range(count):
        delta,pos=_varint(data,pos); size,pos=_varint(data,pos); flags,call_count,pad_count=data[pos:pos+3]; pos+=3; va=prev+delta; prev=va
        confidence='CONFIRMED' if flags&4 else ('HIGH' if ((flags&1 and flags&2) or call_count>=2) else 'MEDIUM')
        out.append({'stable_id':f'FUN_{va:08X}','va':f'0x{va:08X}','rva':f'0x{va-0x00400000:08X}','section':'.text','raw_offset':f'0x{0x400+(va-text_start):08X}','size':str(size),'workflow_status':'DISCOVERED','confidence':confidence,'detection_padding':'true' if flags&1 else 'false','detection_direct_call':'true' if flags&2 else 'false','detection_entry':'true' if flags&4 else 'false','direct_call_count':str(call_count),'padding_bytes':str(pad_count)})
    if len(out)!=count: raise ValueError('function-index record count mismatch')
    return out
