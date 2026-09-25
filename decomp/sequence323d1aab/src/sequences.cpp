#include "sequences.hpp"
namespace re5::sequence323d1aab {
namespace {
void* address(u32 p) noexcept { return reinterpret_cast<void*>(uptr(p)); }
u32 load(const void* p,u32 offset=0) noexcept {
    const auto* b=static_cast<const u8*>(p)+offset;
    return u32(b[0])|(u32(b[1])<<8U)|(u32(b[2])<<16U)|(u32(b[3])<<24U);
}
void store(void* p,u32 offset,u32 value) noexcept {
    auto* b=static_cast<u8*>(p)+offset;
#if defined(__GNUC__) || defined(__clang__)
    __builtin_memcpy(b,&value,4);
#else
    b[0]=u8(value);b[1]=u8(value>>8U);b[2]=u8(value>>16U);b[3]=u8(value>>24U);
#endif
}
bool positive(u32 n) noexcept { return n!=0U && (n&0x80000000U)==0U; }
}
#define RE5_COUNT(n,head,next) \
u32 RE5_SEQ_CC n(const void* self) noexcept { \
    u32 node=load(self,head),count=0; \
    while(node!=0U){node=load(address(u32(node+next)));++count;} return count; \
}
#define RE5_AT(n,head,next) \
u32 RE5_SEQ_CC n(const void* self,u32 index) noexcept { \
    u32 node=load(self,head),position=0; \
    while(node!=0U && position!=index){node=load(address(u32(node+next)));++position;} return node; \
}
#define RE5_TAIL(n,head,next) \
u32 RE5_SEQ_CC n(const void* self) noexcept { \
    u32 node=load(self,head); if(node==0U)return 0U; \
    for(;;){const u32 following=load(address(u32(node+next)));if(following==0U)return node;node=following;} \
}
#define RE5_SUM(n,next,field) \
u32 RE5_SEQ_CC n(const void* self) noexcept { \
    u32 node=load(self,next),value=load(self,field); \
    while(node!=0U){value+=load(address(u32(node+field)));node=load(address(u32(node+next)));}return value; \
}
#define RE5_ZERO_COUNT(n) \
u32 RE5_SEQ_CC n(const void* self) noexcept { \
    u32 count=0;const auto* cursor=static_cast<const u8*>(self); \
    while(load(cursor)!=0U){cursor+=4;++count;}return count; \
}
#define RE5_FIND_KEY(n,count,pointer) \
u32 RE5_SEQ_CC n(const void* self,u32 key) noexcept { \
    const u32 size=load(self,count);if(!positive(size))return 0xFFFFFFFFU; \
    const u32 table=load(self,pointer); \
    for(u32 i=0;i<size;++i){const u32 node=load(address(u32(table+i*4U)));if(load(address(node))==key)return i;} \
    return 0xFFFFFFFFU; \
}
// The original not-found path writes table[-1]. Do not silently turn it into a no-op.
#define RE5_CLEAR(n,count,pointer) \
u32 RE5_SEQ_CC n(void* self,u32 key) noexcept { \
    const u32 size=load(self,count);u32 index=0xFFFFFFFFU; \
    if(positive(size)){u32 cursor=load(self,pointer);for(u32 i=0;i<size;++i){ \
        if(load(address(cursor))==key){index=i;break;}cursor+=4U;}} \
    const u32 table=load(self,pointer);store(address(u32(table+index*4U)),0,0U);return index; \
}
#define RE5_PROPAGATE(n,flag,count,pointer,inner,field) \
void RE5_SEQ_CC n(void* self,u8 value) noexcept { \
    auto* bytes=static_cast<u8*>(self);if(bytes[flag]==value)return; \
    const u32 size=load(self,count);bytes[flag]=value; \
    for(u32 i=0;i<size;++i){const u32 table=load(self,pointer);const u32 node=load(address(u32(table+i*4U))); \
        if(node==0U){continue;}const u32 target=load(address(u32(node+inner))); \
        if(target!=0U)*static_cast<u8*>(address(u32(target+field)))=value;} \
}
#define RE5_FIXED_STORE(n,table,count,field) \
void RE5_SEQ_CC n(void* self,u8 value) noexcept { \
    for(u32 i=0;i<count;++i){const u32 node=load(self,u32(table+i*4U)); \
        if(node!=0U)*static_cast<u8*>(address(u32(node+field)))=value;} \
}
#define RE5_FIND_NODE(n,count,pointer,field) \
u32 RE5_SEQ_CC n(const void* self,u32 key) noexcept { \
    const u32 size=load(self,count);if(size==0U)return 0U;const u32 table=load(self,pointer); \
    for(u32 i=0;i<size;++i){const u32 node=load(address(u32(table+i*4U))); \
        if(node!=0U && load(address(u32(node+field)))==key)return node;}return 0U; \
}
#define RE5_PREVIOUS(n,count,pointer) \
u32 RE5_SEQ_CC n(const void* self,u32 key) noexcept { \
    const u32 size=load(self,count);if(size==0U)return 0U;const u32 table=load(self,pointer); \
    for(u32 i=0;i<size;++i){if(load(address(u32(table+i*4U)))==key){ \
        if(i==0U){return 0U;}return load(address(u32(table+(i-1U)*4U)));}}return 0U; \
}
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
