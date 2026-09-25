"""Small closed grammar of data-transfer bodies, not a runtime interpreter."""
import struct
KINDS=('STORE','SNAPSHOT','DIRTY','INDIRECT','DUPLICATE','DUP_ZERO','STORE_FLAG','PAIR16','PAIR32','NEST16')
class Reader:
    def __init__(self,b):self.b=b;self.i=0;self.jumps=[]
    def take(self,n):
        if self.i+n>len(self.b):raise ValueError('Truncated instruction')
        v=self.b[self.i:self.i+n];self.i+=n;return v
    def exact(self,b):
        if self.take(len(b))!=b:raise ValueError('Wrong opcode or register')
    def mem(self,op,reg,base=1,sib=False):
        self.exact(bytes(op));m=self.take(1)[0];mode=m>>6
        if mode==3 or (m>>3)&7!=reg or m&7!=(4 if sib else base):raise ValueError('Wrong memory operand')
        if sib:self.exact(bytes([0x80|base]))
        if mode==0 and base==5:raise ValueError('Absolute addressing not supported')
        d=0 if mode==0 else int.from_bytes(self.take(1 if mode==1 else 4),'little',signed=True)
        if not 0<=d<0x20000:raise ValueError('Unsupported displacement')
        return d
    def flags(self):
        op=self.b[self.i]
        if op not in (0x81,0x83):raise ValueError('Not OR immediate')
        d=self.mem([op],1);value=int.from_bytes(self.take(1 if op==0x83 else 4),'little',signed=op==0x83)&0xffffffff
        return d,value
    def branch(self,op):
        self.exact(bytes([op]));delta=int.from_bytes(self.take(1),'little',signed=True);self.jumps.append(self.i+delta)
    def end(self):
        self.exact(b'\xc2\x04\x00')
        if self.i!=len(self.b):raise ValueError('Trailing code')
ARG=b'\xf3\x0f\x10\x44\x24\x04'
OUT=b'\x8b\x44\x24\x04'
SSE=[0xf3,0x0f,0x11]

def decode(kind,code):
    r=Reader(code)
    if kind=='SNAPSHOT':
        s=r.mem([0x8b],0);r.exact(ARG);f,m=r.flags();d=r.mem(SSE,0)
        if r.mem([0x89],0)!=s:raise ValueError('Wrong saved field')
        p=[s,f,m,d]
    elif kind=='INDIRECT':
        a=r.mem([0x8b],0);r.exact(ARG);d=r.mem(SSE,0,0);p=[a,d]
    elif kind in ('PAIR16','PAIR32'):
        prefix=[0x66] if kind=='PAIR16' else []
        a=r.mem(prefix+[0x8b],2);r.exact(OUT);b=r.mem(prefix+[0x8b],1)
        c=r.mem(prefix+[0x89],2,0);d=r.mem(prefix+[0x89],1,0);p=[a,b,c,d]
    elif kind=='NEST16':
        t=r.mem([0x8b],2);r.exact(b'\x85\xd2');r.branch(0x74)
        i=r.mem([0x8b],0);r.exact(b'\x85\xc0');r.branch(0x7c)
        c=r.mem([0x3b],0,2);r.branch(0x7d);e=r.mem([0x8b],1,2,True)
        r.exact(b'\x85\xc9');r.branch(0x74);a=r.mem([0x66,0x8b],2);r.exact(OUT);b=r.mem([0x66,0x8b],1)
        d=r.mem([0x66,0x89],2,0);f=r.mem([0x66,0x89],1,0);r.exact(b'\xc2\x04\x00')
        target=r.i
        if any(j!=target for j in r.jumps):raise ValueError('Wrong early-exit target')
        r.exact(OUT)
        if r.mem([0x66,0xc7],0,0)!=d:raise ValueError('Default destination mismatch')
        x=int.from_bytes(r.take(2),'little')
        if r.mem([0x66,0xc7],0,0)!=f:raise ValueError('Default destination mismatch')
        y=int.from_bytes(r.take(2),'little');p=[t,i,c,e,a,b,d,f,x,y]
    else:
        r.exact(ARG)
        if kind=='DIRTY':f,m=r.flags();p=[f,m,r.mem(SSE,0)]
        else:
            d=r.mem(SSE,0);p=[d]
            if kind in ('DUPLICATE','DUP_ZERO'):
                p.append(r.mem(SSE,0))
                if kind=='DUP_ZERO':r.exact(b'\x0f\x57\xc0');p.append(r.mem(SSE,0))
            elif kind=='STORE_FLAG':p+=[r.mem([0xc6],0),r.take(1)[0]]
            elif kind!='STORE':raise ValueError('Unknown kind')
    r.end();return p

