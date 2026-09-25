#!/usr/bin/env python3
"""Check the exact local input/range hashes, then compare safe native fragments.

Does not launch the PE or contact a service. The native optional test executes
compatible leaf fragments in x86-64 mode, NOT an IA32 process or the game.
"""
from pathlib import Path
import argparse, hashlib, json, subprocess
from recover_leaves import read_target, ROOT

p=argparse.ArgumentParser(description=__doc__)
p.add_argument('--exe',type=Path,required=True)
p.add_argument('--build',type=Path,required=True)
p.add_argument('--report',type=Path)
a=p.parse_args()
b,_,_,_=read_target(a.exe)
c=json.loads((ROOT/'generated/catalog.json').read_text())
for r in c['records']:
    code=b[r['file_offset']:r['file_offset']+r['size']]
    if hashlib.sha256(code).hexdigest()!=r['body_sha256']:
        raise SystemExit(f'Instruction range mismatch: {r["symbol"]}')
runner=a.build/'re5_leaf_native'
if not runner.is_file():
    raise SystemExit('Native comparison runner not built on this platform; no execution result is claimed')
result=subprocess.run([str(runner.resolve()),str(a.exe.resolve())],capture_output=True,text=True,check=True)
report=json.loads(result.stdout.strip())
report.update(input_sha256=hashlib.sha256(b).hexdigest(),input_size=len(b),
              range_hashes_verified=len(c['records']),original_pe_launched=False,
              full_ia32_abi_verified=False,proprietary_input_uploaded=False)
print(json.dumps(report,indent=2))
if a.report:
    a.report.write_text(json.dumps(report,indent=2)+'\n')
