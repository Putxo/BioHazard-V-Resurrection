#!/usr/bin/env python3
"""Verify full, branch-containing source recipes against one local authorized PE."""
from __future__ import annotations
import argparse,csv,hashlib,json,pathlib,re,struct,tempfile
ROOT=pathlib.Path(__file__).resolve().parents[1]
TARGET='323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815'
class Reader:
    def __init__(self,data): self.data=data;self.p=0;self.branches=[];self.branch_sites=[]
    def take(self,n):
        if self.p+n>len(self.data): raise ValueError('Truncated body')
        out=self.data[self.p:self.p+n];self.p+=n;return out
    def exact(self,b):
        if self.take(len(b))!=b: raise ValueError('Unexpected instruction or register')
    def mem(self,opcode,reg,base,sib=False):
        self.exact(bytes([opcode]));m=self.take(1)[0];mode=m>>6
        if (m>>3)&7!=reg or m&7!=(4 if sib else base) or mode==3:raise ValueError('Wrong memory operand')
        if sib:self.exact(bytes([0x80|base]))
        off=0 if mode==0 else int.from_bytes(self.take(1 if mode==1 else 4),'little',signed=True)
        if not 0<=off<=0x100000:raise ValueError('Unsupported displacement')
        return off
    def branch(self,op):
        self.branch_sites.append(self.p);self.exact(bytes([op]));rel=int.from_bytes(self.take(1),'little',signed=True);self.branches.append(self.p+rel)

def decode(kind,code,branch_sites=None):
    r=Reader(code)
    if kind=='store':
        owner=r.mem(0x8b,0,1);test=r.mem(0xf6,0,0);mask=r.take(1)[0];r.branch(0x74)
        out=r.mem(0xc6,0,0);value=r.take(1)[0];target=r.p;r.exact(b'\xc3');p=[owner,test,mask,out,value]
    elif kind=='init':
        r.exact(bytes.fromhex('8b44240485c0'));r.branch(0x74);r.exact(b'\x66')
        off=r.mem(0xc7,0,0);tag=int.from_bytes(r.take(2),'little');r.exact(b'\xc7\x00')
        table=int.from_bytes(r.take(4),'little');target=r.p;r.exact(b'\xc3');p=[off,tag,table]
    elif kind=='bits':
        table=r.mem(0x8b,2,1);r.exact(b'\x85\xd2');r.branch(0x74)
        index=r.mem(0x8b,0,1);r.exact(b'\x85\xc0');r.branch(0x7c)
        count=r.mem(0x3b,0,2);r.branch(0x7d);entries=r.mem(0x8b,0,2,True)
        r.exact(b'\x85\xc0');r.branch(0x74);field=r.mem(0x8b,0,0)
        r.exact(b'\xc1\xe8');shift=r.take(1)[0];r.exact(b'\x83\xe0')
        mask=int.from_bytes(r.take(1),'little',signed=True)&0xffffffff
        r.exact(b'\xc3');target=r.p;r.exact(b'\xb8');fallback=int.from_bytes(r.take(4),'little');r.exact(b'\xc3')
        p=[table,index,count,entries,field,shift,mask,fallback]
    else:raise ValueError('Unknown family')
    if r.p!=len(code) or any(x!=target for x in r.branches):raise ValueError('Incomplete or different control flow')
    if branch_sites is not None:branch_sites.extend(r.branch_sites)
    return p

def encode(kind,p):
    def mem(op,reg,base,off,sib=False):
        mode=0 if off==0 else 1 if off<128 else 2
        return bytes([op,(mode<<6)|(reg<<3)|(4 if sib else base)])+(bytes([0x80|base]) if sib else b'')+(b'' if mode==0 else off.to_bytes(1 if mode==1 else 4,'little'))
    if kind=='store':
        owner,test,mask,out,value=p;write=mem(0xc6,0,0,out)+bytes([value])
        return mem(0x8b,0,1,owner)+mem(0xf6,0,0,test)+bytes([mask,0x74,len(write)])+write+b'\xc3'
    if kind=='init':
        off,tag,table=p;write=b'\x66'+mem(0xc7,0,0,off)+tag.to_bytes(2,'little')+b'\xc7\x00'+table.to_bytes(4,'little')
        return bytes.fromhex('8b44240485c0')+bytes([0x74,len(write)])+write+b'\xc3'
    if kind=='bits':
        table,index,count,entries,field,shift,mask,fallback=p
        parts=[mem(0x8b,2,1,table),b'\x85\xd2',0x74,mem(0x8b,0,1,index),b'\x85\xc0',0x7c,mem(0x3b,0,2,count),0x7d,mem(0x8b,0,2,entries,True),b'\x85\xc0',0x74,mem(0x8b,0,0,field),bytes([0xc1,0xe8,shift,0x83,0xe0,mask&255,0xc3])]
        out=bytearray();branches=[]
        for x in parts:
            if isinstance(x,int):branches.append(len(out));out.extend([x,0])
            else:out.extend(x)
        for at in branches:out[at+1]=len(out)-at-2
        return bytes(out)+b'\xb8'+fallback.to_bytes(4,'little')+b'\xc3'
    raise ValueError('Unknown family')

