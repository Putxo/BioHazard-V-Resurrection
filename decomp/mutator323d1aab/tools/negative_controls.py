#!/usr/bin/env python3
"""Require deliberately wrong implementations to fail the source fixtures."""
import argparse,json,pathlib,subprocess
from verify import ROOT,emit,records

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--build-dir',required=True);ap.add_argument('--compiler',default='g++');a=ap.parse_args()
    out=pathlib.Path(a.build_dir).resolve();out.mkdir(parents=True,exist_ok=True);emit(out,records());src=(ROOT/'src/mutations.cpp').read_text()
    variants={
        'missing_dirty_flags':src.replace('store<4>(self,offset,load(self,offset)|bits);','(void)self;(void)offset;(void)bits;',1),
        'widened_byte_word_store':src.replace('store<w>(self,d,value);','store<4>(self,d,value);'),
        'pair_read_ahead':src.replace('const u32 first=load(source,0);store<4>(self,a,first); \\\n    const u32 second=load(source,4);const u32 saved=load(self,s);','const u32 first=load(source,0),second=load(source,4);store<4>(self,a,first); \\\n    const u32 saved=load(self,s);',1),
        'dual_cached_pair':src.replace('const u32 third=load(source,0);','const u32 third=first;').replace('const u32 fourth=load(source,4);','const u32 fourth=second;'),
        'shift_lost_mask':src.replace('difference&=k;store<4>(self,d,load(self,d)^difference);','store<4>(self,d,load(self,d)^difference);',1)
    }
    results={}
    for name,text in variants.items():
        if text==src:raise ValueError('Mutation was not applied: '+name)
        path=out/(name+'.cpp');path.write_text(text);exe=out/name
        subprocess.run([a.compiler,'-std=c++17','-O2','-I'+str(ROOT/'include'),'-I'+str(out),str(path),str(ROOT/'tests/test_mutations.cpp'),'-o',str(exe)],check=True,timeout=60)
        r=subprocess.run([str(exe),'properties'],capture_output=True,text=True,timeout=30)
        if r.returncode!=1 or not r.stdout.startswith('FAIL FUN_'):raise ValueError('Expected assertion failure was not observed: '+name)
        results[name]={'returncode':r.returncode,'stdout':r.stdout.strip(),'stderr':r.stderr.strip()}
        print('REJECTED',name,r.stdout.strip())
    (out/'results.json').write_text(json.dumps(results,indent=2)+'\n')
if __name__=='__main__':main()
