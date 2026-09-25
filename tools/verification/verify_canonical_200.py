#!/usr/bin/env python3
"""Revalidate the 30 existing leaves and 14 wrappers entering main's 200-entry registry.

The proprietary PE stays local. CI uses previously recorded hashes and isolated
instruction fixtures, never the original PE or game. Reconstructed V6 membership
is explicitly distinguished from access to historical CSV bytes.
"""
from __future__ import annotations
import argparse, collections, csv, hashlib, json, pathlib, struct, subprocess, sys, tempfile
ROOT=pathlib.Path(__file__).resolve().parents[2]
MANIFEST=ROOT/'decomp/evidence/canonical_200.json'
TARGET='323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815'
UNION='a7fc3ad913d379f488b8de0a097a6cad290c41fe3cee921f39e3512dee3a4a25'
LEAF=ROOT/'decomp/leaf323d1aab'
sys.path.insert(0,str(LEAF/'tools'))
from restore_sources import body
from compare_coff import symbols

def require(ok,message):
    if not ok: raise ValueError(message)

def read_manifest():
    m=json.loads(MANIFEST.read_text());new=m['new_registry_entries'];pending=m['pending_publication_entries']
    require(m['target_sha256']==TARGET and m['target_size']==19977216,'Wrong target')
    require(m['canonical_denominator']==79782 and m['target_registered']==200,'Changed target count')
    require(len(new)==30 and len(pending)==14,'Changed registration scope')
    require(len({e['stable_id'] for e in new+pending})==44,'Duplicate registration address')
    previous=json.loads((ROOT/'decomp/evidence/batch_00408b50.json').read_text())
    require(m['inventory']['sorted_union_u32le_sha256']==UNION==previous['candidate_reconstruction']['sorted_union_u32le_sha256'],'Different reconstructed V6 universe')
    freeze=json.loads((ROOT/'config/inventory_freeze.json').read_text())
    require(freeze['function_candidate_denominator']==79782 and freeze['target_sha256']==TARGET,'Freeze changed')
    catalog={r['symbol']:r for r in json.loads((LEAF/'generated/catalog.json').read_text())['records']}
    for name,expected in m['source_sha256'].items():
        require(hashlib.sha256((ROOT/name).read_bytes()).hexdigest()==expected,'Existing source changed: '+name)
    for e in new:
        r=catalog[e['stable_id']]
        require(r['family'] in ('load8','load16','load32'),'Not a field reader')
        for field in ('va','rva','size','body_sha256','family','displacement','width'):
            require(r[field]==e[field],'Catalog mismatch: '+field)
        require(hashlib.sha256(body(r)).hexdigest()==e['body_sha256'],'Original recipe mismatch')
        require(e['direct_call_sites'] and e['union_csv_line']>=2,'Missing membership evidence')
        require((' '+e['stable_id']+'(') in (ROOT/e['source_file']).read_text(),'Missing existing definition')
    expected='// Audit-selected existing implementations; no new or duplicated bodies.\n'+''.join(f"CANONICAL_LEAF({e['stable_id']}, {e['displacement']}U, {e['width']}U)\n" for e in new)
    require((ROOT/'tests/unit/canonical_200_leaves.inc').read_text()==expected,'Changed test selection')
    return m,catalog

def registry(m):
    rows=list(csv.DictReader((ROOT/'database/functions.csv').open()));by_id={r['stable_id']:r for r in rows}
    claims=list(csv.DictReader((ROOT/'database/claims.csv').open()))
    require(len(rows)>=200 and len(by_id)==len(rows),'Registry has not reached 200 unique entries')
    for e in m['new_registry_entries']+m['pending_publication_entries']:
        r=by_id[e['stable_id']]
        require(int(r['va'],0)==e['va'] and int(r['size'],0)==e['size'],'Registry address/size mismatch')
        require(r['fast_pass']=='true' and (ROOT/r['source_file']).is_file(),'Missing implementation')
        require((ROOT/r['test']).is_file() and (ROOT/r['evidence']).is_file(),'Missing tests/evidence')
        require(sum(c['stable_id']==e['stable_id'] for c in claims)==1,'Missing/duplicate target claim')
    for e in m['new_registry_entries']:
        r=by_id[e['stable_id']]
        require(r['source_file']==e['source_file'] and r['test']==e['test'],'Wrong source binding')
    print(f'PASS registry {len(rows)} / 79782; 44 scoped implementation/test/evidence bindings')