def get_rows():
    with (ROOT/'records.csv').open(newline='') as f:rows=list(csv.DictReader(f))
    inc='// Exact attached target: named complete bodies and recovered operands.\n';seen=set()
    for r in rows:
        va=int(r['symbol'][4:],16);p=[int(x) for x in r['parameters'].split(';')];b=encode(r['kind'],p)
        if va in seen or va%16 or va<0x410000 or 0xbb9000<=va<0xbb9100:raise ValueError('Duplicate or reserved address')
        seen.add(va)
        if decode(r['kind'],b)!=p or len(b)!=int(r['size']) or hashlib.sha256(b).hexdigest()!=r['sha256']:raise ValueError('Original range hash or operands differ')
        inc+='RE5_G_'+r['kind'].upper()+'('+r['symbol']+', '+', '.join(f'0x{x:08X}U' for x in p)+')\n'
    if len(rows)!=146 or (ROOT/'include/records.inc').read_text()!=inc:raise ValueError('Incomplete source declarations')
    repo=ROOT.parent.parent
    for name in ('leaf','scalar'):
        path=repo/f'decomp/{name}323d1aab/generated/catalog.json'
        if path.exists() and seen&{x['va'] for x in json.loads(path.read_text())['records']}:raise ValueError('Existing leaf/scalar overlap')
    for name,column in [('database/functions.csv','va'),('decomp/accessor323d1aab/records.csv','symbol')]:
        path=repo/name
        if path.exists():
            with path.open(newline='') as f:
                other={int(r[column][4:],16) if column=='symbol' else int(r[column],0) for r in csv.DictReader(f) if r.get(column)}
            if seen&other:raise ValueError('Already recovered address')
    return rows

def read_target(path):
    data=pathlib.Path(path).read_bytes()
    if len(data)!=19977216 or hashlib.sha256(data).hexdigest()!=TARGET:raise ValueError('Wrong input: size/SHA-256')
    pe=struct.unpack_from('<I',data,0x3c)[0]
    if data[:2]!=b'MZ' or data[pe:pe+4]!=b'PE\0\0' or struct.unpack_from('<H',data,pe+4)[0]!=0x14c or struct.unpack_from('<H',data,pe+24)[0]!=0x10b:raise ValueError('Expected I386 PE32')
    return data

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--exe');ap.add_argument('--self-test',action='store_true');a=ap.parse_args();rows=get_rows()
    if a.self_test:
        with tempfile.NamedTemporaryFile() as f:
            f.write(b'not the authorized input');f.flush()
            try:read_target(f.name)
            except ValueError:pass
            else:raise ValueError('Wrong input accepted')
        for r in rows:
            code=bytearray(encode(r['kind'],list(map(int,r['parameters'].split(';')))))
            sites=[];decode(r['kind'],code,sites)
            for at in sites:
                changed=code.copy();changed[at+1]^=0x40
                try:decode(r['kind'],changed)
                except ValueError:pass
                else:raise ValueError('Changed branch target accepted')
            try:decode(r['kind'],code[:-1])
            except ValueError:pass
            else:raise ValueError('Truncation accepted')
    if a.exe:
        data=read_target(a.exe);calls=0
        for r in rows:
            va=int(r['symbol'][4:],16);off=va-0x400c00;b=encode(r['kind'],list(map(int,r['parameters'].split(';'))))
            if data[off:off+len(b)]!=b or data[off-2:off]!=b'\xcc\xcc' or data[off+len(b)]!=0xcc:raise ValueError('Body or boundary mismatch')
            for entry in filter(None,r['direct_calls'].split(';')):
                caller=int(entry,0);o=caller-0x400c00
                if data[o]!=0xe8 or (caller+5+struct.unpack_from('<i',data,o+1)[0])&0xffffffff!=va:raise ValueError('CALL mismatch')
                calls+=1
        print('PASS local target, original boundaries/hashes and',calls,'direct CALL references')
    print('PASS',len(rows),'complete bodies;',sum(int(r['size']) for r in rows),'instruction bytes; no canonical promotion')
if __name__=='__main__':main()
