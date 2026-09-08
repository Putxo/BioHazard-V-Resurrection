#!/usr/bin/env python3
from __future__ import annotations
import csv, pathlib, re

ROOT=pathlib.Path(__file__).resolve().parents[2]
DB=ROOT/'database'/'functions.csv'
README=ROOT/'README.md'
REPORT=ROOT/'reports'/'progress.md'
TIERS=['fast_pass','converted','refined','verified','matched','linked']
LABELS={'fast_pass':'FAST PASS','converted':'CONVERTED','refined':'REFINED','verified':'VERIFIED','matched':'MATCHED','linked':'LINKED'}

def truth(v): return str(v).lower() in {'1','true','yes'}
def rows():
    with DB.open(newline='',encoding='utf-8') as f: return list(csv.DictReader(f))
def bar(done,total,width=30):
    if total<=0: return '░'*width+'  pending'
    n=round(width*done/total); pct=100.0*done/total
    return '█'*n+'░'*(width-n)+f'  {pct:5.1f}%  {done:,} / {total:,}'
def metrics(rs):
    total=len(rs); total_bytes=sum(int(r['size'],0) for r in rs if r.get('size'))
    discovered=sum(1 for r in rs if r.get('workflow_status')!='UNSEEN')
    discovered_bytes=sum(int(r['size'],0) for r in rs if r.get('size') and r.get('workflow_status')!='UNSEEN')
    out={'total':total,'total_bytes':total_bytes,'functions_discovered':discovered,'bytes_discovered':discovered_bytes}
    for t in TIERS: out[t]=sum(1 for r in rs if truth(r.get(t,'')))
    return out
def progress_block(m):
    functions = bar(m['functions_discovered'], m['total'])
    code_size = bar(m['bytes_discovered'], m['total_bytes']) if m['total_bytes'] else '░'*30+'  pending'
    return f'```\nFunctions   {functions}\nCode size   {code_size}\n```'
def tiers_block(m):
    lines=['```']
    for t in TIERS: lines.append(f"{LABELS[t]:10} {bar(m[t],m['total'])}")
    lines.append('```'); return '\n'.join(lines)
def replace_marked(text,start,end,new):
    pattern=re.compile(re.escape(start)+'.*?'+re.escape(end),re.S)
    payload=start+'\n'+new+'\n'+end
    return pattern.sub(payload,text) if pattern.search(text) else text+'\n\n'+payload+'\n'
def main():
    rs=rows(); m=metrics(rs); p=progress_block(m); t=tiers_block(m)
    readme=README.read_text(encoding='utf-8')
    readme=replace_marked(readme,'<!-- progress:start -->','<!-- progress:end -->',p)
    readme=replace_marked(readme,'<!-- tiers:start -->','<!-- tiers:end -->',t)
    README.write_text(readme,encoding='utf-8')
    REPORT.write_text('# Progress\n\n'+p+'\n\n'+t+'\n',encoding='utf-8')
    print(f"functions={m['total']} bytes={m['total_bytes']}")
if __name__=='__main__': main()
