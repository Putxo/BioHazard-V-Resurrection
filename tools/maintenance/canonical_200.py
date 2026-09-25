#!/usr/bin/env python3
"""Scoped registration, CI promotion and incremental publication for 156 -> 200.

Never edits an unrelated row, invents a candidate, changes the denominator,
force-pushes, or operates directly on main/integration. GitHub writes are left
to the explicit caller. Existing implementations are registered, not duplicated.
"""
from __future__ import annotations
import argparse,csv,hashlib,io,json,pathlib,re,subprocess,sys
ROOT=pathlib.Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'tools/verification'))
from verify_canonical_200 import read_manifest,registry,require
DB=('database/functions.csv','database/claims.csv','database/history.csv')
NEW_FILES=(
 'decomp/evidence/canonical_200.json','decomp/evidence/canonical_200.md',
 'decomp/evidence/canonical_200_local_validation.json','decomp/evidence/canonical_200_promotion.json',
 'tests/unit/test_canonical_200_leaves.cpp','tests/unit/canonical_200_leaves.inc',
 'cmake/canonical_200.cmake','tools/maintenance/canonical_200.py',
 'tools/verification/verify_canonical_200.py','.github/workflows/verify-canonical-200.yml')
BATCH_FILES=(
 '.github/workflows/verify-batch-00408b50.yml','cmake/recovery_batch_00408b50.cmake',
 'decomp/evidence/batch_00408b50.json','decomp/evidence/batch_00408b50.md',
 'decomp/evidence/batch_00408b50_instructions.md','include/re5/recovered/batch_00408b50.hpp',
 'src/recovered/batch_00408b50.cpp','tests/unit/test_batch_00408b50.cpp',
 'tools/maintenance/batch_00408b50.py','tools/verification/batch_00408b50_bridge.cpp',
 'tools/verification/reproduce_v6_for_batch_00408b50.py','tools/verification/test_batch_00408b50_tools.py',
 'tools/verification/verify_batch_00408b50.py')

def records(data):return list(csv.DictReader(io.StringIO(data)))
def text(name):return (ROOT/name).read_text()
def git_file(ref,name):return subprocess.check_output(['git','show',f'{ref}:{name}'],cwd=ROOT)
def append(name,rows):
    path=ROOT/name;old=path.read_bytes();fields=next(csv.reader(io.StringIO(old.decode())))
    out=io.StringIO(newline='');writer=csv.DictWriter(out,fieldnames=fields,lineterminator='\n')
    for row in rows:writer.writerow({f:row.get(f,'') for f in fields})
    path.write_bytes(old+(b'' if old.endswith(b'\n') else b'\n')+out.getvalue().encode())
def rewrite(name,replacements):
    path=ROOT/name;lines=path.read_text().splitlines(keepends=True);reader=csv.DictReader(iter(lines));fields=reader.fieldnames;end=reader.line_num
    result=''.join(lines[:end]);seen=set()
    for row in reader:
        newend=reader.line_num;sid=row['stable_id']
        if sid in replacements:
            out=io.StringIO(newline='');w=csv.DictWriter(out,fieldnames=fields,lineterminator='\n');w.writerow(replacements[sid]);result+=out.getvalue();seen.add(sid)
        else:result+=''.join(lines[end:newend])
        end=newend
    require(seen==set(replacements),'Missing target for promotion');path.write_text(result)

