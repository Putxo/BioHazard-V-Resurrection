#include "accessors.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/mman.h>
#include <unistd.h>
using namespace re5::accessor323d1aab;
namespace {
unsigned long long checks = 0;
void require(bool ok) { ++checks; if (!ok) { std::fprintf(stderr,"FAIL at check %llu\n",checks); std::abort(); } }
u32 load(const void* p, unsigned o) { u32 v; std::memcpy(&v,static_cast<const u8*>(p)+o,4); return v; }
void put(void* p, unsigned o, u32 v) { std::memcpy(static_cast<u8*>(p)+o,&v,4); }
u32 random_word() { static u32 x=0x71A48C23U; x^=x<<13U; x^=x>>17U; x^=x<<5U; return x; }
struct Region {
    u8* data; std::size_t page, bytes;
    explicit Region(bool low=false) : page(static_cast<std::size_t>(sysconf(_SC_PAGESIZE))), bytes(page*16U) {
        void* p=mmap(nullptr,bytes,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS|(low?MAP_32BIT:0),-1,0);
        if(p==MAP_FAILED) { std::perror("mmap"); std::abort(); }
        data=static_cast<u8*>(p);
        if(low) require(reinterpret_cast<uptr>(data)+bytes<=0xFFFFFFFFULL);
    }
    Region(const Region&)=delete; Region& operator=(const Region&)=delete;
    ~Region(){munmap(data,bytes);}
    u8* edge()const{return data+bytes-page;}
    void guard(){require(mprotect(edge(),page,PROT_NONE)==0);}
    void readonly(){require(mprotect(data,bytes-page,PROT_READ)==0);}
    u32 pointer(const void* p)const {const auto v=reinterpret_cast<uptr>(p);require(v<=0xFFFFFFFFULL);return u32(v);}
};
struct Flag { const char* name; unsigned offset; u32 set,clear; void(RE5_ACCESS_CC *call)(void*,u8) noexcept; };
struct Array { const char* name; unsigned count,pointer; u32(RE5_ACCESS_CC *call)(const void*,u32) noexcept; };
struct Nested {const char* name; unsigned table,index,count,entries,field; u32(RE5_ACCESS_CC *call)(const void*) noexcept;};
#define RE5_FLAG(n,o,s,c) {#n,o,s,c,&n},
#define RE5_ARRAY(...)
#define RE5_NESTED(...)
const Flag flags[]={
#include "records.inc"
};
#undef RE5_FLAG
#undef RE5_ARRAY
#undef RE5_NESTED
#define RE5_FLAG(...)
#define RE5_ARRAY(n,c,p) {#n,c,p,&n},
#define RE5_NESTED(...)
const Array arrays[]={
#include "records.inc"
};
#undef RE5_FLAG
#undef RE5_ARRAY
#undef RE5_NESTED
#define RE5_FLAG(...)
#define RE5_ARRAY(...)
#define RE5_NESTED(n,t,i,c,e,f) {#n,t,i,c,e,f,&n},
const Nested nested[]={
#include "records.inc"
};
#undef RE5_FLAG
#undef RE5_ARRAY
#undef RE5_NESTED
void properties(){
    require(std::size(flags)==39 && std::size(arrays)==39 && std::size(nested)==49);
    std::array<u8,16384> object{},expected{};
    for(const auto& f:flags){
        object.fill(0xA5);
        for(unsigned enabled=0;enabled<256;++enabled)for(unsigned n=0;n<32;++n){
            const u32 old=n==0?0U:n==1?0xFFFFFFFFU:random_word();
            put(object.data(),f.offset,old);expected=object;
            put(expected.data(),f.offset,enabled?(old|f.set):(old&f.clear));
            f.call(object.data(),u8(enabled));
            require(object==expected);
        }
    }
    Region table(true),item(true);
    for(const auto& f:arrays){
        object.fill(0xB6);put(object.data(),f.pointer,table.pointer(table.data));
        for(unsigned i=0;i<64;++i)put(table.data,4*i,random_word());
        for(u32 length:{0U,1U,2U,31U,64U})for(u32 i=0;i<=65;++i){
            put(object.data(),f.count,length);expected=object;
            const u32 want=i<length?load(table.data,4*i):0U;
            require(f.call(object.data(),i)==want);require(object==expected);
        }
        put(object.data(),f.count,0xFFFFFFFFU);
        for(u32 i:{0x40000000U,0x80000000U,0xC0000000U})require(f.call(object.data(),i)==load(table.data,0));
        require(f.call(object.data(),0xFFFFFFFFU)==0U);
    }
    const std::array<u32,9> indices={0U,1U,7U,8U,0x3FFFFFFFU,0x40000000U,0x7FFFFFFFU,0x80000000U,0xFFFFFFFFU};
    const std::array<u32,8> lengths={0U,1U,7U,8U,0x40000001U,0x7FFFFFFFU,0x80000000U,0xFFFFFFFFU};
    for(const auto& f:nested){
        object.fill(0xC7);put(object.data(),f.table,table.pointer(table.data));
        for(u32 i:indices)for(u32 count:lengths){
            const bool in_range=std::int32_t(i)>=0 && std::int32_t(i)<std::int32_t(count);
            // Arrange table base to make 32-bit wrapped indexing select a safe low mapping.
            const u32 anchor=table.pointer(table.data+16384);
            // The count still has to reside in this mapping; far indices are covered where multiplication wraps.
            if(in_range && i!=0x40000000U && i>8U)continue;
            const u32 owner=(in_range && i==0x40000000U)?anchor-f.entries:table.pointer(table.data);
            put(object.data(),f.table,owner);put(object.data(),f.index,i);
            put(reinterpret_cast<void*>(uptr(owner)),f.count,count);
            for(bool nonnull:{false,true}){
                const u32 value=random_word();put(item.data,f.field,value);
                if(in_range)put(reinterpret_cast<void*>(uptr(u32(owner+i*4U+f.entries))),0,nonnull?item.pointer(item.data):0U);
                expected=object;
                require(f.call(object.data())==(in_range&&nonnull?value:0U));require(object==expected);
            }
        }
        put(object.data(),f.table,0U);put(object.data(),f.index,0xFFFFFFFFU);require(f.call(object.data())==0U);
    }
}
void guards(){
    for(const auto& f:flags){
        Region r;r.guard();u8* self=r.edge()-4-f.offset;put(self,f.offset,0xF0F0F0F0U);
        self[f.offset-1]=0xA5;
        f.call(self,0U);require(load(self,f.offset)==(0xF0F0F0F0U&f.clear));require(self[f.offset-1]==0xA5);
        f.call(self,255U);require(load(self,f.offset)==((0xF0F0F0F0U&f.clear)|f.set));require(self[f.offset-1]==0xA5);
    }
    for(const auto& f:arrays){
        Region r;r.guard();
        u8* self=f.pointer>f.count?r.edge()-4-f.count:r.data+r.page-f.count;
        put(self,f.count,7U);r.readonly();
        if(f.pointer<f.count)require(mprotect(r.data,r.page,PROT_NONE)==0);
        require(f.pointer!=f.count);require(f.call(self,7U)==0U);require(f.call(self,0xFFFFFFFFU)==0U);
        Region data(true);data.guard();put(data.edge()-4,0,0x91C30A7FU);data.readonly();
        std::array<u8,16384> obj{};put(obj.data(),f.count,1);put(obj.data(),f.pointer,data.pointer(data.edge()-4));
        require(f.call(obj.data(),0)==0x91C30A7FU);
    }
    for(const auto& f:nested){
        Region r;r.guard();u8* self=r.edge()-4-f.table;put(self,f.table,0);r.readonly();
        require(f.index>f.table);require(f.call(self)==0U); // index is inaccessible
        Region owner(true);owner.guard();std::array<u8,16384> obj{};
        put(obj.data(),f.table,owner.pointer(owner.edge()));put(obj.data(),f.index,0x80000000U);
        require(f.call(obj.data())==0U);put(obj.data(),f.index,0xFFFFFFFFU);require(f.call(obj.data())==0U);
        // Count at final readable DWORD; out-of-range path must not touch entries.
        u8* table=owner.edge()-4-f.count;put(obj.data(),f.table,owner.pointer(table));
        put(table,f.count,0);put(obj.data(),f.index,0);require(f.call(obj.data())==0U);
        put(table,f.count,0xFFFFFFFFU);require(f.call(obj.data())==0U);
        // Null entry returns without touching an object; final field access is exactly one DWORD.
        put(obj.data(),f.table,owner.pointer(owner.data));put(owner.data,f.count,1);put(owner.data,f.entries,0);
        require(f.call(obj.data())==0U);
        Region item(true);item.guard();u8* node=item.edge()-4-f.field;put(node,f.field,0xDFC54E83U);item.readonly();
        put(owner.data,f.entries,item.pointer(node));owner.readonly();
        require(f.call(obj.data())==0xDFC54E83U);
    }
}
}
int main(int argc,char** argv){
    if(argc!=2)return 2;
    if(std::strcmp(argv[1],"properties")==0)properties();
    else if(std::strcmp(argv[1],"guards")==0)guards();else return 2;
    std::printf("PASS %s: %llu checks; 127 complete function bodies\n",argv[1],checks);
}
