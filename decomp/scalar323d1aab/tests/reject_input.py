import pathlib,subprocess,sys,tempfile
root=pathlib.Path(__file__).resolve().parents[1]
with tempfile.TemporaryDirectory() as t:
 p=pathlib.Path(t)/'not-the-authorized-input';p.write_bytes(b'MZ'+b'\0'*100)
 for f in [p,p.parent/'missing']:
  r=subprocess.run([sys.executable,str(root/'tools/verify_target.py'),'--exe',str(f)],capture_output=True)
  assert r.returncode!=0
print('PASS: missing and wrong input rejected')
