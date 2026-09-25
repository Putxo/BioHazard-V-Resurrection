#!/usr/bin/env python3
"""Deliberately change recovered semantics and require each wrong program to fail."""
import argparse,json,pathlib,subprocess
from verify import ROOT,records,emit

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--build-dir',required=True);a=ap.parse_args();out=pathlib.Path(a.build_dir).resolve();out.mkdir(parents=True,exist_ok=True);emit(out,records());src=(ROOT/'src/transfers.cpp').read_text()
    variants={
        'flush_subnormal_bits':src.replace('store<4>(self,d,bits);','store<4>(self,d,(bits&0x7f800000U)==0U?0U:bits);',1),
        'omit_dirty_flags':src.replace('store<4>(self,f,load<4>(self,f)|m);','(void)self;(void)f;(void)m;',1),
        'late_second_pair_read':src.replace('const u32 first=load<W>(self,a),second=load<W>(self,b);\n    store<W>(output,c,first);store<W>(output,d,second);','const u32 first=load<W>(self,a);store<W>(output,c,first);\n    const u32 second=load<W>(self,b);store<W>(output,d,second);',1),
        'widen_word_outputs':src.replace('store<W>(output,c,first);store<W>(output,d,second);','store<4>(output,c,first);store<4>(output,d,second);',1),
        'unsigned_nested_count':src.replace('(count&0x80000000U)==0U && index<count','index<count',1)
    }
    results={}
    for name,text in variants.items():
        if text==src:raise ValueError('Mutation not applied: '+name)
        path=out/(name+'.cpp');path.write_text(text);exe=out/name
        subprocess.run(['g++','-std=c++17','-O2','-I'+str(ROOT/'include'),'-I'+str(out),str(path),str(ROOT/'tests/test_transfers.cpp'),'-o',str(exe)],check=True,timeout=60)
        r=subprocess.run([str(exe),'properties'],capture_output=True,text=True,timeout=30)
        if not ((r.returncode==1 and r.stdout.startswith('FAIL FUN_')) or r.returncode in (-11,-7)):raise ValueError('Wrong implementation was not rejected: '+name)
        results[name]={'returncode':r.returncode,'stdout':r.stdout.strip(),'stderr':r.stderr.strip()};print('REJECTED',name,r.returncode,r.stdout.strip())
    (out/'results.json').write_text(json.dumps(results,indent=2)+'\n')
if __name__=='__main__':main()
