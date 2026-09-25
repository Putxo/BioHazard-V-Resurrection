#!/usr/bin/env python3
"""Append/promote/publish only the independently verified 00408B50..00408E00 batch.

Run on an exclusive work branch. Never force-push, change inventory denominators,
or copy wholesale integration databases into main. The CI caller does the push.
"""
from __future__ import annotations
import argparse,csv,datetime,hashlib,io,json,pathlib,subprocess
ROOT=pathlib.Path(__file__).resolve().parents[2]
MANIFEST=ROOT/'decomp/evidence/batch_00408b50.json'
BRANCH='decomp/00408b50-00408e00-exact'

def check(ok,text):
    if not ok: raise SystemExit(text)

def records(text): return list(csv.DictReader(io.StringIO(text)))
def file_text(name): return (ROOT/name).read_text(encoding='utf-8')
def git_text(ref,name):
    return subprocess.check_output(['git','show',f'{ref}:{name}'],cwd=ROOT,text=True)
def append(name,items):
    path=ROOT/name; original=path.read_bytes(); fields=next(csv.reader(io.StringIO(original.decode())))
    out=io.StringIO(newline=''); writer=csv.DictWriter(out,fieldnames=fields,lineterminator='\n')
    for row in items: writer.writerow({key:row.get(key,'') for key in fields})
    path.write_bytes(original+(b'' if original.endswith(b'\n') else b'\n')+out.getvalue().encode())
def rewrite_targets(name,replacements):
    path=ROOT/name; text=path.read_text(); lines=text.splitlines(keepends=True)
    reader=csv.DictReader(iter(lines)); fields=reader.fieldnames; end=reader.line_num
    output=''.join(lines[:end]); seen=set()
    for row in reader:
        new_end=reader.line_num; sid=row['stable_id']
        if sid in replacements:
            tmp=io.StringIO(newline=''); writer=csv.DictWriter(tmp,fieldnames=fields,lineterminator='\n')
            writer.writerow(replacements[sid]); output+=tmp.getvalue(); seen.add(sid)
        else: output+=''.join(lines[end:new_end])
        end=new_end
    check(seen==set(replacements),'Promotion IDs are missing')
    path.write_text(output)

