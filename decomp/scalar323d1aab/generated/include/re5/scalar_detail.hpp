#pragma once
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
