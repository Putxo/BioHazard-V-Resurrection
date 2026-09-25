#include "projections.hpp"
#if !defined(RE5_FREESTANDING_I386)
#include <sys/mman.h>
#include <unistd.h>
#endif
using namespace re5::projection323d1aab;
namespace {
u32 checks=0,seed=0x194E718BU;
const char* current="setup";
u32 length(const char* s){u32 n=0;while(s[n])++n;return n;}
#if defined(RE5_FREESTANDING_I386)
static_assert(sizeof(void*)==4);
u32 sys3(u32 n,u32 a,u32 b,u32 c){u32 result;asm volatile("int $0x80":"=a"(result):"0"(n),"b"(a),"c"(b),"d"(c):"memory");return result;}
void output(const char* s){sys3(4,1,u32(uptr(s)),length(s));}
[[noreturn]] void quit(u32 code){sys3(1,code,0,0);__builtin_unreachable();}
void* allocate(u32 bytes){u32 args[]={0,bytes,3,0x22,0xffffffffU,0};const u32 p=sys3(90,u32(uptr(args)),0,0);return p>=0xfffff001U?nullptr:reinterpret_cast<void*>(uptr(p));}
bool protect(void* p,u32 n,u32 perm){return sys3(125,u32(uptr(p)),n,perm)==0;}
void release(void* p,u32 n){sys3(91,u32(uptr(p)),n,0);}
#else
void output(const char* s){const auto ignored=write(1,s,length(s));(void)ignored;}
[[noreturn]] void quit(u32 code){_exit(int(code));}
void* allocate(u32 bytes){void* p=mmap(nullptr,bytes,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS|MAP_32BIT,-1,0);return p==MAP_FAILED?nullptr:p;}
bool protect(void* p,u32 n,u32 perm){return mprotect(p,n,int(perm))==0;}
void release(void* p,u32 n){munmap(p,n);}
#endif
void number(u32 n){char b[12];u32 i=11;b[i]=0;do{b[--i]=char('0'+n%10);n/=10;}while(n);output(b+i);}
void require(bool ok){++checks;if(!ok){output("FAIL ");output(current);output(" check ");number(checks);output("\n");quit(1);}}
u32 random_word(){seed^=seed<<13U;seed^=seed>>17U;seed^=seed<<5U;return seed;}
u32 word(const void* p,u32 o=0,u32 width=4){u32 v=0;const u8* q=static_cast<const u8*>(p)+o;for(u32 i=0;i<width;++i)v|=u32(q[i])<<(8*i);return v;}
void put(void* p,u32 o,u32 v,u32 width=4){u8* q=static_cast<u8*>(p)+o;for(u32 i=0;i<width;++i)q[i]=u8(v>>(8*i));}
u32 pointer(const void* p){const uptr v=reinterpret_cast<uptr>(p);require(v<=0xffffffffULL);return u32(v);}
void* address(u32 p){return reinterpret_cast<void*>(uptr(p));}
struct Region{
    static constexpr u32 page=4096,bytes=0x50000;
    u8* data;
    Region():data(static_cast<u8*>(allocate(bytes))){require(data!=nullptr);require(uptr(data)+bytes<=0xffffffffULL);}
    ~Region(){release(data,bytes);}
    Region(const Region&)=delete;Region& operator=(const Region&)=delete;
    u8* edge(){return data+bytes-page;}
    void guard(){require(protect(edge(),page,0));}
    void readonly(){require(protect(data,bytes-page,1));}
};
using Read=u32(RE5_PROJ_CC*)(const void*) noexcept;
using Get=u32(RE5_PROJ_CC*)(const void*,u32) noexcept;
using Set=void(RE5_PROJ_CC*)(void*,u32,u32) noexcept;
using Store=void(RE5_PROJ_CC*)(void*,u32) noexcept;
enum Kind{Add,Load,Nest,Sentinel,ArrayGet,ArraySet,StoreValue};
struct Case{const char* name;Kind kind;u32 p[8];Read read;Get get;Set set;Store store;};
#if defined(RE5_TEST_REFERENCE)
extern "C" {
#define RE5_ADD(n,a,b,c) u32 RE5_PROJ_CC original_##n(const void*) noexcept;
#define RE5_LOAD(n,a,b,w) u32 RE5_PROJ_CC original_##n(const void*) noexcept;
#define RE5_NEST(n,t,i,c,e,f,w,m,d) u32 RE5_PROJ_CC original_##n(const void*) noexcept;
#define RE5_SENTINEL(n,a,b) u32 RE5_PROJ_CC original_##n(const void*) noexcept;
#define RE5_ARRAY_GET(n,c,p) u32 RE5_PROJ_CC original_##n(const void*,u32) noexcept;
#define RE5_ARRAY_SET(n,c,p) void RE5_PROJ_CC original_##n(void*,u32,u32) noexcept;
#define RE5_STORE(n,a,b,w) void RE5_PROJ_CC original_##n(void*,u32) noexcept;
#include "records.inc"
#undef RE5_ADD
#undef RE5_LOAD
#undef RE5_NEST
#undef RE5_SENTINEL
#undef RE5_ARRAY_GET
#undef RE5_ARRAY_SET
#undef RE5_STORE
}
#endif
#define RE5_ADD(n,a,b,d) {#n,Add,{a,b,d},&FN(n),nullptr,nullptr,nullptr},
#define RE5_LOAD(n,a,b,w) {#n,Load,{a,b,w},&FN(n),nullptr,nullptr,nullptr},
#define RE5_NEST(n,t,i,c,e,f,w,m,d) {#n,Nest,{t,i,c,e,f,w,m,d},&FN(n),nullptr,nullptr,nullptr},
#define RE5_SENTINEL(n,a,b) {#n,Sentinel,{a,b},&FN(n),nullptr,nullptr,nullptr},
#define RE5_ARRAY_GET(n,c,p) {#n,ArrayGet,{c,p},nullptr,&FN(n),nullptr,nullptr},
#define RE5_ARRAY_SET(n,c,p) {#n,ArraySet,{c,p},nullptr,nullptr,&FN(n),nullptr},
#define RE5_STORE(n,a,b,w) {#n,StoreValue,{a,b,w},nullptr,nullptr,nullptr,&FN(n)},
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
#undef RE5_ADD
#undef RE5_LOAD
#undef RE5_NEST
#undef RE5_SENTINEL
#undef RE5_ARRAY_GET
#undef RE5_ARRAY_SET
#undef RE5_STORE
static_assert(sizeof(source_cases)/sizeof(Case)==196);
const u32 indices[]={0,1,7,8,15,16,0x3fffffffU,0x40000000U,0x40000001U,0x7fffffffU,0x80000000U,0xc0000000U,0xffffffffU};
const u32 counts[]={0,1,7,8,16,0x40000001U,0x7fffffffU,0x80000000U,0xffffffffU};
void properties(const Case* cases){
    Region object,table,item;
    for(u32 n=0;n<196;++n){const Case& f=cases[n];current=f.name;const u32* p=f.p;u8* self=object.data;u8* node=item.data+4096;
        if(f.kind==Add){
            for(u32 v:indices){put(self,p[0],v);require(f.read(self)==(v?u32(v+p[1]):p[2]));require(word(self,p[0])==v);}
            put(self,p[0],0xfffffff8U);require(f.read(self)==u32(0xfffffff8U+p[1]));
        }else if(f.kind==Load || f.kind==StoreValue){
            put(self,p[0],0);if(f.kind==Load)require(f.read(self)==0);else f.store(self,0xffffffffU);
            put(self,p[0],pointer(node));
            for(u32 k=0;k<512;++k){const u32 value=random_word();put(node,p[1]-1,0xa5,1);put(node,p[1]+p[2],0x5a,1);
                if(f.kind==Load){put(node,p[1],value,p[2]);require(f.read(self)==(value&(p[2]==2?65535U:0xffffffffU)));}
                else{put(node,p[1],0,p[2]);f.store(self,value);require(word(node,p[1],p[2])==(value&(p[2]==1?255U:0xffffffffU)));}
                require(word(node,p[1]-1,1)==0xa5 && word(node,p[1]+p[2],1)==0x5a);require(word(self,p[0])==pointer(node));
            }
            if(f.kind==StoreValue){ // Destination may alias the stored pointer field.
                put(self,p[0],u32(pointer(self+p[0])-p[1]));f.store(self,0x52791ac3U);
                require(word(self,p[0],p[2])==(p[2]==1?0xc3U:0x52791ac3U));
            }
        }else if(f.kind==Sentinel){
            for(u32 k=0;k<512;++k){const u32 a=k%2?0xffffffffU:random_word(),v=random_word();put(self,p[0],a);put(self,p[1],v);
                require(f.read(self)==(a==0xffffffffU?a:v));require(word(self,p[0])==a && word(self,p[1])==v);}
        }else if(f.kind==ArrayGet || f.kind==ArraySet){
            const u32 base=pointer(node);put(self,p[1],base);
            for(u32 count:counts)for(u32 index:indices){put(self,p[0],count);const u32 va=u32(base+index*4U);u8* slot=static_cast<u8*>(address(va));
                put(slot,0,0x1973acefU);slot[-1]=0x5a;slot[4]=0xa5;const u32 v=random_word();
                if(f.kind==ArrayGet)require(f.get(self,index)==(index<count?0x1973acefU:0U));
                else{f.set(self,v,index);require(word(slot)==(index<count?v:0x1973acefU));}
                require(slot[-1]==0x5a && slot[4]==0xa5);require(word(self,p[0])==count && word(self,p[1])==base);
            }
        }else if(f.kind==Nest){
            put(self,p[0],0);put(self,p[1],0xffffffffU);require(f.read(self)==p[7]);
            const u32 base=pointer(table.data+4096);put(self,p[0],base);
            for(u32 count:counts)for(u32 index:indices)for(u32 nonnull=0;nonnull<2;++nonnull){
                put(self,p[1],index);put(address(base),p[2],count);const bool valid=(index<0x80000000U && count<0x80000000U && index<count);
                const u32 v=random_word();put(node,p[4],v,p[5]);
                if(valid)put(address(u32(base+index*4U+p[3])),0,nonnull?pointer(node):0);
                const u32 expected=(valid&&nonnull)?(v & (p[5]==1?255U:0xffffffffU) & p[6]):p[7];
                require(f.read(self)==expected);require(word(self,p[0])==base && word(self,p[1])==index);
            }
        }
    }
}
void guards(const Case* cases){
    for(u32 n=0;n<196;++n){const Case& f=cases[n];current=f.name;const u32* p=f.p;
        if(f.kind==Add || f.kind==Load || f.kind==StoreValue){
            Region r;r.guard();u8* self=r.edge()-4-p[0];put(self,p[0],0);r.readonly();
            if(f.kind==Add)require(f.read(self)==p[2]);else if(f.kind==Load)require(f.read(self)==0);else{f.store(self,0xffffffffU);require(true);}
            if(f.kind!=Add){Region obj,target;target.guard();u8* node=target.edge()-p[2]-p[1];put(obj.data,p[0],pointer(node));put(node,p[1],0xdf913b7aU,p[2]);obj.readonly();
                if(f.kind==Load){target.readonly();require(f.read(obj.data)==(p[2]==2?0x3b7aU:0xdf913b7aU));}
                else{f.store(obj.data,0x74831ce6U);require(word(node,p[1],p[2])==(p[2]==1?0xe6U:0x74831ce6U));}
            }
        }else if(f.kind==Sentinel){
            require(p[1]>p[0]);Region r;r.guard();u8* self=r.edge()-4-p[0];put(self,p[0],0xffffffffU);r.readonly();require(f.read(self)==0xffffffffU);
        }else if(f.kind==ArrayGet || f.kind==ArraySet){
            Region r;r.guard();u8* self=p[1]>p[0]?r.edge()-4-p[0]:r.data+Region::page-p[0];put(self,p[0],7);r.readonly();
            if(p[1]<p[0])require(protect(r.data,Region::page,0));
            if(f.kind==ArrayGet){require(f.get(self,7)==0);require(f.get(self,0xffffffffU)==0);}
            else{f.set(self,0,7);f.set(self,0,0xffffffffU);require(true);}
            Region object,target;target.guard();put(object.data,p[0],1);put(object.data,p[1],pointer(target.edge()-4));object.readonly();put(target.edge()-4,0,0xc5937241U);
            if(f.kind==ArrayGet){target.readonly();require(f.get(object.data,0)==0xc5937241U);}
            else{f.set(object.data,0x1837dca9U,0);require(word(target.edge()-4)==0x1837dca9U);}
        }else if(f.kind==Nest){
            Region r;r.guard();u8* self=r.edge()-4-p[0];put(self,p[0],0);r.readonly();require(p[1]>p[0]);require(f.read(self)==p[7]);
            Region obj,owner,item;owner.guard();put(obj.data,p[0],pointer(owner.edge()));put(obj.data,p[1],0xffffffffU);require(f.read(obj.data)==p[7]);
            u8* table=owner.edge()-4-p[2];put(obj.data,p[0],pointer(table));put(obj.data,p[1],0);put(table,p[2],0);require(f.read(obj.data)==p[7]);
            put(table,p[2],0x80000000U);require(f.read(obj.data)==p[7]);
            put(obj.data,p[0],pointer(owner.data));put(owner.data,p[2],1);put(owner.data,p[3],0);require(f.read(obj.data)==p[7]);
            item.guard();u8* node=item.edge()-p[5]-p[4];put(node,p[4],0x8ac76d35U,p[5]);put(owner.data,p[3],pointer(node));item.readonly();owner.readonly();obj.readonly();
            require(f.read(obj.data)==(0x8ac76d35U & (p[5]==1?255U:0xffffffffU) & p[6]));
        }
    }
}
void run(const Case* cases,const char* label,bool do_properties,bool do_guards){checks=0;seed=0x194e718bU;if(do_properties)properties(cases);if(do_guards)guards(cases);output("PASS ");output(label);output(" ");number(checks);output(" checks; 196 bodies\n");}
}
#if defined(RE5_FREESTANDING_I386)
extern "C" void run_entry(){
    run(source_cases,"i386-source",true,true);
#if defined(RE5_TEST_REFERENCE)
    run(original_cases,"i386-hash-checked-reference",true,true);
#endif
}
asm(".text\n.global _start\n_start:\n xorl %ebp,%ebp\n andl $-16,%esp\n call run_entry\n xorl %ebx,%ebx\n movl $1,%eax\n int $0x80\n.section .note.GNU-stack,\"\",@progbits\n");
#else
int main(int argc,char** argv){if(argc!=2)return 2;const bool p=argv[1][0]=='p',g=argv[1][0]=='g';if(!p&&!g)return 2;run(source_cases,p?"properties":"guards",p,g);return 0;}
#endif
