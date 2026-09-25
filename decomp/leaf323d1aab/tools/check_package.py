#!/usr/bin/env python3
"""Validate source/catalogue consistency without the proprietary EXE."""
from pathlib import Path
import hashlib, json, re, sys
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tools'))
from recover_leaves import SHA256, SIZE, expression

def main():
    c=json.loads((ROOT/'generated/catalog.json').read_text())
    assert c['input_sha256']==SHA256 and c['input_size']==SIZE
    rows=c['records']; assert len(rows)==c['recovered_leaf_ranges']==7171
    assert sum(r['size'] for r in rows)==c['recovered_instruction_bytes']==42745
    assert c['canonical_denominator']==79782 and not c['denominator_changed']
    assert not c['canonical_registry_imported'] and not c['linked_game']
    assert len({r['va'] for r in rows})==len(rows)
    files=list((ROOT/'generated/src').glob('leaves_*.cpp'))
    source='\n'.join(p.read_text() for p in files)
    definitions=re.findall(r'u32 RE5_LEAF_CC (FUN_[0-9A-F]{8})\(const void\* self\) noexcept \{(.*?)\n\}',source,re.S)
    assert len(definitions)==len(rows)
    definitions=dict(definitions); assert len(definitions)==len(rows)
    for r in rows:
        assert r['symbol']==f'FUN_{r["va"]:08X}' and r['rva']==r['va']-0x400000
        assert r['va']>=0x410000 and not 0xBB9000<=r['va']<0xBB9100
        assert r['va']%16==0 and r['size'] in (3,4,5,6,7,8)
        assert 'return '+expression(r)+';' in definitions[r['symbol']]
        assert len(r['body_sha256'])==64
        if 'llvm_i386' in r:
            assert sum(i['size'] for i in r['llvm_i386'])==r['size']
            assert len(r['llvm_i386'])==2 and r['llvm_i386'][-1]['assembly'].startswith('ret')
    forbidden={'.exe','.dll','.obj','.o','.a','.so','.lib','.bin','.pdb','.pak','.arc'}
    unexpected=[]
    for p in ROOT.rglob('*'):
        if not p.is_file() or any(x in {'__pycache__','.git'} or x.startswith('build') for x in p.relative_to(ROOT).parts):
            continue
        if p.suffix.lower() in forbidden: unexpected.append(str(p.relative_to(ROOT)))
    assert not unexpected, unexpected
    print(json.dumps({'test':'package_integrity','routines':len(rows),'source_files':len(files), 'status':'PASS'}))
if __name__=='__main__': main()
