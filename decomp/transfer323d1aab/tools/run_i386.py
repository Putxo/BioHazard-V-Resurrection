#!/usr/bin/env python3
"""Execute recovered and original-hash instruction fixtures without loading the PE."""
import argparse,pathlib,subprocess
from verify import ROOT,records,reference

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--compiler',default='g++');ap.add_argument('--build-dir',required=True);ap.add_argument('--build-only',action='store_true');a=ap.parse_args()
    out=pathlib.Path(a.build_dir).resolve();out.mkdir(parents=True,exist_ok=True);reference(out,records());exe=out/'transfer-i386-tests'
    cmd=[a.compiler,'-m32','-std=c++17','-O2','-ffreestanding','-fno-builtin','-fno-exceptions','-fno-rtti','-fno-stack-protector','-fno-pic','-fno-pie','-fno-unwind-tables','-fno-asynchronous-unwind-tables','-nostdlib','-static','-no-pie','-DRE5_FREESTANDING_I386','-DRE5_TEST_REFERENCE','-I'+str(ROOT/'include'),'-I'+str(out),str(ROOT/'src/transfers.cpp'),str(ROOT/'tests/test_transfers.cpp'),str(out/'reference.S'),'-o',str(exe)]
    subprocess.run(cmd,check=True,timeout=90);data=exe.read_bytes()
    if data[:5]!=b'\x7fELF\x01' or data[18:20]!=b'\x03\x00':raise ValueError('Not ELF32 I386')
    if a.build_only:print('BUILT ELF32 I386; not executed');return
    try:r=subprocess.run([str(exe)],check=False,text=True,capture_output=True,timeout=60)
    except OSError as e:raise RuntimeError('Native I386 execution unavailable; compilation is not a runtime PASS') from e
    print(r.stdout,end='');print(r.stderr,end='');r.check_returncode()
    for label in ('i386-source','i386-hash-checked-reference'):
        if f'PASS {label} 200318 checks; 201 bodies' not in r.stdout:raise ValueError('Missing full native fixture result')
if __name__=='__main__':main()
