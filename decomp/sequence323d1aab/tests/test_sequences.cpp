#include "sequences.hpp"
#if !defined(RE5_FREESTANDING_I386)
#include <sys/mman.h>
#include <unistd.h>
#endif
using namespace re5::sequence323d1aab;
namespace {
u32 checks=0,seed=0x69572a31U;const char* current="setup";
u32 length(const char* s){u32 n=0;while(s[n])++n;return n;}
#if defined(RE5_FREESTANDING_I386)
static_assert(sizeof(void*)==4);
u32 sys3(u32 n,u32 a,u32 b,u32 c){u32 r;asm volatile("int $0x80":"=a"(r):"0"(n),"b"(a),"c"(b),"d"(c):"memory");return r;}
void output(const char* s){sys3(4,1,u32(uptr(s)),length(s));}
[[noreturn]] void quit(u32 n){sys3(1,n,0,0);__builtin_unreachable();}
void* allocate(u32 n){u32 args[]={0,n,3,0x22,0xffffffffU,0};u32 p=sys3(90,u32(uptr(args)),0,0);return p>=0xfffff001U?nullptr:reinterpret_cast<void*>(uptr(p));}
bool protect(void* p,u32 n,u32 flags){return sys3(125,u32(uptr(p)),n,flags)==0;}
void release(void* p,u32 n){sys3(91,u32(uptr(p)),n,0);}
#else
void output(const char* s){const auto ignored=write(1,s,length(s));(void)ignored;}
[[noreturn]] void quit(u32 n){_exit(int(n));}
void* allocate(u32 n){void* p=mmap(nullptr,n,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS|MAP_32BIT,-1,0);return p==MAP_FAILED?nullptr:p;}
bool protect(void* p,u32 n,u32 flags){return mprotect(p,n,int(flags))==0;}
void release(void* p,u32 n){munmap(p,n);}
#endif
void number(u32 n){char s[12];u32 i=11;s[i]=0;do{s[--i]=char('0'+n%10);n/=10;}while(n);output(s+i);}
void require(bool ok){++checks;if(!ok){output("FAIL ");output(current);output(" check ");number(checks);output("\n");quit(1);}}
u32 random_word(){seed^=seed<<13U;seed^=seed>>17U;seed^=seed<<5U;return seed;}
u32 word(const void* p,u32 o=0){const u8* b=static_cast<const u8*>(p)+o;return u32(b[0])|(u32(b[1])<<8U)|(u32(b[2])<<16U)|(u32(b[3])<<24U);}
void put(void* p,u32 o,u32 v){u8* b=static_cast<u8*>(p)+o;b[0]=u8(v);b[1]=u8(v>>8U);b[2]=u8(v>>16U);b[3]=u8(v>>24U);}
u32 pointer(const void* p){const uptr v=reinterpret_cast<uptr>(p);require(v<=0xffffffffULL);return u32(v);}
void* address(u32 p){return reinterpret_cast<void*>(uptr(p));}
struct Region {
    static constexpr u32 page=4096,bytes=0x20000;
    u8* data;
    Region():data(static_cast<u8*>(allocate(bytes))){require(data!=nullptr);require(uptr(data)+bytes<=0xffffffffULL);}
    Region(const Region&)=delete;Region& operator=(const Region&)=delete;
    ~Region(){release(data,bytes);}
    u8* edge(){return data+bytes-page;}
    void guard(){require(protect(edge(),page,0));}
    void readonly(){require(protect(data,bytes-page,1));}
};
using Read=u32(RE5_SEQ_CC*)(const void*) noexcept;
using ArgRead=u32(RE5_SEQ_CC*)(const void*,u32) noexcept;
using Clear=u32(RE5_SEQ_CC*)(void*,u32) noexcept;
using Write=void(RE5_SEQ_CC*)(void*,u8) noexcept;
enum Kind{Count,At,Tail,Sum,ZeroCount,FindKey,ClearKey,Propagate,FixedStore,FindNode,Previous};
struct Case{const char* name;Kind kind;u32 p[5];Read read;ArgRead arg;Clear clear;Write write;};
#if defined(RE5_TEST_REFERENCE)
extern "C" {
#define RE5_COUNT(n,...) u32 RE5_SEQ_CC original_##n(const void*) noexcept;
#define RE5_AT(n,...) u32 RE5_SEQ_CC original_##n(const void*,u32) noexcept;
#define RE5_TAIL(n,...) u32 RE5_SEQ_CC original_##n(const void*) noexcept;
#define RE5_SUM(n,...) u32 RE5_SEQ_CC original_##n(const void*) noexcept;
#define RE5_ZERO_COUNT(n) u32 RE5_SEQ_CC original_##n(const void*) noexcept;
#define RE5_FIND_KEY(n,...) u32 RE5_SEQ_CC original_##n(const void*,u32) noexcept;
#define RE5_CLEAR(n,...) u32 RE5_SEQ_CC original_##n(void*,u32) noexcept;
#define RE5_PROPAGATE(n,...) void RE5_SEQ_CC original_##n(void*,u8) noexcept;
#define RE5_FIXED_STORE(n,...) void RE5_SEQ_CC original_##n(void*,u8) noexcept;
#define RE5_FIND_NODE(n,...) u32 RE5_SEQ_CC original_##n(const void*,u32) noexcept;
#define RE5_PREVIOUS(n,...) u32 RE5_SEQ_CC original_##n(const void*,u32) noexcept;
#include "records.inc"
#undef RE5_COUNT
#undef RE5_AT
#undef RE5_TAIL
#undef RE5_SUM
#undef RE5_ZERO_COUNT
#undef RE5_FIND_KEY
#undef RE5_CLEAR
#undef RE5_PROPAGATE
#undef RE5_FIXED_STORE
#undef RE5_FIND_NODE
#undef RE5_PREVIOUS
}
#endif
#define RE5_COUNT(n,...) {#n,Count,{__VA_ARGS__},&FN(n),nullptr,nullptr,nullptr},
#define RE5_AT(n,...) {#n,At,{__VA_ARGS__},nullptr,&FN(n),nullptr,nullptr},
#define RE5_TAIL(n,...) {#n,Tail,{__VA_ARGS__},&FN(n),nullptr,nullptr,nullptr},
#define RE5_SUM(n,...) {#n,Sum,{__VA_ARGS__},&FN(n),nullptr,nullptr,nullptr},
#define RE5_ZERO_COUNT(n) {#n,ZeroCount,{},&FN(n),nullptr,nullptr,nullptr},
#define RE5_FIND_KEY(n,...) {#n,FindKey,{__VA_ARGS__},nullptr,&FN(n),nullptr,nullptr},
#define RE5_CLEAR(n,...) {#n,ClearKey,{__VA_ARGS__},nullptr,nullptr,&FN(n),nullptr},
#define RE5_PROPAGATE(n,...) {#n,Propagate,{__VA_ARGS__},nullptr,nullptr,nullptr,&FN(n)},
#define RE5_FIXED_STORE(n,...) {#n,FixedStore,{__VA_ARGS__},nullptr,nullptr,nullptr,&FN(n)},
#define RE5_FIND_NODE(n,...) {#n,FindNode,{__VA_ARGS__},nullptr,&FN(n),nullptr,nullptr},
#define RE5_PREVIOUS(n,...) {#n,Previous,{__VA_ARGS__},nullptr,&FN(n),nullptr,nullptr},
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
#undef RE5_COUNT
#undef RE5_AT
#undef RE5_TAIL
#undef RE5_SUM
#undef RE5_ZERO_COUNT
#undef RE5_FIND_KEY
#undef RE5_CLEAR
#undef RE5_PROPAGATE
#undef RE5_FIXED_STORE
#undef RE5_FIND_NODE
#undef RE5_PREVIOUS
static_assert(sizeof(source_cases)/sizeof(Case)==42);
constexpr u32 negative_counts[]={0x80000000U,0xffffffffU};
void properties(const Case* cases){
    Region object,table,nodes,targets;
    for(u32 n=0;n<42;++n){const Case& f=cases[n];current=f.name;const u32* p=f.p;
        u8* self=object.data;u8* slots=table.data+64;
        if(f.kind==Count || f.kind==At || f.kind==Tail || f.kind==Sum){
            for(u32 size=0;size<=32;++size){u32 total=0xfffffff0U;put(self,p[0],size?pointer(nodes.data):0);
                if(f.kind==Sum)put(self,p[1],total);
                for(u32 i=0;i<size;++i){u8* node=nodes.data+64*i;u32 value=random_word();total+=value;
                    const u32 next=f.kind==Sum?p[0]:p[1];put(node,next,i+1<size?pointer(node+64):0);
                    if(f.kind==Sum)put(node,p[1],value);
                }
                if(f.kind==Count)require(f.read(self)==size);
                if(f.kind==Tail)require(f.read(self)==(size?pointer(nodes.data+64*(size-1)):0));
                if(f.kind==Sum)require(f.read(self)==total);
                if(f.kind==At){for(u32 i=0;i<=size+1;++i)require(f.arg(self,i)==(i<size?pointer(nodes.data+64*i):0));require(f.arg(self,0xffffffffU)==0);}
            }
        }else if(f.kind==ZeroCount){
            for(u32 size=0;size<=96;++size){for(u32 i=0;i<size;++i)put(slots,i*4,random_word()|1U);put(slots,size*4,0);require(f.read(slots)==size);}
        }else if(f.kind==FindKey || f.kind==FindNode || f.kind==Previous || f.kind==ClearKey){
            for(u32 size=0;size<=16;++size)for(u32 q=0;q<=18;++q){
                put(self,p[0],size);put(self,p[1],pointer(slots));put(slots-4,0,0x6b97cdaeU);
                const u32 key=100U+q;u32 expected=f.kind==FindKey || f.kind==ClearKey?0xffffffffU:0U;
                for(u32 i=0;i<16;++i){const u32 v=100U+i/2U;u8* node=nodes.data+64*i;
                    if(f.kind==FindKey){put(slots,4*i,pointer(node));put(node,0,v);}
                    else if(f.kind==FindNode){put(slots,4*i,i%3?pointer(node):0);put(node,p[2],v);}
                    else put(slots,4*i,v);
                }
                bool found=false;
                for(u32 i=0;i<size;++i){u32 v=word(slots,4*i);if(f.kind==FindKey)v=word(address(v));
                    else if(f.kind==FindNode){if(!v)continue;v=word(address(v),p[2]);}
                    if(v==key){found=true;expected=f.kind==FindNode?word(slots,4*i):f.kind==Previous?(i?word(slots,4*(i-1)):0):i;break;}
                }
                const u32 got=f.kind==ClearKey?f.clear(self,key):f.arg(self,key);require(got==expected);
                if(f.kind==ClearKey){require(word(slots-4)==(found?0x6b97cdaeU:0U));for(u32 i=0;i<16;++i)require(word(slots,4*i)==(found&&i==expected?0U:100U+i/2U));}
                require(word(self,p[0])==size && word(self,p[1])==pointer(slots));
            }
            if(f.kind==FindKey || f.kind==ClearKey){for(u32 size:negative_counts){put(self,p[0],size);put(slots-4,0,0xaabbccddU);
                require((f.kind==ClearKey?f.clear(self,123):f.arg(self,123))==0xffffffffU);if(f.kind==ClearKey)require(word(slots-4)==0);}}
            if(f.kind==FindNode || f.kind==Previous){ // Unsigned high-bit counts must still enter the loop.
                put(self,p[0],0x80000000U);put(self,p[1],pointer(slots));put(slots,0,f.kind==FindNode?pointer(nodes.data):123);
                put(nodes.data,p[2],123);require(f.arg(self,123)==(f.kind==FindNode?pointer(nodes.data):0U));}
        }else if(f.kind==Propagate){
            for(u32 size=0;size<=12;++size)for(u32 v=0;v<256;++v){
                const u8 value=u8(v);const bool same=v%4==0;self[p[0]]=same?value:u8(value^0x80U);put(self,p[1],size);put(self,p[2],pointer(slots));
                for(u32 i=0;i<12;++i){u8* node=nodes.data+64*i;u8* target=targets.data+64*i;
                    put(slots,4*i,i%4==0?0U:pointer(node));put(node,p[3],i%4==1?0U:pointer(target));
                    target[p[4]-1]=0xa5;target[p[4]]=u8(value^0x55U);target[p[4]+1]=0x5a;
                }
                f.write(self,value);require(self[p[0]]==value);require(word(self,p[1])==size);
                for(u32 i=0;i<12;++i){u8* target=targets.data+64*i;const bool changed=!same && i<size && i%4>=2;
                    require(target[p[4]]==(changed?value:u8(value^0x55U)));require(target[p[4]-1]==0xa5 && target[p[4]+1]==0x5a);}
            }
            // Count is captured before the first child write: changing it must not lengthen the loop.
            self[p[0]]=0;put(self,p[1],2);put(self,p[2],pointer(slots));put(slots,0,pointer(nodes.data));put(slots,4,0);
            put(nodes.data,p[3],u32(pointer(self+p[1])-p[4]));f.write(self,3);require(word(self,p[1])==3);
            // A child write changes the table pointer's low byte. It must be reloaded for the next item.
            u8* base=table.data+4096;u8* other=base+64;self[p[0]]=0;put(self,p[1],2);put(self,p[2],pointer(base));
            put(base,0,pointer(nodes.data));put(base,4,0);put(other,4,pointer(nodes.data+64));
            put(nodes.data,p[3],u32(pointer(self+p[2])-p[4]));put(nodes.data+64,p[3],pointer(targets.data));targets.data[p[4]]=0;
            f.write(self,64);require(word(self,p[2])==pointer(other));require(targets.data[p[4]]==64);
        }else if(f.kind==FixedStore){
            for(u32 value=0;value<256;value+=17){for(u32 i=0;i<p[1];++i){u8* node=nodes.data+64*i;put(self,p[0]+4*i,i%3==0?0U:pointer(node));
                    node[p[2]-1]=0xa5;node[p[2]]=0x13;node[p[2]+1]=0x5a;}
                f.write(self,u8(value));for(u32 i=0;i<p[1];++i){u8* node=nodes.data+64*i;
                    require(node[p[2]]==(i%3==0?0x13:u8(value)));require(node[p[2]-1]==0xa5 && node[p[2]+1]==0x5a);}
            }
        }
    }
}
void guards(const Case* cases){
    for(u32 n=0;n<42;++n){const Case& f=cases[n];current=f.name;const u32* p=f.p;
        if(f.kind==Count || f.kind==At || f.kind==Tail){
            Region object,target;object.guard();target.guard();u8* self=object.edge()-4-p[0];put(self,p[0],0);
            if(f.kind==At)require(f.arg(self,0xffffffffU)==0);else require(f.read(self)==0);
            if(f.kind==At){put(self,p[0],pointer(target.edge()));require(f.arg(self,0)==pointer(target.edge()));}
            u8* node=target.edge()-4-p[1];put(node,p[1],0);put(self,p[0],pointer(node));object.readonly();target.readonly();
            if(f.kind==At){require(f.arg(self,0)==pointer(node));require(f.arg(self,1)==0);}else require(f.read(self)==(f.kind==Count?1U:pointer(node)));
        }else if(f.kind==Sum){
            Region object,target;object.guard();target.guard();u8* self=object.edge()-4-p[1];u8* node=target.edge()-4-p[1];
            put(self,p[0],0);put(self,p[1],0x13572468U);require(f.read(self)==0x13572468U);
            put(self,p[0],pointer(node));put(node,p[0],0);put(node,p[1],0xfedcba98U);object.readonly();target.readonly();
            require(f.read(self)==u32(0x13572468U+0xfedcba98U));
        }else if(f.kind==ZeroCount){
            Region r;r.guard();u8* self=r.edge()-4;put(self,0,0);r.readonly();require(f.read(self)==0);
        }else if(f.kind==FindKey || f.kind==FindNode || f.kind==Previous || f.kind==ClearKey){
            Region object,table,node;object.guard();table.guard();node.guard();u8* self=object.data;
            if(f.kind!=ClearKey){
                self=p[1]>p[0]?object.edge()-4-p[0]:object.data+Region::page-p[0];put(self,p[0],0);object.readonly();
                if(p[1]<p[0]){require(protect(object.data,Region::page,0));}require(f.arg(self,7)==(f.kind==FindKey?0xffffffffU:0U));
            }else{
                put(self,p[0],0x80000000U);put(self,p[1],pointer(table.edge()));put(table.edge()-4,0,0xabbacdcdU);object.readonly();
                require(f.clear(self,7)==0xffffffffU);require(word(table.edge()-4)==0);
            }
            Region self2;self=self2.data;u8* slots=table.edge()-4;u8* item=node.edge()-4-(f.kind==FindNode?p[2]:0U);
            put(self,p[0],2);put(self,p[1],pointer(slots));put(slots,0,f.kind==FindKey||f.kind==FindNode?pointer(item):7U);put(item,f.kind==FindNode?p[2]:0U,7);
            self2.readonly();node.readonly();
            if(f.kind==ClearKey){require(f.clear(self,7)==0);require(word(slots)==0);}
            else{table.readonly();require(f.arg(self,7)==(f.kind==FindKey?0U:f.kind==FindNode?pointer(item):0U));}
        }else if(f.kind==Propagate){
            Region object,table,node,target;object.guard();table.guard();node.guard();target.guard();u8* self=object.data+Region::page-p[0];
            self[p[0]]=127;object.readonly();require(p[0]>p[1]);require(protect(object.data,Region::page,0));f.write(self,127);require(true);
            Region obj2;self=obj2.data;self[p[0]]=0;put(self,p[1],0);put(self,p[2],pointer(table.edge()));f.write(self,1);require(self[p[0]]==1);
            self[p[0]]=0;put(self,p[1],1);put(self,p[2],pointer(table.edge()-4));put(table.edge()-4,0,0);f.write(self,2);require(self[p[0]]==2);
            u8* item=node.edge()-4-p[3];put(item,p[3],0);put(table.edge()-4,0,pointer(item));f.write(self,3);require(self[p[0]]==3);
            put(item,p[3],u32(pointer(target.edge()-1)-p[4]));table.readonly();node.readonly();f.write(self,4);require(target.edge()[-1]==4);
            // Captured count must stay two even when a child changes the count field to three.
            Region obj3,slots3,child3;slots3.guard();self=obj3.data;self[p[0]]=0;put(self,p[1],2);
            put(self,p[2],pointer(slots3.edge()-8));put(slots3.edge()-8,0,pointer(child3.data));put(slots3.edge()-4,0,0);
            put(child3.data,p[3],u32(pointer(self+p[1])-p[4]));slots3.readonly();child3.readonly();
            f.write(self,3);require(word(self,p[1])==3);
        }else if(f.kind==FixedStore){
            Region object,node;object.guard();node.guard();u8* self=object.edge()-p[0]-4*p[1];
            for(u32 i=0;i<p[1];++i){put(self,p[0]+4*i,0);}put(self,p[0]+4*(p[1]-1),u32(pointer(node.edge()-1)-p[2]));
            object.readonly();f.write(self,255);require(node.edge()[-1]==255);
        }
    }
}
void run(const Case* cases,const char* label,bool properties_enabled,bool guards_enabled){checks=0;seed=0x69572a31U;if(properties_enabled)properties(cases);if(guards_enabled)guards(cases);output("PASS ");output(label);output(" ");number(checks);output(" checks; 42 bodies\n");}
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
