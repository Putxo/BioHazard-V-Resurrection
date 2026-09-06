from __future__ import annotations

import csv
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
DATABASE = ROOT / 'database'


def read_csv(name: str) -> list[dict[str, str]]:
    path = DATABASE / name
    with path.open(newline='', encoding='utf-8') as f:
        return list(csv.DictReader(f))


def write_csv(name: str, rows: list[dict[str, object]], fieldnames: list[str]) -> None:
    path = DATABASE / name
    with path.open('w', newline='', encoding='utf-8') as f:
        w = csv.DictWriter(f, fieldnames=fieldnames)
        w.writeheader()
        for row in rows:
            w.writerow(row)


def parse_int(value: str | None) -> int | None:
    if value is None or value == '':
        return None
    return int(value, 0)


def function_rows() -> list[dict[str, str]]:
    return read_csv('functions.csv')


def active_claims() -> list[dict[str, str]]:
    return [r for r in read_csv('claims.csv') if r.get('active','').lower() in {'1','true','yes'}]
