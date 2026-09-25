#!/usr/bin/env python3
"""Local-only differential validation of 14 reconstructed PE32 entrypoints.

Does not launch the EXE. A deliberately bounded integer-instruction interpreter
executes only the selected instruction ranges; external calls use the same
explicit test model as the C++ adapter. This is not a general x86 emulator or
proof of the external dependencies. No original code is sent to CI/GitHub.
"""
from __future__ import annotations
import argparse, copy, ctypes as C, hashlib, json, pathlib, random, re, struct, subprocess, tempfile

SHA = '323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815'
SIZE = 19977216
RANGES = {0x408B50:0x408B8D, 0x408B90:0x408BBF, 0x408BC0:0x408BD5,
          0x408BE0:0x408C10, 0x408C10:0x408C22, 0x408C30:0x408C42,
          0x408C50:0x408C62, 0x408C70:0x408C9E, 0x408CA0:0x408CCE,
          0x408CD0:0x408CE9, 0x408CF0:0x408DCF, 0x408DD0:0x408DD8,
          0x408DE0:0x408DFA, 0x408E00:0x408E1B}
VOID = {0x408BC0,0x408BE0,0x408CF0,0x408DD0,0x408E00}
ARITY = {0x4B14A0:2,0x4E6F10:0,0x4B6EC0:2,0x4B1470:2,0x4B1430:2,
         0xD14C10:3,0x4AFD20:1,0x4AFD40:1,0x4B13F0:1,0x4C5060:1,
         0xD25060:0,0xD02A20:0,0x70000000:1}
MASK = 0xFFFFFFFF
SELF = 0x02000000
STACK = 0x80008000
STOP = 0xFEE1DEAD
LINE = re.compile(r'^\s*([0-9a-f]+):\s+((?:[0-9a-f]{2}\s+)+)\s*([a-z][a-z0-9.]*)\s*(.*?)\s*$')

def require(ok, message):
    if not ok: raise ValueError(message)

def pe_sections(data):
    require(len(data)==SIZE and hashlib.sha256(data).hexdigest()==SHA, 'Wrong exact target size/SHA-256')
    pe=struct.unpack_from('<I',data,0x3c)[0]; opt=pe+24
    require(data[:2]==b'MZ' and data[pe:pe+4]==b'PE\0\0', 'Not a PE')
    require(struct.unpack_from('<H',data,pe+4)[0]==0x14C and struct.unpack_from('<H',data,opt)[0]==0x10B, 'Not x86 PE32')
    require(struct.unpack_from('<I',data,opt+28)[0]==0x400000, 'Unexpected image base')
    section_start=opt+struct.unpack_from('<H',data,pe+20)[0]
    result=[]
    for i in range(struct.unpack_from('<H',data,pe+6)[0]):
        at=section_start+40*i
        name=data[at:at+8].rstrip(b'\0').decode('ascii')
        vs,rva,rs,ro=struct.unpack_from('<IIII',data,at+8)
        result.append((name,0x400000+rva,vs,ro,rs))
    return result

def target_slice(data,sections,va,length):
    for _,base,_,off,raw in sections:
        if base<=va and va+length<=base+raw: return data[off+va-base:off+va-base+length]
    raise ValueError('Unmapped target range')

class Fixture:
    def __init__(self,seed=0,handle=0,status=0,mode=0,resource_mask=15,mutate=True):
        self.seed=seed; self.status=status; self.mode=mode; self.mask=resource_mask
        self.mutate=mutate; self.loaded=0; self.events=[]; self.errors=[]
        self.mem={SELF+i:(0x55550000+i) for i in range(0,0x40,4)}
        self.mem.update({SELF+0x34:handle,0x165A1F0:0x31000000,0x165BBB0:0x32000000,
                        0x16E1D3C:0x33000000,0x16E2EFC:0x34000000,0x16E1660:0x35000000,
                        0x3501E4EC:0x36000000,0x36000058:mode,0x137119C:0x70000000})
        self.mem.update({0x1568444+i*4:0x40000000+i for i in range(8)})
    def read(self,address):
        require(address in self.mem, f'unmapped read {address:08X}')
        value=self.mem[address]; self.events.append(('R',address,value)); return value
    def write(self,address,value):
        require(address in self.mem, f'unmapped write {address:08X}')
        self.mem[address]=value&MASK; self.events.append(('W',address,value&MASK))
    def call(self,target,this,args):
        require(target in ARITY and len(args)==ARITY[target], f'unsupported callee/arity {target:08X}')
        self.events.append(('C',target,this,tuple(args)))
        if target==0x70000000:
            require(this==0 and args==[SELF+8] and self.mem[SELF]==0x137A990, 'destructor import phase')
        if self.mutate:
            for slot in (0x165A1F0,0x165BBB0,0x16E1D3C,0x16E2EFC):
                self.mem[slot]=(self.mem[slot]+0x10)&MASK
            for i in range(8): self.mem[0x1568444+4*i]=(self.mem[0x1568444+4*i]+0x20)&MASK
            if target==0x4B1430: self.mem[SELF+0x34]=(0x80000000 | self.seed)&MASK
            if target==0x4B14A0: self.mem[SELF+0x34]=(0x11110000 | self.seed)&MASK
            if target==0x4B1470: self.mem[SELF+0x34]=(0x22220000 | self.seed)&MASK
            if target==0xD02A20:
                for off in range(0,0x40,4): self.mem[SELF+off]=0x66660000+off
        if target==0x4E6F10: return self.mode
        if target==0x4B1470: return self.status
        if target==0xD14C10:
            n=self.loaded; self.loaded+=1
            require(n<4, 'too many resource loads')
            return 0x50000000+n*0x100 if (self.mask>>n)&1 else 0
        return (0xA17E0000 ^ target ^ this ^ sum(args) ^ self.seed)&MASK

