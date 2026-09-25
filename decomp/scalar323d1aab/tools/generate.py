#!/usr/bin/env python3
"""Reproduce readable C++ from audited scalar semantics, not binary stubs."""
from __future__ import annotations
import argparse,base64,collections,hashlib,json,pathlib,struct,zlib
ROOT=pathlib.Path(__file__).resolve().parents[1]
FAMILIES=['noop','zero8','constant8','store_arg32','load8','store_imm8','bitfield32','store_imm32','store_arg8','bitfield8','store_arg16','global_copy32','global_store32']
FIELDS=['encoding','displacement','imm','shift','stack_pop','variant','src','dst','load_form','mask_form']

def body(r):
    f=r['family'];e=r.get('encoding',0);d=r.get('displacement',0)
    mr={0:1,8:0x41,32:0x81}[e]
    disp=b'' if e==0 else int(d).to_bytes(e//8,'little',signed=True)
    imm=r.get('imm',0)
    if f=='noop':return b'\xc3' if r['stack_pop']==0 else b'\xc2'+struct.pack('<H',r['stack_pop'])
    if f=='zero8':return bytes([r['variant'],0xc0,0xc3])
    if f=='constant8':return bytes([0xb0,imm,0xc3])
    if f=='load8':return bytes([0x8a,mr])+disp+b'\xc3'
    if f.startswith('store_arg'):
        op={'store_arg8':b'\x88','store_arg16':b'\x66\x89','store_arg32':b'\x89'}[f]
        return bytes.fromhex(r['load_form'])+op+bytes([mr])+disp+b'\xc2\x04\x00'
    if f.startswith('store_imm'):
        return bytes([0xc6 if f=='store_imm8' else 0xc7,mr])+disp+imm.to_bytes(1 if f=='store_imm8' else 4,'little')+b'\xc3'
    if f.startswith('bitfield'):
        op=bytes.fromhex(r['mask_form']);n=4 if op==b'\x25' else 1
        return bytes([0x8b,mr])+disp+bytes([0xc1,0xe8,r['shift']])+op+(imm&((1<<(8*n))-1)).to_bytes(n,'little')+b'\xc3'
    if f=='global_store32':return b'\xc7\x05'+struct.pack('<II',r['dst'],imm)+b'\xc3'
    if f=='global_copy32':
        a,b=[(b'\xa1',b'\xa3'),(b'\x8b\x05',b'\xa3'),(b'\xa1',b'\x89\x05'),(b'\x8b\x05',b'\x89\x05')][r['variant']]
        return a+struct.pack('<I',r['src'])+b+struct.pack('<I',r['dst'])+b'\xc3'
    raise ValueError(f)

def load_records(root=ROOT):
    raw=zlib.decompress(base64.b85decode(''.join((root/'metadata/semantics.b85').read_text().split())))
    if raw[:4]!=b'SCS1':raise ValueError('Wrong semantic-record format')
    pos=4;out=[]
    forms=['8b442404','8a442404','0fb6442404','668b442404','0fb7442404','24','83e0','25']
    def uv():
        nonlocal pos
        value=0;shift=0
        while True:
            if pos>=len(raw) or shift>63:raise ValueError('Invalid semantic varint')
            x=raw[pos];pos+=1;value|=(x&127)<<shift
            if not x&128:return value
            shift+=7
    for family in FAMILIES:
        count=uv();mask=uv();va=0;previous=[0]*len(FIELDS)
        if count>20000 or mask>>len(FIELDS):raise ValueError('Invalid semantic group')
        for _ in range(count):
            va+=uv();r=dict(va=va,family=family)
            for i,k in enumerate(FIELDS):
                if not mask&(1<<i):continue
                v=uv();delta=-(v//2)-1 if v&1 else v//2;previous[i]+=delta
                r[k]=forms[previous[i]] if k in ('load_form','mask_form') else previous[i]
            r.setdefault('displacement',0);r['symbol']=f'FUN_{va:08X}';r['rva']=va-0x400000
            r['file_offset']=r['rva']-0xc00
            code=body(r);r['size']=len(code);r['body_sha256']=hashlib.sha256(code).hexdigest()
            assert r['file_offset']>=0 and va%16==0
            out.append(r)
    assert pos==len(raw),'Trailing semantic data'
    out.sort(key=lambda r:r['va'])
    assert len({r['va'] for r in out})==len(out)
    return out

HEADER='''#pragma once
#if defined(__i386__) || defined(_M_IX86)
# if defined(_MSC_VER)
#  define RE5_SCALAR_CC __thiscall
#  define RE5_SCALAR_POP __stdcall
# else
#  define RE5_SCALAR_CC __attribute__((thiscall))
#  define RE5_SCALAR_POP __attribute__((stdcall))
# endif
#else
# define RE5_SCALAR_CC
# define RE5_SCALAR_POP
#endif
namespace re5::scalar323d1aab {
using u8=unsigned char;
using u16=unsigned short;
using u32=unsigned int;
#if defined(_MSC_VER) && !defined(__clang__)
# if defined(_WIN64)
using uptr=unsigned long long;
# else
using uptr=unsigned int;
# endif
#else
using uptr=__UINTPTR_TYPE__;
#endif
static_assert(sizeof(u8)==1 && sizeof(u16)==2 && sizeof(u32)==4);
static_assert(sizeof(uptr)==sizeof(void*));
'''
DETAIL='''#pragma once
#include "re5/scalars.hpp"
namespace re5::scalar323d1aab::detail {
inline u32 load32(const void* self,u32 off) noexcept {
    const auto* p=static_cast<const u8*>(self)+off;
    return u32(p[0]) | (u32(p[1])<<8U) | (u32(p[2])<<16U) | (u32(p[3])<<24U);
}
inline void store8(void* self,u32 off,u8 x) noexcept { static_cast<u8*>(self)[off]=x; }
inline void store16(void* self,u32 off,u16 x) noexcept {
    auto* p=static_cast<u8*>(self)+off;
    p[0]=u8(x);p[1]=u8(x>>8U);
}
inline void store32(void* self,u32 off,u32 x) noexcept {
#if defined(__clang__) || defined(__GNUC__)
    __builtin_memcpy(static_cast<u8*>(self)+off,&x,4);
#else
    auto* p=static_cast<u8*>(self)+off;
    p[0]=u8(x);p[1]=u8(x>>8U);p[2]=u8(x>>16U);p[3]=u8(x>>24U);
#endif
}
inline void* address(u32 x) noexcept { return reinterpret_cast<void*>(uptr(x)); }
}
'''

def signature(r,decl=False):
    f=r['family'];s=r['symbol']
    if f=='noop':return f'void RE5_SCALAR_POP {s}('+', '.join('u32' for _ in range(r['stack_pop']//4))+') noexcept'
    if f in ['zero8','constant8','load8','bitfield8']:return f'u8 RE5_SCALAR_CC {s}(const void*'+(' self' if f in ['load8','bitfield8'] else '')+') noexcept'
    if f=='bitfield32':return f'u32 RE5_SCALAR_CC {s}(const void* self) noexcept'
    if f.startswith('store_arg'):return f'void RE5_SCALAR_CC {s}(void* self, u{f[9:]} value) noexcept'
    if f.startswith('store_imm'):return f'void RE5_SCALAR_CC {s}(void* self) noexcept'
    if f=='global_copy32':return f'u32 {s}() noexcept'
    if f=='global_store32':return f'void {s}() noexcept'
    raise ValueError(f)

def implementation(r):
    f=r['family'];d=r['displacement'];imm=r.get('imm',0)
    if f=='noop':return ''
    if f=='zero8':return '    return 0;\n'
    if f=='constant8':return f'    return {imm}U;\n'
    if f=='load8':return f'    return static_cast<const u8*>(self)[{d}U];\n'
    if f.startswith('store_arg'):return f'    detail::store{f[9:]}(self,{d}U,value);\n'
    if f.startswith('store_imm'):return f'    detail::store{f[9:]}(self,{d}U,0x{imm:08X}U);\n'
    if f.startswith('bitfield'):
        # SHR uses imm8 modulo 32. AL results do not assert the unused high bits.
        typ='u8' if f=='bitfield8' else 'u32'
        return f'    return static_cast<{typ}>((detail::load32(self,{d}U) >> {r["shift"]&31}U) & 0x{imm:08X}U);\n'
    if f=='global_store32':return f'    detail::store32(detail::address(0x{r["dst"]:08X}U),0,0x{imm:08X}U);\n'
    if f=='global_copy32':return f'    const u32 value=detail::load32(detail::address(0x{r["src"]:08X}U),0);\n    detail::store32(detail::address(0x{r["dst"]:08X}U),0,value);\n    return value;\n'
    raise ValueError(f)

CAT='''#pragma once
#include "re5/scalars.hpp"
namespace re5::scalar323d1aab {
enum class Kind { '''+', '.join(FAMILIES)+''' };
using Action=void(*)() noexcept;
using G8=u8(RE5_SCALAR_CC*)(const void*) noexcept;
using G32=u32(RE5_SCALAR_CC*)(const void*) noexcept;
using W8=void(RE5_SCALAR_CC*)(void*,u8) noexcept;
using W16=void(RE5_SCALAR_CC*)(void*,u16) noexcept;
using W32=void(RE5_SCALAR_CC*)(void*,u32) noexcept;
using Store=void(RE5_SCALAR_CC*)(void*) noexcept;
using Copy=u32(*)() noexcept;
struct Entry {
    u32 va;Kind kind;u32 offset,imm,shift,src,dst,stack_pop;
    Action action;G8 g8;G32 g32;W8 w8;W16 w16;W32 w32;Store store;Copy copy;
};
extern const Entry entries[];
extern const unsigned entry_count;
}
'''

def generate(records):
    result={};inc='include/re5/';result[inc+'scalars.hpp']=HEADER+'\n'.join(signature(r,True)+';' for r in records)+'\n}\n'
    result[inc+'scalar_detail.hpp']=DETAIL;result[inc+'scalar_catalog.hpp']=CAT
    for i in range(0,len(records),256):
        text='#include "re5/scalar_detail.hpp"\nnamespace re5::scalar323d1aab {\n'
        for r in records[i:i+256]:
            text+=f'// VA 0x{r["va"]:08X}; {r["size"]} original bytes; {r["family"]}.\n'+signature(r)+' {\n'+implementation(r)+'}\n'
        result[f'src/scalars_{i//256:03}.cpp']=text+'}\n'
    text='#include "re5/scalar_catalog.hpp"\nnamespace re5::scalar323d1aab {\nnamespace {\n'
    for n in sorted({r['stack_pop'] for r in records if r['family']=='noop'}):
        # Typed trampoline exists solely in the TEST catalogue, never in recovered bodies.
        args=', '.join('u32' for _ in range(n//4));vals=', '.join('0U' for _ in range(n//4))
        text+=f'template<void(RE5_SCALAR_POP *F)({args}) noexcept> void invoke_{n}() noexcept {{ F({vals}); }}\n'
    text+='}\nconst Entry entries[]={\n'
    for r in records:
        f=r['family'];ptrs=['nullptr']*8
        slot=0 if f in ('noop','global_store32') else 1 if f in ('zero8','constant8','load8','bitfield8') else 2 if f=='bitfield32' else 3 if f=='store_arg8' else 4 if f=='store_arg16' else 5 if f=='store_arg32' else 6 if f.startswith('store_imm') else 7
        ptrs[slot]='&'+(f'invoke_{r["stack_pop"]}<{r["symbol"]}>' if f=='noop' else r['symbol'])
        vals=[f'0x{r["va"]:08X}U','Kind::'+f]+[str(r.get(k,0))+'U' for k in ('displacement','imm','shift','src','dst','stack_pop')]+ptrs
        text+='    {'+','.join(vals)+'},\n'
    text+='};\nconst unsigned entry_count=sizeof(entries)/sizeof(entries[0]);\n}\n';result['src/catalog.cpp']=text
    catalogue={'input_sha256':'323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815','input_size':19977216,'record_count':len(records),'instruction_bytes':sum(r['size'] for r in records),'families':dict(collections.Counter(r['family'] for r in records)), 'canonical_membership_verified':False,'full_game_linked':False,'records':records}
    result['catalog.json']=json.dumps(catalogue,indent=2,sort_keys=True)+'\n'
    return result

def digest(files):
    h=hashlib.sha256()
    for p,s in sorted(files.items()):h.update(p.encode()+b'\0'+s.encode()+b'\0')
    return h.hexdigest()

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--check',action='store_true');args=ap.parse_args()
    files=generate(load_records());expected=json.loads((ROOT/'metadata/provenance.json').read_text())['generated_digest']
    assert digest(files)==expected,'generation differs from locally verified source'
    out=ROOT/'generated'
    if args.check:
        for p,s in files.items():assert (out/p).read_bytes()==s.encode(),p
        assert {str(p.relative_to(out)) for p in out.rglob('*') if p.is_file()}==set(files),'unexpected generated files'
    else:
        for p,s in files.items():q=out/p;q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(s.encode())
    print('PASS:',len(load_records()),'scalar bodies; digest',digest(files))
if __name__=='__main__':main()
