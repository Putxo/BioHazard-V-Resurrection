#pragma once
#if defined(__i386__) || defined(_M_IX86)
# if defined(_MSC_VER)
#  define RE5_PROJ_CC __thiscall
# else
#  define RE5_PROJ_CC __attribute__((thiscall))
# endif
#else
# define RE5_PROJ_CC
#endif
namespace re5::projection323d1aab {
using u8 = unsigned char;
using u32 = unsigned int;
#if defined(_MSC_VER) && !defined(__clang__)
# if defined(_WIN64)
using uptr = unsigned long long;
# else
using uptr = unsigned int;
# endif
#else
using uptr = __UINTPTR_TYPE__;
#endif
static_assert(sizeof(u8) == 1 && sizeof(u32) == 4 && sizeof(uptr) == sizeof(void*));
#define RE5_ADD(n,a,b,c) u32 RE5_PROJ_CC n(const void*) noexcept;
#define RE5_LOAD(n,a,b,w) u32 RE5_PROJ_CC n(const void*) noexcept;
#define RE5_NEST(n,t,i,c,e,f,w,m,d) u32 RE5_PROJ_CC n(const void*) noexcept;
#define RE5_SENTINEL(n,a,b) u32 RE5_PROJ_CC n(const void*) noexcept;
#define RE5_ARRAY_GET(n,c,p) u32 RE5_PROJ_CC n(const void*,u32 index) noexcept;
// In the original stack contract VALUE is argument 1 and INDEX is argument 2.
#define RE5_ARRAY_SET(n,c,p) void RE5_PROJ_CC n(void*,u32 value,u32 index) noexcept;
#define RE5_STORE(n,a,b,w) void RE5_PROJ_CC n(void*,u32 value) noexcept;
#include "records.inc"
#undef RE5_ADD
#undef RE5_LOAD
#undef RE5_NEST
#undef RE5_SENTINEL
#undef RE5_ARRAY_GET
#undef RE5_ARRAY_SET
#undef RE5_STORE
}