class CPU:
    def __init__(self,fixture,instructions,edges):
        self.f=fixture; self.ins=instructions; self.edges=edges; self.stack={}
        self.r=dict(eax=0x12345678,ecx=SELF,edx=0xAA55AA55,ebx=0x13571357,
                    esi=0x24682468,edi=0x369C369C,ebp=0x45674567,esp=STACK)
        self.z=self.cf=self.sf=self.of=False
    def address(self,text):
        text=text.strip().replace('ds:','')
        if text.startswith('['): text=text[1:-1]
        total=0
        for piece in text.replace('-','+-').split('+'):
            if not piece: continue
            if '*' in piece:
                a,b=piece.split('*'); total+=self.r[a]*int(b,0)
            elif piece in self.r: total+=self.r[piece]
            else: total+=int(piece,0)
        return total&MASK
    def loadmem(self,address,width=32):
        if 0x80000000<=address<0x80010000:
            base=address&~3
            require(base in self.stack, 'uninitialized stack read')
            return (self.stack[base] >> ((address&3)*8)) & ((1<<width)-1)
        require(width==32, 'unsupported non-stack byte read')
        return self.f.read(address)
    def storemem(self,address,value,width=32):
        require(width==32, 'unsupported byte store')
        if 0x80000000<=address<0x80010000: self.stack[address]=value&MASK
        else: self.f.write(address,value)
    def width(self,op): return 8 if op in ('al','cl','dl','bl') or op.startswith('BYTE PTR') else 32
    def get(self,op):
        op=op.strip()
        if op in self.r: return self.r[op]
        if op in ('al','cl','dl','bl'): return self.r['e'+op[0]+'x']&255
        if 'PTR' in op: return self.loadmem(self.address(op.split('PTR',1)[1]),self.width(op))
        if op.startswith('ds:'): return self.loadmem(self.address(op))
        return int(op,0)&MASK
    def put(self,op,value):
        value &= (1<<self.width(op))-1
        if op in self.r: self.r[op]=value
        elif op in ('al','cl','dl','bl'):
            reg='e'+op[0]+'x'; self.r[reg]=(self.r[reg]&0xFFFFFF00)|value
        elif 'PTR' in op: self.storemem(self.address(op.split('PTR',1)[1]),value,self.width(op))
        else: raise ValueError('invalid destination '+op)
    def push(self,value): self.r['esp']-=4; self.stack[self.r['esp']]=value&MASK
    def pop(self): value=self.loadmem(self.r['esp']); self.r['esp']+=4; return value
    def arithmetic(self,left,right,result,width,subtract):
        mask=(1<<width)-1; sign=1<<(width-1); masked=result&mask
        self.z=masked==0; self.sf=bool(masked&sign)
        self.cf=left<right if subtract else result>mask
        self.of=bool(((left^right)&(left^masked)&sign) if subtract else (~(left^right)&(left^masked)&sign))
    def run(self,va,argument):
        has_arg=va in (0x408CD0,0x408CF0)
        self.stack[STACK]=STOP
        if has_arg: self.stack[STACK+4]=argument
        ip=va; steps=0
        while ip!=STOP:
            steps+=1; require(steps<1000 and ip in self.ins, f'execution outside verified ranges at {ip:08X}')
            size,mn,ops=self.ins[ip]; args=[x.strip() for x in ops.split(',')] if ops else []
            nxt=ip+size
            if mn=='mov': self.put(args[0],self.get(args[1]))
            elif mn=='lea': self.put(args[0],self.address(args[1]))
            elif mn=='push': self.push(self.get(args[0]))
            elif mn=='pop': self.put(args[0],self.pop())
            elif mn in ('xor','test'):
                a,b=map(self.get,args); value=(a^b) if mn=='xor' else (a&b)
                width=self.width(args[0]); self.z=value==0; self.sf=bool(value&(1<<(width-1))); self.cf=self.of=False
                if mn=='xor': self.put(args[0],value)
            elif mn in ('cmp','add','sbb'):
                a,b=map(self.get,args); width=self.width(args[0]); sub=mn!='add'
                if mn=='sbb': b+=int(self.cf)
                result=a-b if sub else a+b
                self.arithmetic(a,b,result,width,sub)
                if mn!='cmp': self.put(args[0],result)
            elif mn=='neg':
                a=self.get(args[0]); result=-a
                self.arithmetic(0,a,result,self.width(args[0]),True); self.put(args[0],result)
            elif mn=='shl':
                a,b=map(self.get,args); self.put(args[0],a<<b)
                value=self.get(args[0]); self.z=value==0; self.sf=bool(value&0x80000000)
                self.cf=bool((a>>(32-b))&1) if b else self.cf
            elif mn=='sete': self.put(args[0],int(self.z))
            elif mn in ('je','jne','jl'):
                taken=self.z if mn=='je' else not self.z if mn=='jne' else self.sf!=self.of
                dest=self.get(args[0]); self.edges.add((ip,dest if taken else nxt))
                if taken: nxt=dest
            elif mn=='jmp': nxt=self.get(args[0])
            elif mn=='call':
                target=self.get(args[0])
                if target in RANGES:
                    self.push(nxt); nxt=target
                else:
                    require(target in ARITY, f'unmodeled external call {target:08X}')
                    count=ARITY[target]
                    callargs=[self.loadmem(self.r['esp']+4*i) for i in range(count)]
                    this=0 if target==0x70000000 else self.r['ecx']
                    value=self.f.call(target,this,callargs)
                    self.r['esp']+=4*count
                    self.r['eax']=value; self.r['ecx']=0xE0C0C0C0; self.r['edx']=0xE0D0D0D0
            elif mn=='ret':
                nxt=self.pop(); self.r['esp']+=self.get(args[0]) if args else 0
            else: raise ValueError('Unsupported instruction '+mn+' '+ops)
            ip=nxt
        require(self.r['esp']==STACK+4+(4 if has_arg else 0), 'incorrect stack cleanup')
        for reg,value in dict(ebx=0x13571357,esi=0x24682468,edi=0x369C369C,ebp=0x45674567).items():
            require(self.r[reg]==value,'callee-saved register corrupted: '+reg)
        return 0 if va in VOID else self.r['eax']&255 if va==0x408B90 else self.r['eax']

