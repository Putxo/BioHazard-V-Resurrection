#!/usr/bin/env python3
"""Read-only audit against the ACTUAL canonical candidate CSV, not a replacement.

The VA must literally occur in decimal in the second column. Missing addresses
are never removed from the 79,782 denominator. This tool does not reserve claims,
change tiers, rewrite metadata, push changes, or substitute for branch/PR checks.
"""
from pathlib import Path
import argparse,csv,json
ROOT=Path(__file__).resolve().parents[1]
DENOMINATOR=79782

def read_candidates(path: Path, expected_count: int=DENOMINATOR):
    result=set()
    with path.open(newline='',encoding='utf-8-sig') as handle:
        for number,row in enumerate(csv.reader(handle),1):
            if not row: continue
            if number==1 and row[0].strip().lower() in ('rva','rva_decimal'):
                if len(row)<2 or row[1].strip().lower() not in ('va','va_decimal'):
                    raise ValueError('Unrecognized canonical CSV header')
                continue
            if len(row)<2 or not row[0].strip().isdecimal() or not row[1].strip().isdecimal():
                raise ValueError(f'Expected literal decimal RVA,VA at CSV line {number}')
            rva,va=int(row[0]),int(row[1])
            if va-rva!=0x400000 or va in result:
                raise ValueError(f'Invalid/duplicate target address at CSV line {number}')
            result.add(va)
    if len(result)!=expected_count:
        raise ValueError(f'Expected {expected_count} unique canonical rows, found {len(result)}')
    return result

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--candidates',type=Path,required=True)
    p.add_argument('--functions',type=Path,help='Optional canonical functions.csv snapshot')
    p.add_argument('--report',type=Path)
    a=p.parse_args()
    universe=read_candidates(a.candidates)
    records=json.loads((ROOT/'generated/catalog.json').read_text())['records']
    existing=set()
    if a.functions:
        with a.functions.open(newline='',encoding='utf-8-sig') as f:
            for row in csv.DictReader(f):
                value=row['va'].strip()
                existing.add(int(value,16) if value.lower().startswith('0x') else int(value,10))
    present=[r['symbol'] for r in records if r['va'] in universe]
    absent=[r['symbol'] for r in records if r['va'] not in universe]
    overlap=[r['symbol'] for r in records if r['va'] in existing]
    report=dict(canonical_denominator=DENOMINATOR,denominator_changed=False,
                canonical_csv_rows=len(universe),literal_membership_present=len(present),
                literal_membership_absent=len(absent),already_registered=len(overlap),
                absent_symbols=absent,registered_overlap=overlap,
                claim_branch_pr_preflight_still_required=True,metadata_written=False)
    text=json.dumps(report,indent=2)+'\n'
    print(text,end='')
    if a.report: a.report.write_text(text,encoding='utf-8')
if __name__=='__main__': main()
