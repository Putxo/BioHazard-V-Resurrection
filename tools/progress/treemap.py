#!/usr/bin/env python3
from __future__ import annotations
import csv, html, json, pathlib

ROOT=pathlib.Path(__file__).resolve().parents[2]
DB=ROOT/'database'/'functions.csv'; SVG=ROOT/'reports'/'progress-treemap.svg'; DATA=ROOT/'reports'/'functions.json'
W,H=1200,650
COLORS={'MATCHED':'#166534','VERIFIED':'#22c55e','REFINED':'#86efac','CONVERTED':'#2dd4bf','FAST_PASS_VALIDATED':'#bef264','FAST_PASS':'#facc15','BLOCKED':'#ef4444','DISASSEMBLED':'#fb923c','DISCOVERED':'#fdba74','UNSEEN':'#d1d5db'}
def truth(v): return str(v).lower() in {'1','true','yes'}
def visual(r):
    for key,label in [('matched','MATCHED'),('verified','VERIFIED'),('refined','REFINED'),('converted','CONVERTED'),('fast_pass_validated','FAST_PASS_VALIDATED'),('fast_pass','FAST_PASS')]:
        if truth(r.get(key,'')): return label
    return r.get('workflow_status') if r.get('workflow_status') in {'BLOCKED','DISASSEMBLED','DISCOVERED','UNSEEN'} else 'UNSEEN'
def read_rows():
    with DB.open(newline='',encoding='utf-8') as f: return list(csv.DictReader(f))
def split(items,x,y,w,h,vertical=True):
    if not items: return []
    if len(items)==1: return [(items[0],x,y,w,h)]
    weights=[max(1,int(i.get('size') or 1)) for i in items]; total=sum(weights); target=total/2; acc=0; cut=1
    for n,weight in enumerate(weights[:-1],1):
        acc+=weight
        if acc>=target: cut=n; break
    a,b=items[:cut],items[cut:]; wa=sum(max(1,int(i.get('size') or 1)) for i in a)/total
    if vertical:
        w1=w*wa; return split(a,x,y,w1,h,False)+split(b,x+w1,y,w-w1,h,False)
    h1=h*wa; return split(a,x,y,w,h1,True)+split(b,x,y+h1,w,h-h1,True)
def main():
    rows=read_rows(); DATA.write_text(json.dumps(rows,indent=2),encoding='utf-8')
    if not rows:
        SVG.write_text(f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="180"><rect width="100%" height="100%" fill="#e5e7eb"/><text x="50%" y="52%" text-anchor="middle" font-family="sans-serif" font-size="26" fill="#374151">Inventory pending — no function progress claimed</text></svg>\n',encoding='utf-8'); return
    ordered=sorted(rows,key=lambda r:int(r.get('va') or '0',0)); rects=split(ordered,0,0,W,H)
    parts=[f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{H}" viewBox="0 0 {W} {H}">']
    for r,x,y,w,h in rects:
        st=visual(r); title=f"{r.get('stable_id')} | {r.get('refined_name') or r.get('provisional_name')} | VA {r.get('va')} | {r.get('size')} bytes | {st}"
        parts.append(f'<g><title>{html.escape(title)}</title><rect x="{x:.2f}" y="{y:.2f}" width="{max(0,w-1):.2f}" height="{max(0,h-1):.2f}" fill="{COLORS[st]}" stroke="#111827" stroke-width="0.35"/></g>')
    parts.append('</svg>'); SVG.write_text('\n'.join(parts)+'\n',encoding='utf-8')
if __name__=='__main__': main()
