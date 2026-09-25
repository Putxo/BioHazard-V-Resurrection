#!/usr/bin/env python3
"""Build/run a freestanding ELF32 source+reference fixture, never the original PE."""
import argparse,pathlib,subprocess,sys
from verify import ROOT,records,emit_reference

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--compiler',default='g++');ap.add_argument('--build-dir',required=True);ap.add_argument('--build-only',action='store_true');args=ap.parse_args()
    out=pathlib.Path(args.build_dir).resolve();out.mkdir(parents=True,exist_ok=True)
    emit_reference(out,records());exe=out/'projection-i386-tests'
    command=[args.compiler,'-m32','-std=c++17','-O2','-ffreestanding','-fno-builtin','-fno-exceptions','-fno-rtti','-fno-stack-protector','-fno-pic','-fno-pie','-fno-unwind-tables','-fno-asynchronous-unwind-tables','-nostdlib','-static','-no-pie','-DRE5_FREESTANDING_I386','-DRE5_TEST_REFERENCE','-I'+str(ROOT/'include'),str(ROOT/'src/projections.cpp'),str(ROOT/'tests/test_projections.cpp'),str(out/'reference.S'),'-o',str(exe)]
    subprocess.run(command,check=True)
    data=exe.read_bytes()
    if data[:5]!=b'\x7fELF\x01' or data[18:20]!=b'\x03\x00':raise ValueError('Not ELF32 I386')
    if args.build_only:print('BUILT ELF32 I386; not executed');return
    try:result=subprocess.run([str(exe)],check=True,text=True,capture_output=True)
    except OSError as e:raise RuntimeError('Native I386 execution is unavailable on this kernel; compilation is not a runtime PASS') from e
    print(result.stdout,end='')
    for label in ['i386-source','i386-hash-checked-reference']:
        if f'PASS {label} 180217 checks; 196 bodies' not in result.stdout:raise ValueError('Missing complete fixture result')
if __name__=='__main__':main()