def main():
    ap=argparse.ArgumentParser(); ap.add_argument('stage',choices=['recover','promote','publish'])
    ap.add_argument('--source-ref'); ap.add_argument('--recovery-pr'); ap.add_argument('--recovery-head'); ap.add_argument('--recovery-merge')
    args=ap.parse_args(); branch=subprocess.check_output(['git','branch','--show-current'],cwd=ROOT,text=True).strip()
    check(branch not in ('main','integration',''),'Use an exclusive named work branch')
    frozen=(ROOT/'config/inventory_freeze.json').read_bytes(); freeze=json.loads(frozen)
    check(freeze['target_sha256']=='323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815' and freeze['function_candidate_denominator']==79782,'Target/freeze mismatch')
    manifest=json.loads(MANIFEST.read_text()); entries=manifest['entries']; ids={e['stable_id'] for e in entries}
    check(len(ids)==14 and sum(e['size'] for e in entries)==632,'Unexpected recovery scope')
    for name,digest in manifest['source_sha256'].items():
        check(hashlib.sha256((ROOT/name).read_bytes()).hexdigest()==digest,'Source hash mismatch: '+name)
    before={name:records(file_text(name)) for name in ('database/functions.csv','database/claims.csv','database/history.csv')}
    old_functions={r['stable_id']:r for r in before['database/functions.csv']}
    now=datetime.datetime.now(datetime.timezone.utc); today=now.date().isoformat(); timestamp=now.isoformat(timespec='seconds').replace('+00:00','Z')
    if args.stage in ('recover','publish'):
        check(not(ids&set(old_functions)),'A target already exists; inspect remote work instead of replacing it')
        check(not any(r['stable_id'] in ids for r in before['database/claims.csv']),'An existing target claim must not be replaced')
        if args.stage=='recover':
            new=[]; claims=[]; history=[]
            for e in entries:
                sid=e['stable_id']; va=int(e['va'],0); claim='EXACT-BATCH-'+sid[4:]
                row=dict(stable_id=sid,va=e['va'],rva=f'0x{va-0x400000:08X}',section='.text',size=str(e['size']),
                         provisional_name=sid,refined_name='',workflow_status='ACTIVE',confidence='HIGH',
                         fast_pass='true',fast_pass_validated='false',converted='false',refined='false',verified='false',matched='false',linked='false',
                         calling_convention=e['original_calling_convention'],return_type=e['return_type'],arguments=e['arguments'],
                         this_type='PE32 address token; native ABI pending',module='UNKNOWN',subsystem=e['subsystem'],
                         source_file='src/recovered/batch_00408b50.cpp',owner='coordinator',claim_id=claim,pr='',commit='',
                         test='tests/unit/test_batch_00408b50.cpp',evidence='decomp/evidence/batch_00408b50.md',
                         notes=e['summary']+'; exact-attachment target-memory C++ adapter; bounded differential tests PASS; dependencies and native ABI unresolved; CI pending')
                new.append(row)
                claims.append(dict(claim_id=claim,stable_id=sid,owner='coordinator',branch=BRANCH,task='exact attachment batch recovery with complete body/guard evidence',claimed_at=timestamp,expires_at='',pr='',active='true'))
                history.append(dict(stable_id=sid,date=today,old_status='UNSEEN',new_status='FAST_PASS',commit='',owner='coordinator',reason='Exact local attachment body reconstructed; target-memory adapter and bounded differential validation; no native ABI or dependency verification claim'))
        else:
            check(bool(args.source_ref),'Publication requires the exact promoted integration ref')
            new=[r for r in records(git_text(args.source_ref,'database/functions.csv')) if r['stable_id'] in ids]
            claims=[r for r in records(git_text(args.source_ref,'database/claims.csv')) if r['stable_id'] in ids]
            history=[r for r in records(git_text(args.source_ref,'database/history.csv')) if r['stable_id'] in ids]
            check(len(new)==14 and len(claims)==14 and all(r['fast_pass_validated']=='true' for r in new),'Source ref is not the complete validated batch')
        append('database/functions.csv',new); append('database/claims.csv',claims); append('database/history.csv',history)
        cmake=ROOT/'CMakeLists.txt'; line='include(cmake/recovery_batch_00408b50.cmake)'
        check(line not in cmake.read_text(),'Duplicate CMake attachment')
        with cmake.open('a') as handle: handle.write('\n'+line+'\n')
    else:
        check(all((args.recovery_pr,args.recovery_head,args.recovery_merge)),'Promotion requires actual recovery CI/merge evidence')
        selected={sid:dict(old_functions[sid]) for sid in ids if sid in old_functions}
        check(len(selected)==14 and all(r['fast_pass_validated']=='false' for r in selected.values()),'Do not repeat a promotion')
        for row in selected.values():
            row.update(fast_pass_validated='true',pr='#'+args.recovery_pr,commit=args.recovery_merge)
            row['notes']=row['notes'].replace('CI pending',f'PR #{args.recovery_pr} standard CI 5/5 and batch GCC/Clang PASS on {args.recovery_head}')
        rewrite_targets('database/functions.csv',selected)
        append('database/history.csv',[dict(stable_id=sid,date=today,old_status='FAST_PASS',new_status='FAST_PASS_VALIDATED',commit=args.recovery_merge,owner='coordinator',reason=f'Recovery PR #{args.recovery_pr} actual standard CI 5/5 and batch matrix PASS on {args.recovery_head}; native ABI/dependency/matching/linking tiers remain false') for sid in sorted(ids)])
    after={r['stable_id']:r for r in records(file_text('database/functions.csv'))}
    check(all(after[sid]==row for sid,row in old_functions.items() if sid not in ids),'Unrelated function row was modified')
    check((ROOT/'config/inventory_freeze.json').read_bytes()==frozen,'Inventory freeze changed')
    subprocess.run(['python','tools/reports/report.py']+(['--publish-readme'] if args.stage=='publish' else []),cwd=ROOT,check=True)
    subprocess.run(['python','tools/progress/treemap.py'],cwd=ROOT,check=True)
    subprocess.run(['python','tools/validate/validate.py'],cwd=ROOT,check=True)
    print('PASS:',args.stage,'only 14 batch IDs; denominator 79,782 unchanged')
if __name__=='__main__': main()
