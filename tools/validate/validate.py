#!/usr/bin/env python3
from __future__ import annotations

import csv
import pathlib
import sys

ROOT = pathlib.Path(__file__).resolve().parents[2]
DB = ROOT / 'database'
VALID_STATUSES = {'UNSEEN','DISCOVERED','DISASSEMBLED','FAST_PASS','FAST_PASS_VALIDATED','CONVERTED','REFINED','VERIFIED','MATCHED','LINKED','BLOCKED'}
FORBIDDEN_EXTS = {'.exe','.dll','.pdb','.bin','.pak','.arc','.rom','.nds','.iso','.img'}


def rows(name: str):
    with (DB / name).open(newline='', encoding='utf-8') as f:
        return list(csv.DictReader(f))


def validate_functions(records):
    errors=[]; ids=set(); vas=set(); ranges=[]
    for index,r in enumerate(records,2):
        sid=r.get('stable_id',''); va=r.get('va',''); status=r.get('status','')
        if not sid.startswith('FUN_'): errors.append(f'functions.csv:{index}: invalid stable_id {sid!r}')
        if sid in ids: errors.append(f'functions.csv:{index}: duplicate stable_id {sid}')
        ids.add(sid)
        if va in vas: errors.append(f'functions.csv:{index}: duplicate VA {va}')
        vas.add(va)
        if status not in VALID_STATUSES: errors.append(f'functions.csv:{index}: invalid status {status}')
        if status in {'VERIFIED','MATCHED','LINKED'} and not r.get('evidence'):
            errors.append(f'functions.csv:{index}: {status} requires evidence')
        size=r.get('size','')
        if va and size:
            try:
                start=int(va,0); n=int(size,0); ranges.append((start,start+n,sid))
            except ValueError: errors.append(f'functions.csv:{index}: invalid VA/size')
    ranges.sort()
    for left,right in zip(ranges,ranges[1:]):
        if right[0] < left[1]: errors.append(f'function range overlap: {left[2]} / {right[2]}')
    return errors


def validate_claims(records):
    active={}; errors=[]
    for index,r in enumerate(records,2):
        if r.get('active','').lower() not in {'true','1','yes'}: continue
        sid=r.get('stable_id','')
        if sid in active: errors.append(f'claims.csv:{index}: duplicate active claim for {sid}')
        active[sid]=r.get('claim_id','')
    return errors


def validate_forbidden_files():
    errors=[]
    for p in ROOT.rglob('*'):
        if not p.is_file() or '.git' in p.parts: continue
        if p.suffix.lower() in FORBIDDEN_EXTS:
            errors.append(f'forbidden binary/material extension: {p.relative_to(ROOT)}')
        if p.stat().st_size > 20 * 1024 * 1024:
            errors.append(f'suspicious file >20MiB: {p.relative_to(ROOT)}')
    return errors


def main():
    errors=[]
    errors += validate_functions(rows('functions.csv'))
    errors += validate_claims(rows('claims.csv'))
    errors += validate_forbidden_files()
    if errors:
        print('\n'.join(errors), file=sys.stderr)
        return 1
    print('validation PASS')
    return 0

if __name__ == '__main__':
    raise SystemExit(main())
