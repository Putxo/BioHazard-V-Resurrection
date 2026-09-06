#!/usr/bin/env python3
from __future__ import annotations

import argparse
import csv
import pathlib

ROOT = pathlib.Path(__file__).resolve().parents[2]
PATH = ROOT / 'database' / 'claims.csv'
FIELDS = ['claim_id','stable_id','owner','branch','task','claimed_at','expires_at','pr','active']


def main():
    ap = argparse.ArgumentParser(); ap.add_argument('stable_id'); ap.add_argument('--owner', default=''); args = ap.parse_args()
    with PATH.open(newline='', encoding='utf-8') as f: rows=list(csv.DictReader(f))
    changed=0
    for r in rows:
        if r.get('stable_id') == args.stable_id and r.get('active','').lower() in {'1','true','yes'}:
            if args.owner and r.get('owner') != args.owner: continue
            r['active']='false'; changed += 1
    if changed == 0: raise SystemExit(f'no matching active claim for {args.stable_id}')
    with PATH.open('w', newline='', encoding='utf-8') as f:
        w=csv.DictWriter(f, fieldnames=FIELDS); w.writeheader(); w.writerows(rows)
    print(f'released {changed} claim(s) for {args.stable_id}')

if __name__ == '__main__': main()
