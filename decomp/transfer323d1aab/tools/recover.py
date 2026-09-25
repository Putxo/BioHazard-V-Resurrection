#!/usr/bin/env python3
"""Fresh conservative discovery from the exact local input using GNU objdump."""
import argparse,csv,hashlib,io,pathlib,re,subprocess,tempfile
from model import KINDS,decode,encode
from verify import ROOT,TARGET

def discover(path):
    data=pathlib.Path(path).read_bytes()
    if len(data)!=19977216 or hashlib.sha256(data).hexdigest()!=TARGET:raise ValueError('Wrong executable')
    rx=re.compile(r'^\s*([0-9a-f]+):\s+((?:[0-9a-f]{2} )+)[ \t]*(.*?)\s*$')
    body=[];active=False;pad=0;selected=[];calls=[]
    with tempfile.TemporaryFile(mode='w+t') as err:
        proc=subprocess.Popen(['objdump','-d','-j','.text','-Mintel','--insn-width=16',str(path)],stdout=subprocess.PIPE,stderr=err,text=True)
        for line in proc.stdout:
            m=rx.match(line)
            if not m:continue
            va=int(m[1],16);code=bytes.fromhex(m[2]);asm=m[3].strip()
            if code[:1]==b'\xe8' and len(code)==5 and asm.startswith('call'):calls.append((va,(va+5+int.from_bytes(code[1:],'little',signed=True))&0xffffffff))
            if asm=='int3':
                if active and body and body[-1][2].startswith('ret'):
                    whole=b''.join(x[1] for x in body)
                    for kind in KINDS:
                        try:p=decode(kind,whole)
                        except (ValueError,IndexError):continue
                        if encode(kind,p)==whole:selected.append([f'FUN_{body[0][0]:08X}',kind,';'.join(map(str,p)),len(whole),hashlib.sha256(whole).hexdigest()])
                        break
                body=[];active=False;pad+=1;continue
            if pad>=2 and va%16==0 and va>=0x410000:body=[];active=True
            pad=0
            if active:
                if body and va!=body[-1][0]+len(body[-1][1]):body=[];active=False;continue
                body.append((va,code,asm))
                if len(body)>80:body=[];active=False
        if proc.wait()!=0:err.seek(0);raise RuntimeError(err.read())
    out=io.StringIO();writer=csv.writer(out,lineterminator='\n');writer.writerow(['symbol','kind','parameters','size','sha256']);writer.writerows(selected)
    starts={int(r[0][4:],16) for r in selected}
    refs='call_va,target_va\n'+''.join(f'0x{a:08X},0x{b:08X}\n' for a,b in calls if b in starts)
    return out.getvalue(),refs

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--exe',required=True);ap.add_argument('--check',action='store_true');ap.add_argument('--out');a=ap.parse_args()
    if not a.check and not a.out:ap.error('Choose --check or --out')
    text,refs=discover(a.exe)
    if a.check and (text!=(ROOT/'records.csv').read_text() or refs!=(ROOT/'call-sites.csv').read_text()):raise ValueError('Fresh discovery differs')
    if a.out:
        out=pathlib.Path(a.out);out.mkdir(parents=True,exist_ok=False);(out/'records.csv').write_text(text);(out/'call-sites.csv').write_text(refs)
    print('PASS fresh discovery:',len(list(csv.DictReader(text.splitlines()))),'complete supported bodies')
if __name__=='__main__':main()
