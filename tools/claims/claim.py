#!/usr/bin/env python3
from __future__ import annotations

import argparse
import csv
import datetime as dt
import pathlib
import uuid

ROOT = pathlib.Path(__file__).resolve().parents[2]
PATH = ROOT / 'database' / 'claims.csv'
FIELDS = ['claim_id','stable_id','owner','branch','task','claimed_at','expires_at','pr','active']


def load_rows():
    with PATH.open(newline='', encoding='utf-8') as f:
        return list(csv.DictReader(f))


def is_active(value: str) -> bool:
    return value.lower() in {'1','true','yes'}


def can_claim(rows, stable_id: str) -> bool:
    return not any(r.get('stable_id') == stable_id and is_active(r.get('active','')) for r in rows)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('stable_id')
    ap.add_argument('owner')
    ap.add_argument('--branch', required=True)
    ap.add_argument('--task', default='')
    ap.add_argument('--expires-at', default='')
    args = ap.parse_args()
    rows = load_rows()
    if not can_claim(rows, args.stable_id):
        raise SystemExit(f'claim rejected: {args.stable_id} already has an active owner')
    rows.append({
        'claim_id': str(uuid.uuid4()), 'stable_id': args.stable_id, 'owner': args.owner,
        'branch': args.branch, 'task': args.task,
        'claimed_at': dt.datetime.now(dt.timezone.utc).isoformat(),
        'expires_at': args.expires_at, 'pr': '', 'active': 'true'
    })
    with PATH.open('w', newline='', encoding='utf-8') as f:
        w = csv.DictWriter(f, fieldnames=FIELDS); w.writeheader(); w.writerows(rows)
    print(f'claimed {args.stable_id} for {args.owner}')

if __name__ == '__main__':
    main()
