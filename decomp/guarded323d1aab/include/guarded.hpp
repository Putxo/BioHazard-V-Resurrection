#pragma once
#if defined(__i386__) || defined(_M_IX86)
# if defined(_MSC_VER)
#  define RE5_G_CC __thiscall
#  define RE5_G_CDECL __cdecl
# else
#  define RE5_G_CC __attribute__((thiscall))
#  define RE5_G_CDECL __attribute__((cdecl))
# endif
#else
# define RE5_G_CC
# define RE5_G_CDECL
#endif
namespace re5::guarded323d1aab {
using u8 = unsigned char;
using u16 = unsigned short;
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
static_assert(sizeof(u8)==1 && sizeof(u16)==2 && sizeof(u32)==4 && sizeof(uptr)==sizeof(void*));
#define RE5_G_STORE(n,owner,test,mask,out,value) void RE5_G_CC n(const void*) noexcept;
#define RE5_G_INIT(n,offset,tag,table) void* RE5_G_CDECL n(void*) noexcept;
#define RE5_G_BITS(n,table,index,count,entries,field,shift,mask,fallback) u32 RE5_G_CC n(const void*) noexcept;
#include "records.inc"
#undef RE5_G_STORE
#undef RE5_G_INIT
#undef RE5_G_BITS
}
