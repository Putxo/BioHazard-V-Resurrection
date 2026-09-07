#!/usr/bin/env python3
from __future__ import annotations

import argparse
import csv
import json
import pathlib
import re

ROOT = pathlib.Path(__file__).resolve().parents[2]
DB = ROOT / 'database' / 'functions.csv'
FREEZE = ROOT / 'config' / 'inventory_freeze.json'
README = ROOT / 'README.md'
REPORT = ROOT / 'reports' / 'progress.md'
TIERS = ['fast_pass', 'converted', 'refined', 'verified', 'matched', 'linked']
LABELS = {
    'fast_pass': 'FAST PASS',
    'converted': 'CONVERTED',
    'refined': 'REFINED',
    'verified': 'VERIFIED',
    'matched': 'MATCHED',
    'linked': 'LINKED',
}


def truth(value: object) -> bool:
    return str(value).lower() in {'1', 'true', 'yes'}


def rows() -> list[dict[str, str]]:
    with DB.open(newline='', encoding='utf-8') as handle:
        return list(csv.DictReader(handle))


def baseline() -> dict[str, int]:
    if not FREEZE.exists():
        return {'total': 0, 'total_bytes': 0}
    data = json.loads(FREEZE.read_text(encoding='utf-8'))
    return {
        'total': int(data.get('function_candidate_denominator', 0)),
        'total_bytes': int(data.get('estimated_function_bytes', 0)),
    }


def bar(done: int, total: int, width: int = 30) -> str:
    if total <= 0:
        return '░' * width + '  pending'
    filled = round(width * done / total)
    pct = 100.0 * done / total
    return '█' * filled + '░' * (width - filled) + f'  {pct:5.1f}%  {done:,} / {total:,}'


def metrics(records: list[dict[str, str]]) -> dict[str, int]:
    frozen = baseline()
    total = frozen['total'] or len(records)
    total_bytes = frozen['total_bytes'] or sum(int(r['size'], 0) for r in records if r.get('size'))
    discovered = sum(1 for r in records if r.get('workflow_status') != 'UNSEEN')
    discovered_bytes = sum(
        int(r['size'], 0)
        for r in records
        if r.get('size') and r.get('workflow_status') != 'UNSEEN'
    )
    result = {
        'total': total,
        'total_bytes': total_bytes,
        'functions_discovered': discovered,
        'bytes_discovered': discovered_bytes,
    }
    for tier in TIERS:
        result[tier] = sum(1 for r in records if truth(r.get(tier, '')))
    return result


def progress_block(m: dict[str, int]) -> str:
    functions = bar(m['functions_discovered'], m['total'])
    code_size = bar(m['bytes_discovered'], m['total_bytes']) if m['total_bytes'] else '░' * 30 + '  pending'
    return f'```\nFunctions   {functions}\nCode size   {code_size}\n```'


def tiers_block(m: dict[str, int]) -> str:
    lines = ['```']
    for tier in TIERS:
        lines.append(f"{LABELS[tier]:10} {bar(m[tier], m['total'])}")
    lines.append('```')
    return '\n'.join(lines)


def replace_marked(text: str, start: str, end: str, new: str) -> str:
    pattern = re.compile(re.escape(start) + '.*?' + re.escape(end), re.S)
    payload = start + '\n' + new + '\n' + end
    return pattern.sub(payload, text) if pattern.search(text) else text


def publish_public_readme(progress: str, tiers: str) -> None:
    readme = README.read_text(encoding='utf-8')
    if '<!-- progress:start -->' not in readme or '<!-- tiers:start -->' not in readme:
        raise SystemExit(
            'README.md has no public-dashboard markers. '
            'Public README publishing is only valid on main.'
        )
    readme = replace_marked(readme, '<!-- progress:start -->', '<!-- progress:end -->', progress)
    readme = replace_marked(readme, '<!-- tiers:start -->', '<!-- tiers:end -->', tiers)
    README.write_text(readme, encoding='utf-8')


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        '--publish-readme',
        action='store_true',
        help='Update the public dashboard markers in README.md. Use only when preparing/validating main.',
    )
    args = parser.parse_args()

    records = rows()
    m = metrics(records)
    progress = progress_block(m)
    tiers = tiers_block(m)

    REPORT.write_text('# Progress\n\n' + progress + '\n\n' + tiers + '\n', encoding='utf-8')
    if args.publish_readme:
        publish_public_readme(progress, tiers)

    print(
        f"functions={m['total']} bytes={m['total_bytes']} "
        f"publish_readme={str(args.publish_readme).lower()}"
    )


if __name__ == '__main__':
    main()
