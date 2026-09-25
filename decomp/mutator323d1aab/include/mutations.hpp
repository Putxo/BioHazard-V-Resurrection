#pragma once
#if defined(__i386__) || defined(_M_IX86)
# if defined(_MSC_VER)
#  define RE5_MUT_CC __thiscall
# else
#  define RE5_MUT_CC __attribute__((thiscall))
# endif
#else
# define RE5_MUT_CC
#endif
namespace re5::mutator323d1aab {
using u8 = unsigned char;
using u32 = unsigned int;
static_assert(sizeof(u8) == 1 && sizeof(u32) == 4);
#define RE5_S8(n,...) void RE5_MUT_CC n(void*,u32) noexcept;
#define RE5_S16 RE5_S8
#define RE5_S32 RE5_S8
#define RE5_D8 RE5_S8
#define RE5_D16 RE5_S8
#define RE5_D32 RE5_S8
#define RE5_SHIFT RE5_S8
#define RE5_MASK RE5_S8
#define RE5_PAIR(n,...) void RE5_MUT_CC n(void*,const void*) noexcept;
#define RE5_DUAL RE5_PAIR
#define RE5_COPY RE5_PAIR
#include "records.inc"
#undef RE5_S8
#undef RE5_S16
#undef RE5_S32
#undef RE5_D8
#undef RE5_D16
#undef RE5_D32
#undef RE5_SHIFT
#undef RE5_MASK
#undef RE5_PAIR
#undef RE5_DUAL
#undef RE5_COPY
}
