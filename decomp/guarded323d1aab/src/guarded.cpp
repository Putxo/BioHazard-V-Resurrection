#include "guarded.hpp"
namespace re5::guarded323d1aab {
namespace {
u32 load32(const void* self, u32 offset) noexcept {
    const auto* p=static_cast<const u8*>(self)+offset;
    return u32(p[0]) | (u32(p[1])<<8U) | (u32(p[2])<<16U) | (u32(p[3])<<24U);
}
void store16(void* self, u32 offset, u16 value) noexcept {
    auto* p=static_cast<u8*>(self)+offset;
#if defined(__GNUC__) || defined(__clang__)
    __builtin_memcpy(p,&value,2);
#else
    p[0]=u8(value);p[1]=u8(value>>8U);
#endif
}
void store32(void* self, u32 value) noexcept {
#if defined(__GNUC__) || defined(__clang__)
    __builtin_memcpy(self,&value,4);
#else
    auto* p=static_cast<u8*>(self);
    p[0]=u8(value);p[1]=u8(value>>8U);p[2]=u8(value>>16U);p[3]=u8(value>>24U);
#endif
}
void* address(u32 value) noexcept { return reinterpret_cast<void*>(uptr(value)); }
}
// Macros instantiate complete named bodies, not interpreter dispatch entries.
#define RE5_G_STORE(n,owner,test,mask,out,value) \
void RE5_G_CC n(const void* self) noexcept { \
    const u32 object=load32(self,owner); \
    if ((*static_cast<const u8*>(address(u32(object+test))) & mask)!=0U) \
        *static_cast<u8*>(address(u32(object+out)))=u8(value); \
}
#define RE5_G_INIT(n,offset,tag,table) \
void* RE5_G_CDECL n(void* object) noexcept { \
    if (object==nullptr) return nullptr; \
    store16(object,offset,u16(tag)); \
    store32(object,table); \
    return object; \
}
#define RE5_G_BITS(n,table,index,count,entries,field,shift,mask,fallback) \
u32 RE5_G_CC n(const void* self) noexcept { \
    const u32 owner=load32(self,table); \
    if (owner==0U) return fallback; \
    const u32 selected=load32(self,index); \
    if ((selected&0x80000000U)!=0U) return fallback; \
    const u32 length=load32(address(u32(owner+count)),0U); \
    if ((length&0x80000000U)!=0U || selected>=length) return fallback; \
    const u32 item=load32(address(u32(owner+selected*4U+entries)),0U); \
    if (item==0U) return fallback; \
    return (load32(address(u32(item+field)),0U)>>(shift&31U))&mask; \
}
#include "records.inc"
#undef RE5_G_STORE
#undef RE5_G_INIT
#undef RE5_G_BITS
}
