#include "guarded.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/mman.h>
#include <unistd.h>
using namespace re5::guarded323d1aab;
namespace {
unsigned long long checks=0;
void require(bool ok){++checks;if(!ok){std::fprintf(stderr,"FAIL check %llu\n",checks);std::abort();}}
u32 random_word(){static u32 x=0x483A39E1U;x^=x<<13U;x^=x>>17U;x^=x<<5U;return x;}
u32 get(const void* p,u32 off){u32 x;std::memcpy(&x,static_cast<const u8*>(p)+off,4);return x;}
void put(void* p,u32 off,u32 v){std::memcpy(static_cast<u8*>(p)+off,&v,4);}
void put16(void* p,u32 off,u16 v){std::memcpy(static_cast<u8*>(p)+off,&v,2);}
struct Region {
    u8* data;std::size_t page,bytes;
    explicit Region(bool low=false):page(static_cast<std::size_t>(sysconf(_SC_PAGESIZE))),bytes(page*16U){
        void* p=mmap(nullptr,bytes,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS|(low?MAP_32BIT:0),-1,0);
        if(p==MAP_FAILED){std::perror("mmap");std::abort();}data=static_cast<u8*>(p);
        if(low)require(reinterpret_cast<uptr>(data)+bytes<=0xFFFFFFFFULL);
    }
    Region(const Region&)=delete;Region& operator=(const Region&)=delete;
    ~Region(){munmap(data,bytes);}
    u8* edge()const{return data+bytes-page;}
    void guard(){require(mprotect(edge(),page,PROT_NONE)==0);}
    void readonly(){require(mprotect(data,bytes-page,PROT_READ)==0);}
    u32 pointer(const void* p)const{uptr x=reinterpret_cast<uptr>(p);require(x<=0xFFFFFFFFULL);return u32(x);}
};
struct Store {u32 owner,test,mask,out,value;void(RE5_G_CC *call)(const void*) noexcept;};
struct Init {u32 offset,tag,table;void*(RE5_G_CDECL *call)(void*) noexcept;};
struct Bits {u32 table,index,count,entries,field,shift,mask,fallback;u32(RE5_G_CC *call)(const void*) noexcept;};
#define RE5_G_STORE(n,o,t,m,out,v) {o,t,m,out,v,&n},
#define RE5_G_INIT(...)
#define RE5_G_BITS(...)
const Store stores[]={
#include "records.inc"
};
#undef RE5_G_STORE
#undef RE5_G_INIT
#undef RE5_G_BITS
#define RE5_G_STORE(...)
#define RE5_G_INIT(n,o,t,v) {o,t,v,&n},
#define RE5_G_BITS(...)
const Init inits[]={
#include "records.inc"
};
#undef RE5_G_STORE
#undef RE5_G_INIT
#undef RE5_G_BITS
#define RE5_G_STORE(...)
#define RE5_G_INIT(...)
#define RE5_G_BITS(n,t,i,c,e,f,s,m,d) {t,i,c,e,f,s,m,d,&n},
const Bits bits[]={
#include "records.inc"
};
#undef RE5_G_STORE
#undef RE5_G_INIT
#undef RE5_G_BITS
void properties(){
    require(std::size(stores)==59 && std::size(inits)==56 && std::size(bits)==31);
    std::array<u8,32768> object{},expected{};Region block(true),owner(true);
    for(const auto& f:stores){
        require(f.owner+4U<object.size() && f.test<expected.size() && f.out<expected.size());
        object.fill(0xA5);put(object.data(),f.owner,block.pointer(block.data));
        for(unsigned flag=0;flag<256;++flag)for(unsigned n=0;n<16;++n){
            std::memset(block.data,0x6C,expected.size());block.data[f.test]=u8(flag);block.data[f.out]=u8(random_word());
            std::memcpy(expected.data(),block.data,expected.size());
            const auto before=object;
            if((expected[f.test]&f.mask)!=0U)expected[f.out]=u8(f.value);
            f.call(object.data());require(std::memcmp(block.data,expected.data(),expected.size())==0);require(object==before);
        }
    }
    for(const auto& f:inits){
        require(f.offset+2U<object.size());require(f.call(nullptr)==nullptr);
        for(unsigned i=0;i<4096;++i){
            for(unsigned j=0;j<32;++j){object[j]=u8(random_word());}
            expected=object;
            put16(expected.data(),f.offset,u16(f.tag));put(expected.data(),0,f.table);
            require(f.call(object.data())==object.data());require(object==expected);
        }
    }
    for(const auto& f:bits){
        object.fill(0xC7);put(object.data(),f.table,owner.pointer(owner.data));
        for(u32 index:{0U,1U,7U,8U,0x40000000U,0x7FFFFFFFU,0x80000000U,0xFFFFFFFFU})
        for(u32 count:{0U,1U,7U,8U,0x40000001U,0x7FFFFFFFU,0x80000000U,0xFFFFFFFFU}){
            const bool valid=(index&0x80000000U)==0U && (count&0x80000000U)==0U && index<count;
            if(valid && index>8U && index!=0x40000000U)continue;
            put(object.data(),f.index,index);put(owner.data,f.count,count);
            for(bool nonnull:{false,true})for(unsigned n=0;n<32;++n){
                const u32 value=n==0?0U:n==1?0xFFFFFFFFU:n==2?0x80000000U:random_word();
                put(block.data,f.field,value);
                if(valid)put(owner.data,u32(index*4U+f.entries),nonnull?block.pointer(block.data):0U);
                expected=object;
                u32 result=f.fallback;
                if(valid&&nonnull){const std::uint64_t divisor=std::uint64_t(1)<<(f.shift&31U);result=u32(std::uint64_t(value)/divisor)&f.mask;}
                require(f.call(object.data())==result);require(object==expected);
            }
        }
        put(object.data(),f.table,0U);put(object.data(),f.index,0xFFFFFFFFU);require(f.call(object.data())==f.fallback);
    }
}
void guards(){
    for(const auto& f:stores){
        Region block(true);block.guard();std::array<u8,32768> self{};
        const u32 extent=std::max(f.test,f.out)+1U;u8* p=block.edge()-extent;
        put(self.data(),f.owner,block.pointer(p));p[f.test]=u8(f.mask);p[f.out]=0x73U;
        // Set the tested byte last, including any possible alias between fields.
        p[f.test]=u8(f.mask);f.call(self.data());require(p[f.out]==u8(f.value));
        p[f.test]=0U;block.readonly();f.call(self.data());require(p[f.test]==0U);
        Region host;host.guard();u8* this_ptr=host.edge()-4-f.owner;put(this_ptr,f.owner,block.pointer(p));host.readonly();
        f.call(this_ptr);require(p[f.test]==0U);
    }
    for(const auto& f:inits){
        Region r;r.guard();const u32 extent=std::max(4U,f.offset+2U);u8* p=r.edge()-extent;
        std::memset(p-1,0xA5,extent+1U);require(f.call(p)==p);require(get(p,0)==f.table);
        u16 word;std::memcpy(&word,p+f.offset,2);require(word==u16(f.tag));require(p[-1]==0xA5);require(f.call(nullptr)==nullptr);
    }
    for(const auto& f:bits){
        Region host;host.guard();u8* self=host.edge()-4-f.table;put(self,f.table,0U);host.readonly();
        require(f.index>f.table);require(f.call(self)==f.fallback);
        Region owner(true);owner.guard();std::array<u8,32768> obj{};
        put(obj.data(),f.table,owner.pointer(owner.edge()));
        for(u32 index:{0x80000000U,0xFFFFFFFFU}){put(obj.data(),f.index,index);require(f.call(obj.data())==f.fallback);}
        u8* table=owner.edge()-4-f.count;put(obj.data(),f.table,owner.pointer(table));put(obj.data(),f.index,0U);
        for(u32 count:{0U,0x80000000U,0xFFFFFFFFU}){put(table,f.count,count);require(f.call(obj.data())==f.fallback);}
        put(obj.data(),f.table,owner.pointer(owner.data));put(owner.data,f.count,1U);put(owner.data,f.entries,0U);require(f.call(obj.data())==f.fallback);
        Region value(true);value.guard();u8* entry=value.edge()-4-f.field;put(entry,f.field,0xB71FA560U);value.readonly();
        put(owner.data,f.entries,value.pointer(entry));owner.readonly();
        require(f.call(obj.data())==((0xB71FA560U>>(f.shift&31U))&f.mask));
    }
}
}
int main(int argc,char** argv){
    if(argc!=2)return 2;
    if(std::strcmp(argv[1],"properties")==0)properties();
    else if(std::strcmp(argv[1],"guards")==0)guards();else return 2;
    std::printf("PASS %s: %llu checks across 146 complete functions\n",argv[1],checks);
}