def main():
    ap=argparse.ArgumentParser();ap.add_argument('stage',choices=['register','promote','publish']);ap.add_argument('--source-ref');ap.add_argument('--recovery-pr',type=int);ap.add_argument('--recovery-head');ap.add_argument('--recovery-merge');args=ap.parse_args()
    branch=subprocess.check_output(['git','branch','--show-current'],cwd=ROOT,text=True).strip()
    require(branch.startswith(('decomp/canonical-200','promote/canonical-200','release/canonical-200')),'Use the exclusive canonical-200 branch')
    frozen=(ROOT/'config/inventory_freeze.json').read_bytes()
    m=json.loads((ROOT/'decomp/evidence/canonical_200.json').read_text());new=m['new_registry_entries'];pending=m['pending_publication_entries'];ids={e['stable_id'] for e in new+pending};new_ids={e['stable_id'] for e in new}
    before={name:records(text(name)) for name in DB};before_functions={r['stable_id']:r for r in before[DB[0]]};date=m['claimed_at'][:10]
    if args.stage=='register':
        read_manifest()
        require((ROOT/'decomp/evidence/canonical_200.md').is_file(),'Missing scoped evidence document')
        require(len(before_functions)==170,'Registration base changed: inspect before updating')
        require(not new_ids&set(before_functions),'Existing target function must not be overwritten')
        require(not any(r['stable_id'] in new_ids for r in before[DB[1]]),'Existing target claim must not be overwritten')
        rows=[];claims=[];history=[]
        for e in new:
            sid=e['stable_id'];cid='CANONICAL-200-'+sid[4:]
            rows.append(dict(stable_id=sid,va=f"0x{e['va']:08X}",rva=f"0x{e['rva']:08X}",section='.text',size=str(e['size']),provisional_name=sid,workflow_status='ACTIVE',confidence='HIGH',fast_pass='true',fast_pass_validated='false',converted='false',refined='false',verified='false',matched='false',linked='false',calling_convention='thiscall',return_type='uint32_t',arguments='',this_type='const void*; observed PE32 field layout',module='UNKNOWN',subsystem='FieldAccess',source_file=e['source_file'],owner='coordinator',claim_id=cid,pr='',commit='',test=e['test'],evidence='decomp/evidence/canonical_200.md',notes=f"Unsigned {e['width']*8}-bit read at this+0x{e['displacement']:X}; existing leaf C++ registered after exact-body and frozen-V6 membership revalidation; no duplicate implementation; CI pending"))
            claims.append(dict(claim_id=cid,stable_id=sid,owner='coordinator',branch='decomp/canonical-200',task='Verify frozen V6 membership and register existing tested field reader',claimed_at=m['claimed_at'],expires_at='',pr='',active='true'))
            history.append(dict(stable_id=sid,date=date,old_status='UNSEEN',new_status='FAST_PASS',commit='',owner='coordinator',reason='Canonical registration of existing C++ after exact-input body and reproduced frozen-V6 set checks; no new duplicate source body or whole-game claim'))
        append(DB[0],rows);append(DB[1],claims);append(DB[2],history)
        cmake=ROOT/'CMakeLists.txt'
        if 'include(cmake/canonical_200.cmake)' not in cmake.read_text():
            with cmake.open('a') as handle:
                handle.write('\n# Focused tests for the thirty existing leaves entering the 200-entry registry.\ninclude(cmake/canonical_200.cmake)\n')
    elif args.stage=='promote':
        read_manifest();require(len(before_functions)==200,'Promotion base changed')
        require(args.recovery_pr and all(re.fullmatch('[0-9a-f]{40}',x or '') for x in (args.recovery_head,args.recovery_merge)),'Actual recovery PR/head/merge required')
        require(ids<=before_functions.keys(),'Incomplete recovery scope')
        selected={sid:dict(before_functions[sid]) for sid in ids}
        require(all(r['fast_pass_validated']=='false' for r in selected.values()),'Promotion already performed or inconsistent')
        for r in selected.values():
            r.update(fast_pass_validated='true',pr='#'+str(args.recovery_pr),commit=args.recovery_merge)
            r['notes']=r['notes'].replace('CI pending',f"Recovery PR #{args.recovery_pr} full CI PASS on {args.recovery_head}; FAST_PASS_VALIDATED only")
        rewrite(DB[0],selected)
        append(DB[2],[dict(stable_id=sid,date=date,old_status='FAST_PASS',new_status='FAST_PASS_VALIDATED',commit=args.recovery_merge,owner='coordinator',reason=f"Recovery PR #{args.recovery_pr} full CI and scoped source/guard checks PASS on {args.recovery_head}; no CONVERTED/VERIFIED/MATCHED/LINKED promotion") for sid in sorted(ids)])
        receipt=dict(recovery_pr=args.recovery_pr,recovery_head=args.recovery_head,recovery_merge=args.recovery_merge,validated_entries=sorted(ids),tier='FAST_PASS_VALIDATED',denominator=79782,game_linked=False)
        (ROOT/'decomp/evidence/canonical_200_promotion.json').write_text(json.dumps(receipt,indent=2)+'\n')
    else:
        require(args.source_ref and re.fullmatch('[0-9a-f]{40}',args.source_ref),'Use an immutable validated integration commit')
        require(len(before_functions)==156,'Main advanced: inspect and rebase before publishing')
        require(not ids&set(before_functions),'Do not overwrite an already published target')
        require(not any(r['stable_id'] in ids for r in before[DB[1]]),'Do not overwrite an existing main claim')
        for name in BATCH_FILES+NEW_FILES:
            content=git_file(args.source_ref,name);path=ROOT/name
            require(not path.exists() or path.read_bytes()==content,'Refusing to replace a different existing source: '+name)
            path.parent.mkdir(parents=True,exist_ok=True);path.write_bytes(content)
        source={name:records(git_file(args.source_ref,name).decode()) for name in DB}
        selected=[r for r in source[DB[0]] if r['stable_id'] in ids]
        require(len(selected)==44 and all(r['fast_pass_validated']=='true' for r in selected),'Source batch is not fully CI-promoted')
        for name in DB:append(name,[r for r in source[name] if r['stable_id'] in ids])
        cmake=ROOT/'CMakeLists.txt'
        with cmake.open('a') as handle:
            for include in ('include(cmake/recovery_batch_00408b50.cmake)','include(cmake/canonical_200.cmake)'):
                require(include not in cmake.read_text(),'Duplicate CMake publication attachment');handle.write('\n'+include+'\n')
        read_manifest()
    after={r['stable_id']:r for r in records(text(DB[0]))}
    untouched=set(before_functions)-(ids if args.stage=='promote' else set())
    require(all(before_functions[sid]==after[sid] for sid in untouched),'An unrelated function row changed')
    require(len(after)==200,'Registry is not exactly 200')
    require((ROOT/'config/inventory_freeze.json').read_bytes()==frozen,'Inventory baseline changed')
    subprocess.run([sys.executable,'tools/reports/report.py']+(['--publish-readme'] if args.stage=='publish' else []),cwd=ROOT,check=True)
    subprocess.run([sys.executable,'tools/progress/treemap.py'],cwd=ROOT,check=True)
    subprocess.run([sys.executable,'tools/validate/validate.py'],cwd=ROOT,check=True)
    registry(m)
    print(f'PASS {args.stage}: official registry 200 / 79782; existing unrelated rows and frozen denominator preserved')
if __name__=='__main__':main()
