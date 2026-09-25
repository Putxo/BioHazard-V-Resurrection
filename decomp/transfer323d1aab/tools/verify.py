#!/usr/bin/env python3
"""Validate original-body hashes and generate named C++ records, never launch the PE."""
import argparse,csv,hashlib,json,pathlib,re,struct,tempfile
from model import KINDS,decode,encode
ROOT=pathlib.Path(__file__).resolve().parents[1]
TARGET='323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815'
MANIFEST='c738cc4500ad8b80c2c63834ba66c9542685b274b3d9483fc732b0698df4b77b'

def records():
    data=(ROOT/'records.csv').read_bytes()
    if hashlib.sha256(data).hexdigest()!=MANIFEST:raise ValueError('Changed original-body manifest')
    rows=list(csv.DictReader(data.decode().splitlines()));seen=set()
    for r in rows:
        if not re.fullmatch(r'FUN_[0-9A-F]{8}',r['symbol']) or r['kind'] not in KINDS:raise ValueError('Invalid record')
        r['va']=int(r['symbol'][4:],16);r['p']=list(map(int,r['parameters'].split(';')));r['size']=int(r['size'])
        if r['va'] in seen or r['va']%16 or r['va']<0x410000:raise ValueError('Invalid start')
        seen.add(r['va']);body=encode(r['kind'],r['p'])
        if len(body)!=r['size'] or hashlib.sha256(body).hexdigest()!=r['sha256'] or decode(r['kind'],body)!=r['p']:raise ValueError('Instruction contract mismatch')
    if len(rows)!=201 or sum(r['size'] for r in rows)!=5470:raise ValueError('Incorrect body set')
    repo=ROOT.parent.parent
    for folder in (repo/'decomp').glob('*323d1aab'):
        if folder==ROOT:continue
        p=folder/'generated/catalog.json'
        if p.exists():other={x['va'] for x in json.loads(p.read_text())['records']}
        elif (folder/'records.csv').exists():other={int(x['symbol'][4:],16) for x in csv.DictReader((folder/'records.csv').read_text().splitlines())}
        else:continue
        if seen&other:raise ValueError('Overlapping recovery: '+folder.name)
    for name in ('functions.csv','claims.csv'):
        p=repo/'database'/name
        if p.exists() and seen&{int(x,16) for x in re.findall(r'FUN_([0-9a-fA-F]{8})',p.read_text())}:raise ValueError('Canonical/claim overlap')
    return rows

def emit(out,rows):
    out=pathlib.Path(out);out.mkdir(parents=True,exist_ok=True)
    (out/'records.inc').write_text('// Generated named definitions from audited original-body records.\n'+''.join(f"RE5_{r['kind']}({r['symbol']}"+''.join(f', 0x{x:08X}U' for x in r['p'])+')\n' for r in rows))

def reference(out,rows):
    emit(out,rows);text='.text\n'
    for r in rows:
        n='original_'+r['symbol'];body=encode(r['kind'],r['p'])
        text+=f'.balign 16\n.global {n}\n.type {n},@function\n{n}:\n.byte '+','.join(map(str,body))+f'\n.size {n},.-{n}\n'
    (pathlib.Path(out)/'reference.S').write_text(text+'.section .note.GNU-stack,"",@progbits\n')

def verify_target(path,rows):
    data=pathlib.Path(path).read_bytes()
    if len(data)!=19977216 or hashlib.sha256(data).hexdigest()!=TARGET:raise ValueError('Wrong executable size/SHA-256')
    pe=struct.unpack_from('<I',data,0x3c)[0];opt=pe+24
    if data[:2]!=b'MZ' or data[pe:pe+4]!=b'PE\0\0' or struct.unpack_from('<H',data,pe+4)[0]!=0x14c or struct.unpack_from('<H',data,opt)[0]!=0x10b or struct.unpack_from('<I',data,opt+28)[0]!=0x400000:raise ValueError('Wrong PE32 mapping')
    sec=opt+struct.unpack_from('<H',data,pe+20)[0];text=None
    for i in range(struct.unpack_from('<H',data,pe+6)[0]):
        o=sec+40*i
        if data[o:o+8].rstrip(b'\0')==b'.text':text=struct.unpack_from('<IIII',data,o+8)
    if text!=(16185403,4096,16185856,1024):raise ValueError('Wrong text mapping')
    for r in rows:
        o=r['va']-0x400c00;body=data[o:o+r['size']]
        if hashlib.sha256(body).hexdigest()!=r['sha256'] or decode(r['kind'],body)!=r['p'] or body!=encode(r['kind'],r['p']) or data[o-2:o]!=b'\xcc\xcc' or data[o+r['size']]!=0xcc:raise ValueError('Original body or boundary mismatch')
    calls=list(csv.DictReader((ROOT/'call-sites.csv').read_text().splitlines()));starts={r['va'] for r in rows}
    for r in calls:
        a=int(r['call_va'],0);b=int(r['target_va'],0);o=a-0x400c00
        if b not in starts or data[o]!=0xe8 or (a+5+struct.unpack_from('<i',data,o+1)[0])&0xffffffff!=b:raise ValueError('Wrong CALL evidence')
    print('PASS exact local input: 201 bodies / 5470 bytes /',len(calls),'CALL sites')

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--exe');ap.add_argument('--emit');ap.add_argument('--self-test',action='store_true');a=ap.parse_args();rows=records()
    if a.self_test:
        for r in rows:
            body=bytearray(encode(r['kind'],r['p']));body[-1]^=255
            try:decode(r['kind'],body)
            except ValueError:pass
            else:raise ValueError('Changed return accepted')
        with tempfile.NamedTemporaryFile() as f:
            f.write(b'wrong');f.flush()
            try:verify_target(f.name,rows)
            except ValueError:pass
            else:raise ValueError('Wrong input accepted')
    if a.exe:verify_target(a.exe,rows)
    if a.emit:emit(a.emit,rows)
    print('PASS 201 original hashes, instruction recipes, parameter contracts and overlap checks; no canonical promotion')
if __name__=='__main__':main()
