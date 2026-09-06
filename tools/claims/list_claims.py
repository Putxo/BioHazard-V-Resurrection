#!/usr/bin/env python3
import argparse, csv, pathlib
ROOT=pathlib.Path(__file__).resolve().parents[2]
PATH=ROOT/'database'/'claims.csv'
ap=argparse.ArgumentParser(); ap.add_argument('--owner', default=''); ap.add_argument('--all', action='store_true'); args=ap.parse_args()
with PATH.open(newline='', encoding='utf-8') as f: rows=list(csv.DictReader(f))
for r in rows:
    active=r.get('active','').lower() in {'1','true','yes'}
    if not args.all and not active: continue
    if args.owner and r.get('owner') != args.owner: continue
    print(f"{r.get('stable_id')} owner={r.get('owner')} branch={r.get('branch')} active={r.get('active')}")
