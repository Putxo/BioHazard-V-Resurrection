#!/usr/bin/env python3
import argparse, hashlib, json, pathlib, struct, sys


def sha256(path):
    h = hashlib.sha256()
    with open(path, 'rb') as f:
        for chunk in iter(lambda: f.read(1024 * 1024), b''):
            h.update(chunk)
    return h.hexdigest()


def pe_metadata(path):
    data = pathlib.Path(path).read_bytes()
    if len(data) < 0x40 or data[:2] != b'MZ':
        raise ValueError('not an MZ executable')
    pe_off = struct.unpack_from('<I', data, 0x3C)[0]
    if pe_off + 24 > len(data) or data[pe_off:pe_off+4] != b'PE\0\0':
        raise ValueError('missing PE signature')
    machine, sections, timestamp = struct.unpack_from('<HHI', data, pe_off + 4)
    opt_size = struct.unpack_from('<H', data, pe_off + 20)[0]
    opt = pe_off + 24
    if opt + opt_size > len(data):
        raise ValueError('truncated optional header')
    magic = struct.unpack_from('<H', data, opt)[0]
    if magic != 0x10B:
        raise ValueError('target is not PE32')
    entry_rva = struct.unpack_from('<I', data, opt + 16)[0]
    image_base = struct.unpack_from('<I', data, opt + 28)[0]
    return {
        'file_size': len(data), 'sha256': sha256(path), 'machine': machine,
        'pe_timestamp': timestamp, 'image_base': image_base,
        'entry_point': image_base + entry_rva, 'section_count': sections,
    }


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('binary', nargs='?', help='local target executable; never committed')
    ap.add_argument('--config', default='config/target.json')
    args = ap.parse_args()
    cfg = json.loads(pathlib.Path(args.config).read_text(encoding='utf-8'))
    if not args.binary:
        print('Target definition schema OK; binary verification skipped (no local binary supplied).')
        return 0
    md = pe_metadata(args.binary)
    mismatches = []
    for key in ('sha256','file_size','pe_timestamp','machine','image_base','entry_point'):
        expected = cfg.get(key)
        if expected is not None and expected != md[key]:
            mismatches.append((key, expected, md[key]))
    if mismatches:
        for k,e,a in mismatches: print(f'MISMATCH {k}: expected={e!r} actual={a!r}', file=sys.stderr)
        return 2
    print(json.dumps(md, indent=2))
    return 0

if __name__ == '__main__':
    raise SystemExit(main())