READ=C.CFUNCTYPE(C.c_uint32,C.c_void_p,C.c_uint32)
WRITE=C.CFUNCTYPE(None,C.c_void_p,C.c_uint32,C.c_uint32)
CALL=C.CFUNCTYPE(C.c_uint32,C.c_void_p,C.c_uint32,C.c_uint32,C.POINTER(C.c_uint32),C.c_uint32)
class Access(C.Structure): _fields_=[('context',C.c_void_p),('read32',READ),('write32',WRITE),('invoke',CALL)]

def native(lib,f,va,arg):
    def protect(fn,*args):
        try: return fn(*args)
        except Exception as exc: f.errors.append(str(exc)); return 0
    r=READ(lambda _,at:protect(f.read,at))
    w=WRITE(lambda _,at,val:protect(f.write,at,val))
    c=CALL(lambda _,target,this,args,n:protect(f.call,target,this,[args[i] for i in range(n)]))
    result=lib.re5_batch_408_run(va,C.byref(Access(None,r,w,c)),SELF,arg)
    require(not f.errors,'native callback failure: '+str(f.errors))
    return result

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--exe',type=pathlib.Path,required=True)
    ap.add_argument('--source-root',type=pathlib.Path,default=pathlib.Path(__file__).resolve().parents[2])
    ap.add_argument('--compiler',default='g++'); ap.add_argument('--objdump',default='objdump')
    ap.add_argument('--report',type=pathlib.Path)
    args=ap.parse_args(); data=args.exe.read_bytes(); sections=pe_sections(data)
    text=subprocess.check_output([args.objdump,'-d','-Mintel','--insn-width=16','--start-address=0x408b50','--stop-address=0x408e1b',str(args.exe)],text=True)
    instructions={}; expected_edges=set(); ranges=[]
    for line in text.splitlines():
        m=LINE.match(line)
        if not m: continue
        va=int(m[1],16)
        if any(start<=va<end for start,end in RANGES.items()):
            raw=bytes.fromhex(m[2]); require(target_slice(data,sections,va,len(raw))==raw,'decoder byte disagreement')
            instructions[va]=(len(raw),m[3],m[4])
            if m[3] in ('je','jne','jl'): expected_edges.update(((va,va+len(raw)),(va,int(m[4],0))))
    for start,end in RANGES.items():
        at=start
        while at<end:
            require(at in instructions,'instruction gap'); at+=instructions[at][0]
        require(at==end,'instruction boundary mismatch')
        ranges.append({'va':f'0x{start:08X}','end_exclusive':f'0x{end:08X}','size':end-start,
                       'sha256':hashlib.sha256(target_slice(data,sections,start,end-start)).hexdigest()})
    scenarios=[]; boundaries=[0,1,2,3,0x7FFFFFFF,0x80000000,0x80000001,0xFFFFFFFF]
    for va in RANGES:
        for n in range(96):
            seed=n*0x12345; rnd=random.Random(seed)
            scenarios.append((va,Fixture(seed, boundaries[n%8],boundaries[(n//8)%8],boundaries[(n//4)%8],n%16,n%2==0),
                              [0,1,0x100,0x80000000,0xFFFFFFFF,rnd.getrandbits(32)][n%6]))
    for va in (0x408B90,0x408BC0,0x408BE0):
        f=Fixture(handle=0xFFFFFFFF,mutate=False); f.mem={SELF+0x34:0xFFFFFFFF}
        scenarios.append((va,f,0))
    # Every byte under multiple nonzero high-word patterns distinguishes the
    # CD0 byte argument from CF0's full-word boolean normalization.
    for high in (0,0x100,0x7FFFFF00,0xFFFFFF00):
        for low in range(256): scenarios.append((0x408CD0,Fixture(seed=low,mutate=False),high|low))
    observed=set(); counts={f'{va:08X}':0 for va in RANGES}; events=0
    with tempfile.TemporaryDirectory(prefix='re5-batch-') as tmp:
        so=pathlib.Path(tmp)/'batch.so'; root=args.source_root
        subprocess.run([args.compiler,'-std=c++20','-O2','-Wall','-Wextra','-Wpedantic','-Werror','-shared','-fPIC',
                        '-I'+str(root/'include'),str(root/'src/recovered/batch_00408b50.cpp'),
                        str(root/'tools/verification/batch_00408b50_bridge.cpp'),'-o',str(so)],check=True)
        lib=C.CDLL(str(so)); lib.re5_batch_408_run.restype=C.c_uint32
        lib.re5_batch_408_run.argtypes=[C.c_uint32,C.POINTER(Access),C.c_uint32,C.c_uint32]
        for va,original,arg in scenarios:
            ref=copy.deepcopy(original); recovered=copy.deepcopy(original)
            expected=CPU(ref,instructions,observed).run(va,arg)
            actual=native(lib,recovered,va,arg)
            require(expected==actual,f'{va:08X} return differs: {expected:08X}/{actual:08X}')
            require(ref.events==recovered.events,f'{va:08X} ordered memory/call trace differs:\n{ref.events}\n{recovered.events}')
            require(ref.mem==recovered.mem,f'{va:08X} final target memory differs')
            counts[f'{va:08X}']+=1; events+=len(ref.events)
    require(expected_edges<=observed,'Uncovered conditional edges: '+str(sorted(expected_edges-observed)))
    report={'status':'PASS','input_sha256':SHA,'input_size':SIZE,'functions':14,'instruction_bytes':sum(e-s for s,e in RANGES.items()),
            'differential_scenarios':len(scenarios),'ordered_memory_and_call_events_compared':events,
            'conditional_edges_covered':len(expected_edges),'conditional_edges_total':len(expected_edges),
            'cases_per_entrypoint':counts,'ranges':ranges,
            'scope':'Bounded integer instruction model versus C++; same explicit external-callee fixture. Not native x86 execution, dependency verification, matching, linking, or whole-game validation.'}
    if args.report: args.report.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report,indent=2))

if __name__=='__main__': main()