def verify_exe(path,m,catalog,csv_output):
    data=pathlib.Path(path).read_bytes()
    require(len(data)==19977216 and hashlib.sha256(data).hexdigest()==TARGET,'Wrong local EXE')
    # Reuse the exact PE layout validator already committed for the wrapper batch.
    from verify_batch_00408b50 import pe_sections
    sections=pe_sections(data);_,base,length,offset,_=next(s for s in sections if s[0]=='.text')
    raw=data[offset:offset+length]
    padding={base+i for i in range(16,length,16) if raw[i-1] in (0xcc,0x90) and raw[i-2] in (0xcc,0x90) and raw[i] not in (0xcc,0x90)}
    pointers=set()
    for off,size in [(0,1024)]+[(s[3],s[4]) for s in sections if s[0]!='.text']:
        run=[]
        for word, in struct.iter_unpack('<I',data[off:off+size]):
            if base<=word<base+length: run.append(word)
            else:
                if len(run)>=2:pointers.update(run)
                run=[]
        if len(run)>=2:pointers.update(run)
    calls=collections.defaultdict(list)
    with subprocess.Popen(['objdump','-d','-j','.text','-Mintel',str(path)],stdout=subprocess.PIPE,text=True) as proc:
        for line in proc.stdout:
            parts=line.split('\t')
            if len(parts)<3:continue
            try:va=int(parts[0].strip().rstrip(':'),16)
            except ValueError:continue
            ins=parts[-1].split()
            if not base<=va<base+length or len(ins)<2 or ins[0]!='call' or not ins[1].startswith('0x'):continue
            try:target=int(ins[1],16)
            except ValueError:continue
            if base<=target<base+length:calls[target].append(va)
        require(proc.wait()==0,'objdump failed')
    direct=set(calls)|{0x012570A9};roots=sorted(direct|padding|pointers)
    require((len(calls),len(direct),len(pointers),len(padding),len(roots))==(19194,19195,40912,69076,79782),'Frozen V6 aggregates differ')
    require(hashlib.sha256(b''.join(struct.pack('<I',v) for v in roots)).hexdigest()==UNION,'Frozen V6 set commitment differs')
    ordered_direct=sorted(direct)
    for va,line in {0x408620:137,0x408630:138,0x408640:139,0x4086a0:140,0x408700:141,0x408af0:142,0x408b10:143,0x408b30:144}.items():
        require(ordered_direct.index(va)+2==line,'Historical direct-group anchor differs')
    out=pathlib.Path(csv_output);out.parent.mkdir(parents=True,exist_ok=True)
    # This CSV is a reproducible new serialization, not the historical CSV file.
    with out.open('w',newline='') as handle:
        w=csv.writer(handle,lineterminator='\n');w.writerow(['rva','va']);w.writerows((v-0x400000,v) for v in roots)
    with out.open(newline='') as handle:literal={int(r['va']):i+2 for i,r in enumerate(csv.DictReader(handle))}
    for e in m['new_registry_entries']+m['pending_publication_entries']:
        require(literal.get(e['va'])==e['union_csv_line'],'Literal reproduced CSV membership differs')
        off=e['va']-0x400c00;actual=data[off:off+e['size']]
        require(hashlib.sha256(actual).hexdigest()==e['body_sha256'],'Original body changed')
        require(calls.get(e['va'],[])==e['direct_call_sites'],'Direct calls differ')
        if e in m['new_registry_entries']:
            require(actual==body(catalog[e['stable_id']]),'Recovered leaf recipe differs')
            require(data[off-2:off]==b'\xcc\xcc' and data[off+e['size']:off+e['size']+2]==b'\xcc\xcc','Leaf boundaries differ')
    print('PASS exact local EXE; full frozen V6 set; reproduced CSV second-column membership; all 44 original bodies/call sites')