def encode(kind,p):
    def mem(op,reg,d,base=1,sib=False):
        mode=0 if d==0 else 1 if d<128 else 2
        return bytes(op)+bytes([(mode<<6)|(reg<<3)|(4 if sib else base)])+(bytes([0x80|base]) if sib else b'')+(b'' if mode==0 else d.to_bytes(1 if mode==1 else 4,'little'))
    def flag(f,m):
        small=m<128 or m>=0xffffff80
        return mem([0x83 if small else 0x81],1,f)+(bytes([m&255]) if small else struct.pack('<I',m))
    end=b'\xc2\x04\x00'
    if kind=='STORE':b=ARG+mem(SSE,0,p[0])
    elif kind=='SNAPSHOT':s,f,m,d=p;b=mem([0x8b],0,s)+ARG+flag(f,m)+mem(SSE,0,d)+mem([0x89],0,s)
    elif kind=='DIRTY':f,m,d=p;b=ARG+flag(f,m)+mem(SSE,0,d)
    elif kind=='INDIRECT':a,d=p;b=mem([0x8b],0,a)+ARG+mem(SSE,0,d,0)
    elif kind=='DUPLICATE':a,d=p;b=ARG+mem(SSE,0,a)+mem(SSE,0,d)
    elif kind=='DUP_ZERO':a,d,z=p;b=ARG+mem(SSE,0,a)+mem(SSE,0,d)+b'\x0f\x57\xc0'+mem(SSE,0,z)
    elif kind=='STORE_FLAG':a,f,v=p;b=ARG+mem(SSE,0,a)+mem([0xc6],0,f)+bytes([v])
    elif kind in ('PAIR16','PAIR32'):
        a,d,c,e=p;pre=[0x66] if kind=='PAIR16' else []
        b=mem(pre+[0x8b],2,a)+OUT+mem(pre+[0x8b],1,d)+mem(pre+[0x89],2,c,0)+mem(pre+[0x89],1,e,0)
    elif kind=='NEST16':
        t,i,c,e,a,d,f,g,x,y=p;b=bytearray();jumps=[]
        def add(v):b.extend(v)
        def jump(op):jumps.append(len(b));add(bytes([op,0]))
        add(mem([0x8b],2,t)+b'\x85\xd2');jump(0x74)
        add(mem([0x8b],0,i)+b'\x85\xc0');jump(0x7c)
        add(mem([0x3b],0,c,2));jump(0x7d)
        add(mem([0x8b],1,e,2,True)+b'\x85\xc9');jump(0x74)
        add(mem([0x66,0x8b],2,a)+OUT+mem([0x66,0x8b],1,d)+mem([0x66,0x89],2,f,0)+mem([0x66,0x89],1,g,0)+end)
        target=len(b)
        add(OUT+mem([0x66,0xc7],0,f,0)+struct.pack('<H',x)+mem([0x66,0xc7],0,g,0)+struct.pack('<H',y))
        for j in jumps:b[j+1]=target-j-2
        b=bytes(b)
    else:raise ValueError('Unknown kind')
    return b+end
