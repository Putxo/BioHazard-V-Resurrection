"""Closed instruction grammar for exact-target flag/field mutators."""
import struct
KINDS=('S8','S16','S32','D8','D16','D32','PAIR','DUAL','SHIFT','MASK','COPY')
class Reader:
    def __init__(self,b):self.b=b;self.i=0
    def take(self,n):
        if self.i+n>len(self.b):raise ValueError('Truncated instruction')
        a=self.b[self.i:self.i+n];self.i+=n;return a
    def exact(self,x):
        if self.take(len(x))!=x:raise ValueError('Unexpected opcode/register')
    def mem(self,op,reg,base=1):
        self.exact(bytes(op));m=self.take(1)[0];mode=m>>6
        if mode==3 or (m>>3)&7!=reg or m&7!=base:raise ValueError('Unexpected memory operand')
        d=0 if mode==0 else int.from_bytes(self.take(1 if mode==1 else 4),'little',signed=True)
        if not 0<=d<=65536:raise ValueError('Unsupported displacement')
        return d
    def flags(self):
        op=self.b[self.i]
        if op not in (0x81,0x83):raise ValueError('Expected OR')
        d=self.mem([op],1);mask=int.from_bytes(self.take(1 if op==0x83 else 4),'little',signed=op==0x83)&0xffffffff
        return d,mask
    def mask(self):
        op=self.take(1)[0]
        if op==0x25:return int.from_bytes(self.take(4),'little')
        if op==0x83:self.exact(b'\xe0');return int.from_bytes(self.take(1),'little',signed=True)&0xffffffff
        raise ValueError('Expected AND EAX')

def arg(width):return (b'\x66' if width==2 else b'')+bytes([0x8a if width==1 else 0x8b,0x44,0x24,4])
def st(width):return ([0x66] if width==2 else [])+[0x88 if width==1 else 0x89]

def decode(kind,code):
    r=Reader(code)
    if kind[0] in ('S','D') and kind[1:] in ('8','16','32'):
        w=int(kind[1:])//8;r.exact(arg(w));snap=r.mem([0x8b],2) if kind[0]=='S' else None
        f,m=r.flags();dest=r.mem(st(w),0)
        if snap is not None and r.mem([0x89],2)!=snap:raise ValueError('Different snapshot destination')
        p=[snap,f,m,dest] if snap is not None else [f,m,dest]
    elif kind=='PAIR':
        r.exact(arg(4)+b'\x8b\x10');a=r.mem([0x89],2);r.exact(b'\x8b\x40\x04');s=r.mem([0x8b],2);f,m=r.flags();b=r.mem([0x89],0)
        if r.mem([0x89],2)!=s:raise ValueError('Snapshot mismatch')
        p=[a,b,s,f,m]
    elif kind=='DUAL':
        r.exact(arg(4)+b'\x8b\x10');a=r.mem([0x89],2);r.exact(b'\x8b\x50\x04');f,m=r.flags();b=r.mem([0x89],2);s=r.mem([0x8b],2)
        if r.mem([0x89],2)!=s:raise ValueError('Snapshot mismatch')
        r.exact(b'\x8b\x10');c=r.mem([0x89],2);r.exact(b'\x8b\x40\x04');t=r.mem([0x8b],2);g,n=r.flags();d=r.mem([0x89],0)
        if r.mem([0x89],2)!=t:raise ValueError('Snapshot mismatch')
        p=[a,b,s,f,m,c,d,t,g,n]
    elif kind=='SHIFT':
        r.exact(arg(4));s=r.mem([0x8b],2);f,m=r.flags();r.exact(b'\xc1\xe0');shift=r.take(1)[0]
        if not 0<shift<32:raise ValueError('Unsupported shift')
        d=r.mem([0x33],0)
        if r.mem([0x89],2)!=s:raise ValueError('Snapshot mismatch')
        mask=r.mask()
        if r.mem([0x31],0)!=d:raise ValueError('Different destination')
        p=[s,f,m,d,shift,mask]
    elif kind=='MASK':
        d=r.mem([0x8b],0);r.exact(b'\x33\x44\x24\x04');s=r.mem([0x8b],2);f,m=r.flags();mask=r.mask()
        if r.mem([0x31],0)!=d or r.mem([0x89],2)!=s:raise ValueError('Destination mismatch')
        p=[s,f,m,d,mask]
    elif kind=='COPY':
        r.exact(arg(4)+b'\x8b\x10');s=r.mem([0x8b],0);f,m=r.flags();d=r.mem([0x89],2)
        if r.mem([0x89],0)!=s:raise ValueError('Snapshot mismatch')
        p=[s,f,m,d]
    else:raise ValueError('Unknown family')
    r.exact(b'\xc2\x04\x00')
    if r.i!=len(code):raise ValueError('Trailing instructions')
    return p

def encode(kind,p):
    def mem(op,reg,d,base=1):
        mode=0 if d==0 else 1 if d<128 else 2
        return bytes(op)+bytes([(mode<<6)|(reg<<3)|base])+(b'' if mode==0 else d.to_bytes(1 if mode==1 else 4,'little'))
    def flag(d,m):
        small=m<128 or m>=0xffffff80
        return mem([0x83 if small else 0x81],1,d)+(bytes([m&255]) if small else struct.pack('<I',m))
    def mask(m):return b'\x83\xe0'+bytes([m&255]) if m<128 or m>=0xffffff80 else b'\x25'+struct.pack('<I',m)
    if kind[0] in ('S','D') and kind[1:] in ('8','16','32'):
        w=int(kind[1:])//8
        if kind[0]=='S':s,f,m,d=p;b=arg(w)+mem([0x8b],2,s)+flag(f,m)+mem(st(w),0,d)+mem([0x89],2,s)
        else:f,m,d=p;b=arg(w)+flag(f,m)+mem(st(w),0,d)
    elif kind=='PAIR':
        a,b,s,f,m=p;b=arg(4)+b'\x8b\x10'+mem([0x89],2,a)+b'\x8b\x40\x04'+mem([0x8b],2,s)+flag(f,m)+mem([0x89],0,b)+mem([0x89],2,s)
    elif kind=='DUAL':
        a,b,s,f,m,c,d,t,g,n=p;b=arg(4)+b'\x8b\x10'+mem([0x89],2,a)+b'\x8b\x50\x04'+flag(f,m)+mem([0x89],2,b)+mem([0x8b],2,s)+mem([0x89],2,s)+b'\x8b\x10'+mem([0x89],2,c)+b'\x8b\x40\x04'+mem([0x8b],2,t)+flag(g,n)+mem([0x89],0,d)+mem([0x89],2,t)
    elif kind=='SHIFT':
        s,f,m,d,h,k=p;b=arg(4)+mem([0x8b],2,s)+flag(f,m)+bytes([0xc1,0xe0,h])+mem([0x33],0,d)+mem([0x89],2,s)+mask(k)+mem([0x31],0,d)
    elif kind=='MASK':
        s,f,m,d,k=p;b=mem([0x8b],0,d)+b'\x33\x44\x24\x04'+mem([0x8b],2,s)+flag(f,m)+mask(k)+mem([0x31],0,d)+mem([0x89],2,s)
    elif kind=='COPY':
        s,f,m,d=p;b=arg(4)+b'\x8b\x10'+mem([0x8b],0,s)+flag(f,m)+mem([0x89],2,d)+mem([0x89],0,s)
    else:raise ValueError('Unknown family')
    return b+b'\xc2\x04\x00'
