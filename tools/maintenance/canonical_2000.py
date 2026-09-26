#!/usr/bin/env python3
from __future__ import annotations
import argparse,csv,io,json,pathlib,re,subprocess,sys
ROOT=pathlib.Path(__file__).resolve().parents[2]
SEL=ROOT/'decomp/evidence/canonical_2000_selected.csv'
CAT=ROOT/'decomp/leaf323d1aab/generated/catalog.json'
DB=('database/functions.csv','database/claims.csv','database/history.csv')

def req(x,m):
    if not x: raise ValueError(m)
def rows(n): return list(csv.DictReader((ROOT/n).open(newline='')))
def selection():
    ids=[r['stable_id'] for r in csv.DictReader(SEL.open())]; req(len(ids)==1000 and len(set(ids))==1000,'need 1000 unique ids')
    cat={r['symbol']:r for r in json.loads(CAT.read_text())['records']}; req(set(ids)<=cat.keys(),'selection missing from leaf catalog')
    return [cat[x] for x in ids]
def append(n,add):
    p=ROOT/n
    with p.open(newline='') as f: fields=csv.DictReader(f).fieldnames
    with p.open('a',newline='') as f:
        w=csv.DictWriter(f,fieldnames=fields,lineterminator='\n')
        for r in add:w.writerow({k:r.get(k,'') for k in fields})
def rewrite(changes):
    p=ROOT/DB[0]
    with p.open(newline='') as f:r=csv.DictReader(f);fields=r.fieldnames;data=list(r)
    out=io.StringIO(newline='');w=csv.DictWriter(out,fieldnames=fields,lineterminator='\n');w.writeheader();found=set()
    for row in data:
        if row['stable_id'] in changes: row=changes[row['stable_id']];found.add(row['stable_id'])
        w.writerow(row)
    req(found==set(changes),'missing promotion target');p.write_text(out.getvalue())
def refresh(publish=False):
    subprocess.run([sys.executable,'tools/reports/report.py']+(['--publish-readme'] if publish else []),cwd=ROOT,check=True)
    subprocess.run([sys.executable,'tools/progress/treemap.py'],cwd=ROOT,check=True)
    subprocess.run([sys.executable,'tools/validate/validate.py'],cwd=ROOT,check=True)
    subprocess.run([sys.executable,'tools/verification/verify_canonical_2000.py','--registry'],cwd=ROOT,check=True)
def note(e):
    f=e['family']
    if f=='constant': desc=f"Returns original constant 0x{e['immediate']:08X}"
    elif f.startswith('load'): desc=f"Unsigned {e['width']*8}-bit load at this+0x{e['displacement']:X}"
    elif f=='address': desc=f"Returns 32-bit receiver-relative address this+0x{e['displacement']:X}"
    else: desc='Existing exact-target leaf body'
    return desc+'; frozen-V6 member and locally byte-identical I386 body; CI pending'
def register():
    before=rows(DB[0]);req(len(before)==1000,'expected 1000-entry base');existing={r['stable_id'] for r in before};sel=selection();req(not existing&{e['symbol'] for e in sel},'target already registered')
    catalog_list=json.loads(CAT.read_text())['records']; index={r['symbol']:i for i,r in enumerate(catalog_list)}
    fs=[];cs=[];hs=[]
    for e in sel:
        sid=e['symbol'];cid='CANONICAL-2000-'+sid[4:];va=e['va'];src='decomp/leaf323d1aab/generated/src/leaves_%03d.cpp'%(index[sid]//256)
        fs.append(dict(stable_id=sid,va=f'0x{va:08X}',rva=f"0x{e['rva']:08X}",section='.text',size=str(e['size']),provisional_name=sid,workflow_status='ACTIVE',confidence='HIGH',fast_pass='true',fast_pass_validated='false',converted='false',refined='false',verified='false',matched='false',linked='false',calling_convention='thiscall',return_type='uint32_t',arguments='',this_type='const void*; observed PE32 leaf contract',module='UNKNOWN',subsystem='LeafRecovery',source_file=src,owner='coordinator',claim_id=cid,pr='',commit='',test='decomp/leaf323d1aab/tests/properties.cpp',evidence='decomp/evidence/canonical_2000.md',notes=note(e)))
        cs.append(dict(claim_id=cid,stable_id=sid,owner='coordinator',branch='decomp/canonical-2000',task='Register existing exact-target leaf after frozen V6 membership and byte-identical I386 revalidation',claimed_at='2026-09-26T00:24:00+02:00',expires_at='',pr='',active='true'))
        hs.append(dict(stable_id=sid,date='2026-09-26',old_status='UNSEEN',new_status='FAST_PASS',commit='',owner='coordinator',reason='Register existing C++ leaf after exact-input body, frozen-V6 membership and local I386 byte-match checks; no duplicate body or whole-game claim'))
    append(DB[0],fs);append(DB[1],cs);append(DB[2],hs)
    cm=ROOT/'CMakeLists.txt';inc='include(cmake/canonical_2000.cmake)';req(inc not in cm.read_text(),'already attached');cm.write_text(cm.read_text()+'\n# Official 2,000-entry registry validation for existing recovered leaves.\n'+inc+'\n')
    refresh(False)
def promote(pr,head,merge):
    data=rows(DB[0]);req(len(data)==2000,'expected 2000-entry registration');ids={e['symbol'] for e in selection()};by={r['stable_id']:r for r in data};changes={}
    for sid in ids:
        r=dict(by[sid]);req(r['fast_pass_validated']=='false','already promoted');r.update(fast_pass_validated='true',pr='#'+str(pr),commit=merge);r['notes']=r['notes'].replace('CI pending',f'Recovery PR #{pr} full CI PASS on {head}; FAST_PASS_VALIDATED only');changes[sid]=r
    rewrite(changes);append(DB[2],[dict(stable_id=s,date='2026-09-26',old_status='FAST_PASS',new_status='FAST_PASS_VALIDATED',commit=merge,owner='coordinator',reason=f'Recovery PR #{pr} full CI PASS on {head}; no higher-tier promotion') for s in sorted(ids)])
    receipt={'recovery_pr':pr,'recovery_head':head,'recovery_merge':merge,'validated_count':1000,'tier':'FAST_PASS_VALIDATED','denominator':79782,'game_linked':False};(ROOT/'decomp/evidence/canonical_2000_promotion.json').write_text(json.dumps(receipt,indent=2)+'\n');refresh(False)
def main():
    a=argparse.ArgumentParser();a.add_argument('stage',choices=['register','promote']);a.add_argument('--recovery-pr',type=int);a.add_argument('--recovery-head');a.add_argument('--recovery-merge');x=a.parse_args()
    if x.stage=='register':register()
    else:req(x.recovery_pr and re.fullmatch(r'[0-9a-f]{40}',x.recovery_head or '') and re.fullmatch(r'[0-9a-f]{40}',x.recovery_merge or ''),'actual recovery refs required');promote(x.recovery_pr,x.recovery_head,x.recovery_merge)
if __name__=='__main__':main()
