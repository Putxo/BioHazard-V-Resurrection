#include "transfers.hpp"
#if !defined(RE5_FREESTANDING_I386)
#include <sys/mman.h>
#include <unistd.h>
#endif
using namespace re5::transfer323d1aab;
namespace {
u32 checks=0,seed=0x864acf19U;const char* current="setup";
u32 length(const char* p){u32 n=0;while(p[n])++n;return n;}
#if defined(RE5_FREESTANDING_I386)
static_assert(sizeof(void*)==4);
u32 sys3(u32 n,u32 a,u32 b,u32 c){u32 r;asm volatile("int $0x80":"=a"(r):"0"(n),"b"(a),"c"(b),"d"(c):"memory");return r;}
void output(const char* p){sys3(4,1,u32(uptr(p)),length(p));}
[[noreturn]] void quit(u32 n){sys3(1,n,0,0);__builtin_unreachable();}
void* allocate(u32 n){u32 a[]={0,n,3,0x22,0xffffffffU,0};u32 p=sys3(90,u32(uptr(a)),0,0);return p>=0xfffff001U?nullptr:reinterpret_cast<void*>(uptr(p));}
bool protect(void* p,u32 n,u32 f){return sys3(125,u32(uptr(p)),n,f)==0;}
void release(void* p,u32 n){sys3(91,u32(uptr(p)),n,0);}
#else
void output(const char* p){const auto ignored=write(1,p,length(p));(void)ignored;}
[[noreturn]] void quit(u32 n){_exit(int(n));}
void* allocate(u32 n){void* p=mmap(nullptr,n,3,MAP_PRIVATE|MAP_ANONYMOUS|MAP_32BIT,-1,0);return p==MAP_FAILED?nullptr:p;}
bool protect(void* p,u32 n,u32 f){return mprotect(p,n,int(f))==0;}
void release(void* p,u32 n){munmap(p,n);}
#endif
void number(u32 n){char s[12];u32 i=11;s[i]=0;do{s[--i]=char('0'+n%10);n/=10;}while(n);output(s+i);}
void require(bool ok){++checks;if(!ok){output("FAIL ");output(current);output(" check ");number(checks);output("\n");quit(1);}}
u32 random_word(){seed^=seed<<13U;seed^=seed>>17U;seed^=seed<<5U;return seed;}
u32 read(const void* p,u32 o=0,u32 w=4){u32 v=0;auto* b=static_cast<const u8*>(p)+o;for(u32 i=0;i<w;++i)v|=u32(b[i])<<(8*i);return v;}
void put(void* p,u32 o,u32 v,u32 w=4){auto* b=static_cast<u8*>(p)+o;for(u32 i=0;i<w;++i)b[i]=u8(v>>(8*i));}
void fill(u8* p,u32 n,u32 salt){for(u32 i=0;i<n;++i)p[i]=u8(i*31U^(i>>3U)^salt^(salt>>((i%4)*8)));}
void copy(u8* to,const u8* from,u32 n){for(u32 i=0;i<n;++i)to[i]=from[i];}
bool equal(const u8* a,const u8* b,u32 n){for(u32 i=0;i<n;++i)if(a[i]!=b[i])return false;return true;}
u32 pointer(const void* p){require(uptr(p)<=0xffffffffULL);return u32(uptr(p));}
void* address(u32 p){return reinterpret_cast<void*>(uptr(p));}
u32 mxcsr(){u32 v;asm volatile("stmxcsr %0":"=m"(v));return v;}
void mxcsr(u32 v){asm volatile("ldmxcsr %0"::"m"(v):"memory");}
struct Region {
    static constexpr u32 page=4096,bytes=0x40000;
    u8* data;
    Region():data(static_cast<u8*>(allocate(bytes))){require(data!=nullptr);require(uptr(data)+bytes<=0xffffffffULL);}
    ~Region(){release(data,bytes);}
    Region(const Region&)=delete;Region& operator=(const Region&)=delete;
    u8* edge(){return data+bytes-page;}
    void guard(){require(protect(edge(),page,0));}
    void readonly(){require(protect(data,bytes-page,1));}
};
using Setter=void(RE5_TRANSFER_CC*)(void*,u32) noexcept;
using To=void(RE5_TRANSFER_CC*)(const void*,void*) noexcept;
enum Kind{Store,Snapshot,Dirty,Indirect,Duplicate,DupZero,StoreFlag,Pair16,Pair32,Nest16};
struct Case{const char* name;Kind kind;u32 p[10];Setter set;To to;};
#if defined(RE5_TEST_REFERENCE)
extern "C" {
#define RE5_STORE(n,...) void RE5_TRANSFER_CC original_##n(void*,u32) noexcept;
#define RE5_SNAPSHOT RE5_STORE
#define RE5_DIRTY RE5_STORE
#define RE5_INDIRECT RE5_STORE
#define RE5_DUPLICATE RE5_STORE
#define RE5_DUP_ZERO RE5_STORE
#define RE5_STORE_FLAG RE5_STORE
#define RE5_PAIR16(n,...) void RE5_TRANSFER_CC original_##n(const void*,void*) noexcept;
#define RE5_PAIR32 RE5_PAIR16
#define RE5_NEST16 RE5_PAIR16
#include "records.inc"
#undef RE5_STORE
#undef RE5_SNAPSHOT
#undef RE5_DIRTY
#undef RE5_INDIRECT
#undef RE5_DUPLICATE
#undef RE5_DUP_ZERO
#undef RE5_STORE_FLAG
#undef RE5_PAIR16
#undef RE5_PAIR32
#undef RE5_NEST16
}
#endif
#define RE5_STORE(n,...) {#n,Store,{__VA_ARGS__},&FN(n),nullptr},
#define RE5_SNAPSHOT(n,...) {#n,Snapshot,{__VA_ARGS__},&FN(n),nullptr},
#define RE5_DIRTY(n,...) {#n,Dirty,{__VA_ARGS__},&FN(n),nullptr},
#define RE5_INDIRECT(n,...) {#n,Indirect,{__VA_ARGS__},&FN(n),nullptr},
#define RE5_DUPLICATE(n,...) {#n,Duplicate,{__VA_ARGS__},&FN(n),nullptr},
#define RE5_DUP_ZERO(n,...) {#n,DupZero,{__VA_ARGS__},&FN(n),nullptr},
#define RE5_STORE_FLAG(n,...) {#n,StoreFlag,{__VA_ARGS__},&FN(n),nullptr},
#define RE5_PAIR16(n,...) {#n,Pair16,{__VA_ARGS__},nullptr,&FN(n)},
#define RE5_PAIR32(n,...) {#n,Pair32,{__VA_ARGS__},nullptr,&FN(n)},
#define RE5_NEST16(n,...) {#n,Nest16,{__VA_ARGS__},nullptr,&FN(n)},
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
#undef RE5_STORE
#undef RE5_SNAPSHOT
#undef RE5_DIRTY
#undef RE5_INDIRECT
#undef RE5_DUPLICATE
#undef RE5_DUP_ZERO
#undef RE5_STORE_FLAG
#undef RE5_PAIR16
#undef RE5_PAIR32
#undef RE5_NEST16
static_assert(sizeof(source_cases)/sizeof(Case)==201);
u8 wanted[0x40000],before[0x40000];
u32 maximum(u32 a,u32 b){return a>b?a:b;}
u32 extent(const Case& f){const u32* p=f.p;switch(f.kind){
case Store:return p[0]+4;
case Snapshot:return maximum(maximum(p[0],p[1]),p[3])+4;
case Dirty:return maximum(p[0],p[2])+4;
case Indirect:return p[0]+4;
case Duplicate:return maximum(p[0],p[1])+4;
case DupZero:return maximum(maximum(p[0],p[1]),p[2])+4;
case StoreFlag:return maximum(p[0]+4,p[1]+1);
case Pair16:case Pair32:return maximum(p[0],p[1])+(f.kind==Pair16?2U:4U);
case Nest16:return maximum(p[0],p[1])+4;
}return 0;}
void expected_scalar(const Case& f,u8* self,u32 v,u8* indirect){const u32* p=f.p;
    switch(f.kind){
    case Store:put(self,p[0],v);break;
    case Snapshot:{u32 s=read(self,p[0]);put(self,p[1],read(self,p[1])|p[2]);put(self,p[3],v);put(self,p[0],s);break;}
    case Dirty:put(self,p[0],read(self,p[0])|p[1]);put(self,p[2],v);break;
    case Indirect:put(indirect,p[1],v);break;
    case Duplicate:put(self,p[0],v);put(self,p[1],v);break;
    case DupZero:put(self,p[0],v);put(self,p[1],v);put(self,p[2],0);break;
    case StoreFlag:put(self,p[0],v);self[p[1]]=u8(p[2]);break;
    default:require(false);
    }
}
const u32 special[]={0,0x80000000U,1,0x80000001U,0x007fffffU,0x00800000U,0x7f7fffffU,0x7f800000U,0xff800000U,0x7fc00000U,0x7f800001U,0xff800001U,0xffffffffU,0x3f800000U};
const u32 environments[]={0x1f80U,0x9f80U,0x1fc0U,0x9fc0U};
void properties(const Case* cases){
    Region object,out,owner,node;const u32 saved_env=mxcsr();
    for(u32 n=0;n<201;++n){const Case& f=cases[n];current=f.name;const u32* p=f.p;u8* self=object.data+5;u8* result=out.data+5;
        if(f.set){
            const u32 len=extent(f)+16;const u32 child_len=f.kind==Indirect?p[1]+20:20;
            for(u32 env:environments)for(u32 k=0;k<142;++k){const u32 v=k<14?special[k]:random_word();
                fill(self,len,random_word());fill(node.data,child_len,random_word());
                if(f.kind==Indirect)put(self,p[0],pointer(node.data));
                copy(before,self,len);copy(wanted,f.kind==Indirect?node.data:self,f.kind==Indirect?child_len:len);
                if(f.kind==Indirect)put(wanted,p[1],v);else expected_scalar(f,wanted,v,nullptr);
                mxcsr(env);f.set(self,v);require(mxcsr()==env);mxcsr(saved_env);
                require(equal(wanted,f.kind==Indirect?node.data:self,f.kind==Indirect?child_len:len));
                if(f.kind==Indirect)require(equal(self,before,len));
            }
            if(f.kind==Indirect){
                fill(self,len,123);put(self,p[0],u32(pointer(self+p[0])-p[1]));f.set(self,0x8af36c12U);require(read(self,p[0])==0x8af36c12U);
            }
        }else if(f.kind==Pair16 || f.kind==Pair32){
            const u32 w=f.kind==Pair16?2U:4U;const u32 len=extent(f)+32,outsize=maximum(p[2],p[3])+w;
            for(u32 k=0;k<128;++k){fill(self,len,random_word());copy(before,self,len);fill(result,outsize+8,random_word());copy(wanted,result,outsize+8);
                u32 a=read(self,p[0],w),b=read(self,p[1],w);put(wanted,p[2],a,w);put(wanted,p[3],b,w);f.to(self,result);
                require(equal(result,wanted,outsize+8));require(equal(self,before,len));
            }
            const int deltas[]={-7,-4,-2,-1,0,1,2,4,7};
            for(u32 field=0;field<2;++field)for(int delta:deltas){int pos=int(p[field])+delta;if(pos<0 || u32(pos)+outsize>len)continue;
                fill(self,len,random_word());copy(wanted,self,len);u32 a=read(self,p[0],w),b=read(self,p[1],w);
                put(wanted+pos,p[2],a,w);put(wanted+pos,p[3],b,w);f.to(self,self+pos);require(equal(self,wanted,len));
            }
        }else{
            const u32 indices[]={0,1,2,0x40000000U,0x7fffffffU,0x80000000U,0xffffffffU};
            const u32 counts[]={0,1,2,0x7fffffffU,0x80000000U,0xffffffffU};
            u8* table=owner.data+4096;u8* item=node.data+4096;const u32 outsize=maximum(p[6],p[7])+2;
            for(u32 has_owner=0;has_owner<2;++has_owner)for(u32 index:indices)for(u32 count:counts)for(u32 has_node=0;has_node<2;++has_node){
                put(self,p[0],has_owner?pointer(table):0);put(self,p[1],index);put(table,p[2],count);
                const bool in_range=index<0x80000000U && count<0x80000000U && index<count;
                if(in_range)put(address(u32(pointer(table)+index*4U+p[3])),0,has_node?pointer(item):0);
                put(item,p[4],random_word(),2);put(item,p[5],random_word(),2);fill(result,outsize+8,random_word());copy(wanted,result,outsize+8);
                const bool valid=has_owner && in_range && has_node;
                const u32 a=valid?read(item,p[4],2):p[8],b=valid?read(item,p[5],2):p[9];
                put(wanted,p[6],a,2);put(wanted,p[7],b,2);f.to(self,result);require(equal(result,wanted,outsize+8));
            }
            // Success output aliases its second input: both words must be read first.
            put(self,p[0],pointer(table));put(self,p[1],0);put(table,p[2],1);put(table,p[3],pointer(item));
            u8* alias=item+p[5]-p[6];put(item,p[4],0x1357,2);put(item,p[5],0x2468,2);
            const u32 first=read(item,p[4],2),second=read(item,p[5],2);f.to(self,alias);
            require(read(alias,p[6],2)==first);require(read(alias,p[7],2)==second);
        }
    }
    mxcsr(saved_env);
}
void guards(const Case* cases){
    for(u32 n=0;n<201;++n){const Case& f=cases[n];current=f.name;const u32* p=f.p;Region object,out,target;object.guard();out.guard();target.guard();
        const u32 size=extent(f);u8* self=object.edge()-size;
        if(f.set){
            fill(self,size,random_word());copy(wanted,self,size);
            if(f.kind==Indirect){u8* item=target.edge()-4-p[1];put(self,p[0],pointer(item));object.readonly();f.set(self,0xff800001U);require(read(target.edge()-4)==0xff800001U);}
            else{expected_scalar(f,wanted,0xff800001U,nullptr);f.set(self,0xff800001U);require(equal(self,wanted,size));}
        }else if(f.kind!=Nest16){
            const u32 w=f.kind==Pair16?2U:4U,outsize=maximum(p[2],p[3])+w;u8* result=out.edge()-outsize;fill(self,size,1234);
            const u32 a=read(self,p[0],w),b=read(self,p[1],w);object.readonly();f.to(self,result);require(read(result,p[2],w)==a);require(read(result,p[3],w)==b);
        }else{
            const u32 outsize=maximum(p[6],p[7])+2;u8* result=out.edge()-outsize;
            // A null owner must avoid reading the inaccessible later index.
            self=object.edge()-4-p[0];require(p[1]>p[0]);put(self,p[0],0);object.readonly();f.to(self,result);
            require(read(result,p[6],2)==p[8] && read(result,p[7],2)==p[9]);
            Region regular;self=regular.data;
            put(self,p[0],pointer(target.edge()));put(self,p[1],0xffffffffU);f.to(self,result);require(read(result,p[6],2)==p[8]);
            u8* table=target.edge()-4-p[2];put(self,p[0],pointer(table));put(self,p[1],0);put(table,p[2],0x80000000U);f.to(self,result);require(read(result,p[7],2)==p[9]);
            put(table,p[2],0);f.to(self,result);require(read(result,p[6],2)==p[8]);
            Region entries,item;entries.guard();item.guard();table=entries.edge()-4-p[3];put(self,p[0],pointer(table));put(table,p[2],1);put(table,p[3],0);f.to(self,result);require(read(result,p[7],2)==p[9]);
            u8* source=item.edge()-2-maximum(p[4],p[5]);put(source,p[4],0xa5b6,2);put(source,p[5],0xc7d8,2);put(table,p[3],pointer(source));regular.readonly();entries.readonly();item.readonly();f.to(self,result);
            require(read(result,p[6],2)==0xa5b6 && read(result,p[7],2)==0xc7d8);
        }
    }
}
void run(const Case* cases,const char* label,bool p,bool g){checks=0;seed=0x864acf19U;if(p)properties(cases);if(g)guards(cases);output("PASS ");output(label);output(" ");number(checks);output(" checks; 201 bodies\n");}
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
