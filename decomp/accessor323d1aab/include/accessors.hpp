#pragma once
#if defined(__i386__) || defined(_M_IX86)
# if defined(_MSC_VER)
#  define RE5_ACCESS_CC __thiscall
# else
#  define RE5_ACCESS_CC __attribute__((thiscall))
# endif
#else
# define RE5_ACCESS_CC
#endif
namespace re5::accessor323d1aab {
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
#define RE5_FLAG(name, offset, set_mask, clear_mask) void RE5_ACCESS_CC name(void*, u8) noexcept;
#define RE5_ARRAY(name, count, pointer) u32 RE5_ACCESS_CC name(const void*, u32) noexcept;
#define RE5_NESTED(name, table, index, count, entries, field) u32 RE5_ACCESS_CC name(const void*) noexcept;
#include "records.inc"
#undef RE5_FLAG
#undef RE5_ARRAY
#undef RE5_NESTED
}
