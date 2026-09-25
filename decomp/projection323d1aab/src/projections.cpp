#include "projections.hpp"
namespace re5::projection323d1aab {
namespace {
template<unsigned Width> u32 load(const void* object,u32 offset) noexcept {
    static_assert(Width == 1 || Width == 2 || Width == 4);
    const auto* p = static_cast<const u8*>(object) + offset;
    u32 value = p[0];
    if constexpr (Width >= 2) value |= u32(p[1]) << 8U;
    if constexpr (Width == 4) value |= (u32(p[2]) << 16U) | (u32(p[3]) << 24U);
    return value;
}
template<unsigned Width> void store(void* object,u32 offset,u32 value) noexcept {
    static_assert(Width == 1 || Width == 4);
    auto* p = static_cast<u8*>(object) + offset;
#if defined(__GNUC__) || defined(__clang__)
    __builtin_memcpy(p,&value,Width); // Exact-width unaligned store on x86.
#else
    p[0] = u8(value);
    if constexpr (Width == 4) { p[1]=u8(value>>8U); p[2]=u8(value>>16U); p[3]=u8(value>>24U); }
#endif
}
void* address(u32 value) noexcept { return reinterpret_cast<void*>(uptr(value)); }
}
#define RE5_ADD(n,a,b,d) \
u32 RE5_PROJ_CC n(const void* self) noexcept { \
    const u32 pointer=load<4>(self,a); return pointer ? u32(pointer+b) : d; \
}
#define RE5_LOAD(n,a,b,w) \
u32 RE5_PROJ_CC n(const void* self) noexcept { \
    const u32 pointer=load<4>(self,a); \
    if(pointer==0U) return 0U; \
    return load<w>(address(u32(pointer+b)),0U); \
}
#define RE5_NEST(n,t,i,c,e,f,w,m,d) \
u32 RE5_PROJ_CC n(const void* self) noexcept { \
    const u32 owner=load<4>(self,t); if(owner==0U) return d; \
    const u32 index=load<4>(self,i); if(index & 0x80000000U) return d; \
    const u32 count=load<4>(address(u32(owner+c)),0U); \
    if((count & 0x80000000U) || index>=count) return d; \
    const u32 item=load<4>(address(u32(owner+index*4U+e)),0U); \
    if(item==0U) return d; \
    return load<w>(address(u32(item+f)),0U) & m; \
}
#define RE5_SENTINEL(n,a,b) \
u32 RE5_PROJ_CC n(const void* self) noexcept { \
    if(load<4>(self,a)==0xFFFFFFFFU) { return 0xFFFFFFFFU; } return load<4>(self,b); \
}
#define RE5_ARRAY_GET(n,c,p) \
u32 RE5_PROJ_CC n(const void* self,u32 index) noexcept { \
    if(index>=load<4>(self,c)) return 0U; \
    const u32 pointer=load<4>(self,p); return load<4>(address(u32(pointer+index*4U)),0U); \
}
#define RE5_ARRAY_SET(n,c,p) \
void RE5_PROJ_CC n(void* self,u32 value,u32 index) noexcept { \
    if(index>=load<4>(self,c)) return; \
    const u32 pointer=load<4>(self,p); store<4>(address(u32(pointer+index*4U)),0U,value); \
}
#define RE5_STORE(n,a,b,w) \
void RE5_PROJ_CC n(void* self,u32 value) noexcept { \
    const u32 pointer=load<4>(self,a); if(pointer==0U) return; \
    store<w>(address(u32(pointer+b)),0U,value); \
}
#include "records.inc"
#undef RE5_ADD
#undef RE5_LOAD
#undef RE5_NEST
#undef RE5_SENTINEL
#undef RE5_ARRAY_GET
#undef RE5_ARRAY_SET
#undef RE5_STORE
}
