#!/usr/bin/env python3
from __future__ import annotations
import argparse,csv,hashlib,json,pathlib,re,struct,subprocess
ROOT=pathlib.Path(__file__).resolve().parents[2];LEAF=ROOT/'decomp/leaf323d1aab';SEL=ROOT/'decomp/evidence/canonical_3000_selected.csv'
TARGET='323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815';UNION='a7fc3ad913d379f488b8de0a097a6cad290c41fe3cee921f39e3512dee3a4a25'
def req(x,m):
    if not x:raise ValueError(m)
def load():
    s=list(csv.DictReader(SEL.open()));req(len(s)==1000 and len({r['stable_id'] for r in s})==1000,'need 1000 unique selections');catalog_list=json.loads((LEAF/'generated/catalog.json').read_text())['records'];cat={r['symbol']:r for r in catalog_list};idx={r['symbol']:i for i,r in enumerate(catalog_list)};out=[]
    for row in s:
        e=dict(cat[row['stable_id']]);req(int(row['va'])==e['va'],'selection VA mismatch');e['source_file']='decomp/leaf323d1aab/generated/src/leaves_%03d.cpp'%(idx[e['symbol']]//256);req((ROOT/e['source_file']).is_file(),'missing leaf source');out.append(e)
    return out
def registry(sel):
    rows=list(csv.DictReader((ROOT/'database/functions.csv').open()));by={r['stable_id']:r for r in rows};claims=list(csv.DictReader((ROOT/'database/claims.csv').open()));req(len(rows)==3000 and len(by)==3000,'registry must be exactly 3000 unique rows')
    for e in sel:
        r=by.get(e['symbol']);req(r and int(r['va'],0)==e['va'] and int(r['size'])==e['size'],'registry mismatch '+e['symbol']);req(r['source_file']==e['source_file'] and r['fast_pass']=='true','source/tier mismatch '+e['symbol']);req(sum(c['stable_id']==e['symbol'] for c in claims)==1,'claim mismatch '+e['symbol'])
    print('PASS registry: 3000 / 79782; 1000 existing leaf implementations bound once each')
def verify_exe(exe,candidates,sel):
    data=pathlib.Path(exe).read_bytes();req(len(data)==19977216 and hashlib.sha256(data).hexdigest()==TARGET,'wrong EXE');rs=list(csv.DictReader(open(candidates)));req(len(rs)==79782 and len({int(r['va']) for r in rs})==79782,'wrong candidate universe');vas=[int(r['va']) for r in rs];req(hashlib.sha256(b''.join(struct.pack('<I',v) for v in vas)).hexdigest()==UNION,'frozen union changed');lines={v:i+2 for i,v in enumerate(vas)}
    calls={e['va']:[] for e in sel};targets=set(calls);p=subprocess.Popen(['objdump','-d','-j','.text','-Mintel',exe],stdout=subprocess.PIPE,text=True)
    for raw in p.stdout:
        parts=raw.split('\t')
        if len(parts)<3:continue
        try:va=int(parts[0].strip().rstrip(':'),16)
        except ValueError:continue
        ins=parts[-1].split()
        if len(ins)>=2 and ins[0]=='call' and ins[1].startswith('0x'):
            try:t=int(ins[1],16)
            except ValueError:continue
            if t in targets:calls[t].append(va)
    req(p.wait()==0,'objdump failed')
    for e in sel:
        req(e['va'] in lines,'membership changed');off=e['va']-0x400c00;body=data[off:off+e['size']];req(hashlib.sha256(body).hexdigest()==e['body_sha256'],'body mismatch');req(data[off-2:off]==b'\xcc\xcc' and data[off+e['size']:off+e['size']+2]==b'\xcc\xcc','boundary mismatch');req(calls[e['va']]==e['direct_call_sites'],'CALL mismatch')
    print('PASS exact local EXE: 1000 bodies /',sum(e['size'] for e in sel),'bytes / reproduced 79,782-address membership')
def coff_symbols(path):
    d=path.read_bytes();machine,ns,_,ptr,count,opt,_=struct.unpack_from('<HHIIIHH',d);req(machine==0x14c and opt==0,'not I386 COFF');secs={}
    for i in range(ns):
        b=20+40*i;size,raw,rel=struct.unpack_from('<III',d,b+16);nr=struct.unpack_from('<H',d,b+32)[0];secs[i+1]=(d[raw:raw+size],[struct.unpack_from('<I',d,rel+10*j)[0] for j in range(nr)])
    strings=ptr+count*18;out={};i=0
    while i<count:
        o=ptr+18*i;name=d[o:o+8]
        if name[:4]==b'\0'*4:
            at=strings+struct.unpack_from('<I',name,4)[0];name=d[at:d.index(b'\0',at)]
        name=name.rstrip(b'\0').decode('ascii',errors='replace');value,sec,typ,storage,aux=struct.unpack_from('<IhHBB',d,o+8);m=re.search(r'FUN_([0-9A-F]{8})',name)
        if m and sec>0 and (typ&0x20) and storage==2:
            va=int(m.group(1),16);blob,rels=secs[sec];out[va]=(blob[value:],[x-value for x in rels if x>=value])
        i+=1+aux
    return out
def coff(sel,compiler,build):
    build=pathlib.Path(build);build.mkdir(parents=True,exist_ok=True);defs={};flags=['--target=i686-pc-windows-msvc','-std=c++17','-O2','-ffreestanding','-fno-exceptions','-fno-rtti','-Wall','-Wextra','-Wpedantic','-Werror','-I'+str(LEAF/'generated/include')]
    for src in sorted({ROOT/e['source_file'] for e in sel}):
        obj=build/(src.stem+'.obj');subprocess.run([compiler,*flags,'-c',str(src),'-o',str(obj)],check=True);defs.update(coff_symbols(obj))
    exact=0;byt=0
    for e in sel:
        code,rels=defs[e['va']];n=e['size'];ok=not any(0<=x<n for x in rels) and hashlib.sha256(code[:n]).hexdigest()==e['body_sha256'];exact+=ok;byt+=n if ok else 0
    print(json.dumps({'selected_definitions':1000,'exact_body_matches':exact,'exact_instruction_bytes':byt,'game_linked':False}));req(exact==1000,'not all selected I386 bodies are byte-identical')
def main():
    p=argparse.ArgumentParser();p.add_argument('--registry',action='store_true');p.add_argument('--exe');p.add_argument('--candidates');p.add_argument('--coff',action='store_true');p.add_argument('--compiler',default='clang++');p.add_argument('--build-dir');a=p.parse_args();sel=load()
    if a.registry:registry(sel)
    if a.exe:req(a.candidates,'--candidates required');verify_exe(a.exe,a.candidates,sel)
    if a.coff:req(a.build_dir,'--build-dir required');coff(sel,a.compiler,a.build_dir)
    print('PASS canonical-3000 checks; no historical-CSV-byte or whole-game claim')
if __name__=='__main__':main()
