#!/usr/bin/env python3
"""Inventory MSVC x86 RTTI, vtables, and aligned code pointers from a local RE5 target."""
from __future__ import annotations
import argparse, hashlib, json, pathlib, re, struct


def u16(data, off): return struct.unpack_from('<H', data, off)[0]
def u32(data, off): return struct.unpack_from('<I', data, off)[0]


def sections(data: bytes):
    pe = u32(data, 0x3C); opt = pe + 24; image_base = u32(data, opt + 28)
    count = u16(data, pe + 6); opt_size = u16(data, pe + 20); sh = opt + opt_size
    out = []
    for i in range(count):
        o = sh + i * 40
        name = data[o:o+8].split(b'\0')[0].decode('latin1')
        vsize, rva, rawsize, raw = struct.unpack_from('<IIII', data, o + 8)
        out.append({'name': name, 'va': image_base + rva, 'vsize': vsize,
                    'raw': raw, 'rawsize': rawsize})
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('binary')
    ap.add_argument('--summary', default='database/rtti_summary.json')
    ap.add_argument('--vtables', default='reports/vtables.json')
    args = ap.parse_args()
    path = pathlib.Path(args.binary); data = path.read_bytes(); secs = sections(data)
    by_name = {s['name']: s for s in secs}; text = by_name['.text']; rdata = by_name['.rdata']; dsec = by_name['.data']
    text_start, text_end = text['va'], text['va'] + text['vsize']
    def in_range(va, s): return s['va'] <= va < s['va'] + s['vsize']
    dbuf = data[dsec['raw']:dsec['raw'] + dsec['rawsize']]
    type_descriptors = {}
    for m in re.finditer(rb'\.\?[AVUT][A-Z][^\x00]{1,240}\x00', dbuf):
        name = m.group()[:-1].decode('latin1', 'replace')
        name_va = dsec['va'] + m.start(); td_va = name_va - 8
        if td_va >= dsec['va']: type_descriptors[td_va] = name
    rbuf = data[rdata['raw']:rdata['raw'] + rdata['rawsize']]
    cols = {}
    for rel in range(0, len(rbuf) - 20, 4):
        sig, obj_off, cd_off, ptd, pchd = struct.unpack_from('<IIIII', rbuf, rel)
        va = rdata['va'] + rel
        if sig == 0 and ptd in type_descriptors and in_range(pchd, rdata):
            cols[va] = {'col_va': va, 'object_offset': obj_off, 'cd_offset': cd_off,
                        'type_descriptor_va': ptd, 'class_hierarchy_va': pchd,
                        'type_name': type_descriptors[ptd]}
    vtables = {}
    method_targets = set()
    for rel in range(0, len(rbuf) - 8, 4):
        col = struct.unpack_from('<I', rbuf, rel)[0]
        if col not in cols: continue
        j = rel + 4; methods = []
        while j + 4 <= len(rbuf):
            fn = struct.unpack_from('<I', rbuf, j)[0]
            if not text_start <= fn < text_end: break
            methods.append(fn); method_targets.add(fn); j += 4
        if methods:
            entry = dict(cols[col]); entry.update(vtable_va=rdata['va'] + rel + 4,
                                                   method_count=len(methods),
                                                   methods=methods)
            vtables[entry['vtable_va']] = entry
    pointer_summary = {}
    for s in (rdata, dsec):
        buf = data[s['raw']:s['raw'] + s['rawsize']]; targets = []
        for rel in range(0, len(buf) - 3, 4):
            val = struct.unpack_from('<I', buf, rel)[0]
            if text_start <= val < text_end: targets.append(val)
        pointer_summary[s['name']] = {'count': len(targets), 'unique_targets': len(set(targets))}
    summary = {
        'target_sha256': hashlib.sha256(data).hexdigest(),
        'type_descriptors': len(type_descriptors),
        'complete_object_locators': len(cols),
        'vtables': len(vtables),
        'vtable_methods_total': sum(v['method_count'] for v in vtables.values()),
        'vtable_method_targets_unique': len(method_targets),
        'code_pointers': pointer_summary,
        'methodology': {
            'rtti': 'MSVC x86 TypeDescriptor + CompleteObjectLocator validation',
            'vtable': 'pointer-to-COL immediately preceding one or more .text pointers',
            'code_pointer': 'aligned absolute VA inside .text'
        }
    }
    pathlib.Path(args.summary).write_text(json.dumps(summary, indent=2, sort_keys=True) + '\n')
    pathlib.Path(args.vtables).write_text(json.dumps(sorted(vtables.values(), key=lambda x: x['vtable_va']), indent=2) + '\n')
    print(json.dumps(summary, indent=2, sort_keys=True))

if __name__ == '__main__': main()