def compile_check(m,catalog,compiler,out,native,build_only):
    out=pathlib.Path(out).resolve();out.mkdir(parents=True,exist_ok=True)
    require(ROOT!=out and ROOT not in out.parents,'Build outside the source checkout')
    sources=sorted({e['source_file'] for e in m['new_registry_entries']})
    include='-I'+str(LEAF/'generated/include')
    if native:
        text='.text\n'
        for e in m['new_registry_entries']:
            n='original_'+e['stable_id'];code=body(catalog[e['stable_id']])
            text+=f'.balign 16\n.global {n}\n.type {n},@function\n{n}:\n.byte '+','.join(map(str,code))+f'\n.size {n},.-{n}\n'
        ref=out/'reference.S';ref.write_text(text+'.section .note.GNU-stack,"",@progbits\n')
        executable=out/'canonical-200-i386'
        flags=['-m32','-std=c++17','-O2','-ffreestanding','-fno-builtin','-fno-exceptions','-fno-rtti','-fno-stack-protector','-fno-pic','-fno-pie','-fno-unwind-tables','-fno-asynchronous-unwind-tables','-nostdlib','-static','-no-pie','-DRE5_CANONICAL_I386','-DRE5_CANONICAL_REFERENCE',include]
        subprocess.run([compiler,*flags,*(str(ROOT/s) for s in sources),str(ROOT/'tests/unit/test_canonical_200_leaves.cpp'),str(ref),'-o',str(executable)],check=True,timeout=120)
        b=executable.read_bytes();require(b[:5]==b'\x7fELF\x01' and b[18:20]==b'\x03\x00','Not I386 ELF32')
        if build_only: print('BUILT I386 test; not executed');return
        try:r=subprocess.run([str(executable)],check=True,text=True,capture_output=True,timeout=60)
        except OSError as exc:raise RuntimeError('Native I386 execution unavailable; compilation is not runtime validation') from exc
        print(r.stdout,end='')
        for label in ('i386-source','i386-hash-checked-reference'):
            require(f'PASS {label} 491761 checks; 30 bodies' in r.stdout,'Incomplete native fixture result')
    else:
        definitions={}
        for s in sources:
            obj=out/(pathlib.Path(s).stem+'.obj')
            subprocess.run([compiler,'--target=i686-pc-windows-msvc','-std=c++17','-O2','-ffreestanding','-fno-exceptions','-fno-rtti',include,'-c',str(ROOT/s),'-o',str(obj)],check=True,timeout=60)
            definitions.update(symbols(obj))
        exact=[]
        for e in m['new_registry_entries']:
            code,relocations=definitions[e['va']];n=e['size']
            if not any(0<=r<n for r in relocations) and hashlib.sha256(code[:n]).hexdigest()==e['body_sha256']:exact.append(e['stable_id'])
        result=dict(selected_definitions=30,exact_body_matches=len(exact),matched=exact,game_linked=False,canonical_matched_tier_changed=False)
        (out/'coff-report.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--registry',action='store_true');ap.add_argument('--exe');ap.add_argument('--csv-output');ap.add_argument('--native',action='store_true');ap.add_argument('--coff',action='store_true');ap.add_argument('--compiler',default='g++');ap.add_argument('--build-dir');ap.add_argument('--build-only',action='store_true');a=ap.parse_args();m,catalog=read_manifest()
    if a.registry:registry(m)
    if a.exe:
        require(a.csv_output,'Supply --csv-output for the explicitly reproduced local inventory')
        verify_exe(a.exe,m,catalog,a.csv_output)
    if a.native or a.coff:
        require(a.build_dir and not(a.native and a.coff),'Choose one build check and an external build directory')
        compile_check(m,catalog,a.compiler,a.build_dir,a.native,a.build_only)
    print('PASS scoped source/manifest checks; no whole-game or historical-CSV-byte equivalence claim')
if __name__=='__main__':main()
