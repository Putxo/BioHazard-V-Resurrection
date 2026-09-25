#include "mutations.hpp"
#if !defined(RE5_FREESTANDING_I386)
#include <sys/mman.h>
#include <unistd.h>
#endif
using namespace re5::mutator323d1aab;
namespace {
using uptr=__UINTPTR_TYPE__;
u32 checks=0,seed=0x8ab37621U;
const char* current="setup";
u32 length(const char* s){u32 n=0;while(s[n])++n;return n;}
#if defined(RE5_FREESTANDING_I386)
static_assert(sizeof(void*)==4);
u32 sys3(u32 n,u32 a,u32 b,u32 c){u32 r;asm volatile("int $0x80":"=a"(r):"0"(n),"b"(a),"c"(b),"d"(c):"memory");return r;}
void output(const char* s){sys3(4,1,u32(uptr(s)),length(s));}
[[noreturn]] void quit(u32 n){sys3(1,n,0,0);__builtin_unreachable();}
void* allocate(u32 n){u32 a[]={0,n,3,0x22,0xffffffffU,0};u32 p=sys3(90,u32(uptr(a)),0,0);return p>=0xfffff001U?nullptr:reinterpret_cast<void*>(uptr(p));}
bool protect(void* p,u32 n,u32 f){return sys3(125,u32(uptr(p)),n,f)==0;}
void release(void* p,u32 n){sys3(91,u32(uptr(p)),n,0);}
#else
void output(const char* s){const auto ignored=write(1,s,length(s));(void)ignored;}
[[noreturn]] void quit(u32 n){_exit(int(n));}
void* allocate(u32 n){void* p=mmap(nullptr,n,3,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);return p==MAP_FAILED?nullptr:p;}
bool protect(void* p,u32 n,u32 f){return mprotect(p,n,int(f))==0;}
void release(void* p,u32 n){munmap(p,n);}
#endif
void number(u32 n){char s[12];u32 i=11;s[i]=0;do{s[--i]=char('0'+n%10);n/=10;}while(n);output(s+i);}
void require(bool b){++checks;if(!b){output("FAIL ");output(current);output(" check ");number(checks);output("\n");quit(1);}}
u32 random_word(){seed^=seed<<13U;seed^=seed>>17U;seed^=seed<<5U;return seed;}
u32 read(const u8* p,u32 o){u32 v=0;for(u32 i=0;i<4;++i)v|=u32(p[o+i])<<(i*8U);return v;}
void put(u8* p,u32 o,u32 v,u32 w=4){for(u32 i=0;i<w;++i)p[o+i]=u8(v>>(i*8U));}
void copy(u8* to,const u8* from,u32 n){for(u32 i=0;i<n;++i)to[i]=from[i];}
bool equal(const u8* a,const u8* b,u32 n){for(u32 i=0;i<n;++i)if(a[i]!=b[i])return false;return true;}
void fill(u8* p,u32 n,u32 salt){for(u32 i=0;i<n;++i)p[i]=u8((i*29U)^(i>>3U)^salt^(salt>>((i%4U)*8U)));}
using Scalar=void(RE5_MUT_CC*)(void*,u32) noexcept;
using From=void(RE5_MUT_CC*)(void*,const void*) noexcept;
enum Kind {S8,S16,S32,D8,D16,D32,Pair,Dual,Shift,Mask,Copy};
struct Case {const char* name;Kind kind;u32 p[10];Scalar scalar;From from;};
#if defined(RE5_TEST_REFERENCE)
extern "C" {
#define RE5_S8(n,...) void RE5_MUT_CC original_##n(void*,u32) noexcept;
#define RE5_S16 RE5_S8
#define RE5_S32 RE5_S8
#define RE5_D8 RE5_S8
#define RE5_D16 RE5_S8
#define RE5_D32 RE5_S8
#define RE5_SHIFT RE5_S8
#define RE5_MASK RE5_S8
#define RE5_PAIR(n,...) void RE5_MUT_CC original_##n(void*,const void*) noexcept;
#define RE5_DUAL RE5_PAIR
#define RE5_COPY RE5_PAIR
#include "records.inc"
#undef RE5_S8
#undef RE5_S16
#undef RE5_S32
#undef RE5_D8
#undef RE5_D16
#undef RE5_D32
#undef RE5_SHIFT
#undef RE5_MASK
#undef RE5_PAIR
#undef RE5_DUAL
#undef RE5_COPY
}
#endif
#define RE5_S8(n,...) {#n,S8,{__VA_ARGS__},&FN(n),nullptr},
#define RE5_S16(n,...) {#n,S16,{__VA_ARGS__},&FN(n),nullptr},
#define RE5_S32(n,...) {#n,S32,{__VA_ARGS__},&FN(n),nullptr},
#define RE5_D8(n,...) {#n,D8,{__VA_ARGS__},&FN(n),nullptr},
#define RE5_D16(n,...) {#n,D16,{__VA_ARGS__},&FN(n),nullptr},
#define RE5_D32(n,...) {#n,D32,{__VA_ARGS__},&FN(n),nullptr},
#define RE5_SHIFT(n,...) {#n,Shift,{__VA_ARGS__},&FN(n),nullptr},
#define RE5_MASK(n,...) {#n,Mask,{__VA_ARGS__},&FN(n),nullptr},
#define RE5_PAIR(n,...) {#n,Pair,{__VA_ARGS__},nullptr,&FN(n)},
#define RE5_DUAL(n,...) {#n,Dual,{__VA_ARGS__},nullptr,&FN(n)},
#define RE5_COPY(n,...) {#n,Copy,{__VA_ARGS__},nullptr,&FN(n)},
#define FN(n) n
const Case source_cases[]={
#include "records.inc"
};
#undef FN
#if defined(RE5_TEST_REFERENCE)
#define FN(n) original_##n
const Case original_cases[]={
#include "records.inc"
};
#undef FN
#endif
#undef RE5_S8
#undef RE5_S16
#undef RE5_S32
#undef RE5_D8
#undef RE5_D16
#undef RE5_D32
#undef RE5_SHIFT
#undef RE5_MASK
#undef RE5_PAIR
#undef RE5_DUAL
#undef RE5_COPY
static_assert(sizeof(source_cases)/sizeof(Case)==422);
u32 width(Kind k){return k==S8||k==D8?1U:k==S16||k==D16?2U:4U;}
u32 fields(const Case& f,u32* offsets,u32* widths){
    const u32* p=f.p;u32 n=0;
    auto add=[&](u32 o,u32 w=4U){offsets[n]=o;widths[n++]=w;};
    switch(f.kind){
    case S8:case S16:case S32:add(p[0]);add(p[1]);add(p[3],width(f.kind));break;
    case D8:case D16:case D32:add(p[0]);add(p[2],width(f.kind));break;
    case Pair:for(u32 i=0;i<4;++i)add(p[i]);break;
    case Dual:for(u32 i=0;i<4;++i){add(p[i]);add(p[i+5]);}break;
    case Copy:case Shift:case Mask:add(p[0]);add(p[1]);add(p[3]);break;
    }
    return n;
}
u32 extent(const Case& f){u32 o[10],w[10],max=0;const u32 n=fields(f,o,w);for(u32 i=0;i<n;++i)if(o[i]+w[i]>max)max=o[i]+w[i];return max;}
// Independently sequenced memory oracle; native fixtures test this expectation too.
void expected(const Case& f,u8* self,u32 arg,const u8* src){
    const u32* p=f.p;
    auto flags=[&](u32 at,u32 bits){put(self,at,read(self,at)|bits);};
    switch(f.kind){
    case S8:case S16:case S32:{u32 saved=read(self,p[0]);flags(p[1],p[2]);put(self,p[3],arg,width(f.kind));put(self,p[0],saved);break;}
    case D8:case D16:case D32:flags(p[0],p[1]);put(self,p[2],arg,width(f.kind));break;
    case Pair:{put(self,p[0],read(src,0));u32 b=read(src,4),s=read(self,p[2]);flags(p[3],p[4]);put(self,p[1],b);put(self,p[2],s);break;}
    case Dual:{put(self,p[0],read(src,0));u32 b=read(src,4);flags(p[3],p[4]);put(self,p[1],b);put(self,p[2],read(self,p[2]));
        put(self,p[5],read(src,0));u32 d=read(src,4),s=read(self,p[7]);flags(p[8],p[9]);put(self,p[6],d);put(self,p[7],s);break;}
    case Copy:{u32 v=read(src,0),s=read(self,p[0]);flags(p[1],p[2]);put(self,p[3],v);put(self,p[0],s);break;}
    case Shift:{u32 s=read(self,p[0]);flags(p[1],p[2]);u32 delta=(arg<<p[4])^read(self,p[3]);put(self,p[0],s);put(self,p[3],read(self,p[3])^(delta&p[5]));break;}
    case Mask:{u32 delta=read(self,p[3])^arg,s=read(self,p[0]);flags(p[1],p[2]);put(self,p[3],read(self,p[3])^(delta&p[4]));put(self,p[0],s);break;}
    }
}
u8 actual[65568],want[65568],external[24],external_before[24];
void properties(const Case* cases){
    for(u32 n=0;n<422;++n){const Case& f=cases[n];current=f.name;const u32 size=extent(f)+16;
        require(size+8<sizeof(actual));
        // An unaligned receiver and whole-object comparison expose unintended writes.
        u8* self=actual+5;u8* model=want+5;
        for(u32 j=0;j<512;++j){const u32 salt=random_word();fill(actual,size+8,salt);copy(want,actual,size+8);
            fill(external,sizeof(external),random_word());copy(external_before,external,sizeof(external));
            const u32 value=(random_word()&0xffffff00U)|(j&255U);
            expected(f,model,value,external+1);
            if(f.scalar)f.scalar(self,value);else f.from(self,external+1);
            require(equal(actual,want,size+8));require(equal(external,external_before,sizeof(external)));
        }
        if(f.from){u32 off[10],w[10];const u32 count=fields(f,off,w);
            const int deltas[]={-7,-4,-1,0,1,4,7};
            for(u32 field=0;field<count;++field)for(int delta:deltas){const int pos=int(off[field])+delta;
                if(pos<0 || u32(pos)+8>=size)continue;
                for(u32 j=0;j<32;++j){fill(actual,size+8,random_word());copy(want,actual,size+8);
                    expected(f,model,0,model+pos);f.from(self,self+pos);require(equal(actual,want,size+8));}
            }
        }
    }
}
struct Region {
    static constexpr u32 page=4096,bytes=0x20000;
    u8* data;
    Region():data(static_cast<u8*>(allocate(bytes))){require(data!=nullptr);}
    ~Region(){release(data,bytes);}
    Region(const Region&)=delete;Region& operator=(const Region&)=delete;
    u8* edge(){return data+bytes-page;}
    void guard(){require(protect(edge(),page,0));}
    void readonly(){require(protect(data,bytes-page,1));}
};
void guards(const Case* cases){
    for(u32 n=0;n<422;++n){const Case& f=cases[n];current=f.name;Region object,source;
        object.guard();source.guard();const u32 size=extent(f);u8* self=object.edge()-size;
        const u32 input_size=f.kind==Copy?4U:8U;u8* src=source.edge()-input_size;
        fill(self,size,random_word());copy(want,self,size);fill(src,input_size,random_word());source.readonly();
        expected(f,want,0xe971ab83U,src);
        if(f.scalar)f.scalar(self,0xe971ab83U);else f.from(self,src);
        require(equal(self,want,size));
    }
}
void run(const Case* cases,const char* label,bool p,bool g){checks=0;seed=0x8ab37621U;if(p)properties(cases);if(g)guards(cases);output("PASS ");output(label);output(" ");number(checks);output(" checks; 422 bodies\n");}
}
#if defined(RE5_FREESTANDING_I386)
extern "C" void run_entry(){run(source_cases,"i386-source",true,true);
#if defined(RE5_TEST_REFERENCE)
run(original_cases,"i386-hash-checked-reference",true,true);
#endif
}
asm(".text\n.global _start\n_start:\n xorl %ebp,%ebp\n andl $-16,%esp\n call run_entry\n xorl %ebx,%ebx\n movl $1,%eax\n int $0x80\n.section .note.GNU-stack,\"\",@progbits\n");
#else
int main(int argc,char** argv){if(argc!=2)return 2;bool p=argv[1][0]=='p',g=argv[1][0]=='g';if(!p&&!g)return 2;run(source_cases,p?"properties":"guards",p,g);return 0;}
#endif
