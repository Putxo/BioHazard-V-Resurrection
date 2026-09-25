#!/usr/bin/env python3
"""Repeat conservative discovery from only the exact local EXE; never execute it."""
import argparse,csv,hashlib,pathlib,re,subprocess,tempfile
from model import KINDS,decode,encode
from verify import ROOT,TARGET

def discover(path):
    data=pathlib.Path(path).read_bytes()
    if len(data)!=19977216 or hashlib.sha256(data).hexdigest()!=TARGET:raise ValueError('Wrong input')
    rx=re.compile(r'^\s*([0-9a-f]+):\s+((?:[0-9a-f]{2} )+)[ \t]*(.*?)\s*$')
    selected=[];calls=[];body=[];padding=0;active=False
    with tempfile.TemporaryFile(mode='w+t') as err:
        proc=subprocess.Popen(['objdump','-d','-j','.text','-Mintel','--insn-width=16',str(path)],stdout=subprocess.PIPE,stderr=err,text=True)
        for line in proc.stdout:
            m=rx.match(line)
            if not m:continue
            va=int(m[1],16);code=bytes.fromhex(m[2]);asm=m[3].strip()
            if code[:1]==b'\xe8' and len(code)==5 and asm.startswith('call'):
                calls.append((va,(va+5+int.from_bytes(code[1:],'little',signed=True))&0xffffffff))
            if asm=='int3':
                if active and body and body[-1][2].startswith('ret'):
                    whole=b''.join(x[1] for x in body)
                    for kind in KINDS:
                        try:p=decode(kind,whole)
                        except (ValueError,IndexError):continue
                        if encode(kind,p)==whole:selected.append((body[0][0],kind,p))
                        break
                active=False;body=[];padding+=1;continue
            if padding>=2 and va%16==0 and va>=0x410000:body=[];active=True
            padding=0
            if active:
                if body and va!=body[-1][0]+len(body[-1][1]):active=False;body=[];continue
                body.append((va,code,asm))
                if len(body)>100:active=False;body=[]
        if proc.wait()!=0:
            err.seek(0);raise RuntimeError(err.read())
    starts={a for a,_,_ in selected}
    text='symbol,kind,parameters\n'+''.join(f'FUN_{a:08X},{k},'+ ';'.join(map(str,p))+'\n' for a,k,p in selected)
    refs='call_va,target_va\n'+''.join(f'0x{a:08X},0x{b:08X}\n' for a,b in calls if b in starts)
    return text,refs

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--exe',required=True);ap.add_argument('--check',action='store_true');ap.add_argument('--out');a=ap.parse_args()
    if not a.check and not a.out:ap.error('Select --check or --out')
    text,refs=discover(a.exe)
    if a.check and (text!=(ROOT/'records.csv').read_text() or refs!=(ROOT/'call-sites.csv').read_text()):raise ValueError('Discovery differs from recorded set')
    if a.out:
        out=pathlib.Path(a.out);out.mkdir(parents=True,exist_ok=False)
        (out/'records.csv').write_text(text);(out/'call-sites.csv').write_text(refs)
    print('PASS fresh discovery:',len(list(csv.DictReader(text.splitlines()))),'complete supported bodies')
if __name__=='__main__':main()
