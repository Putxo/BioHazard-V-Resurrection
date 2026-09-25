#pragma once
#if defined(__i386__) || defined(_M_IX86)
# if defined(_MSC_VER)
#  define RE5_SEQ_CC __thiscall
# else
#  define RE5_SEQ_CC __attribute__((thiscall))
# endif
#else
# define RE5_SEQ_CC
#endif
namespace re5::sequence323d1aab {
using u8=unsigned char;
using u32=unsigned int;
#if defined(_MSC_VER) && !defined(__clang__)
# if defined(_WIN64)
using uptr=unsigned long long;
# else
using uptr=unsigned int;
# endif
#else
using uptr=__UINTPTR_TYPE__;
#endif
static_assert(sizeof(u8)==1 && sizeof(u32)==4 && sizeof(uptr)==sizeof(void*));
#define RE5_COUNT(n,...) u32 RE5_SEQ_CC n(const void*) noexcept;
#define RE5_AT(n,...) u32 RE5_SEQ_CC n(const void*,u32) noexcept;
#define RE5_TAIL(n,...) u32 RE5_SEQ_CC n(const void*) noexcept;
#define RE5_SUM(n,...) u32 RE5_SEQ_CC n(const void*) noexcept;
#define RE5_ZERO_COUNT(n) u32 RE5_SEQ_CC n(const void*) noexcept;
#define RE5_FIND_KEY(n,...) u32 RE5_SEQ_CC n(const void*,u32) noexcept;
#define RE5_CLEAR(n,...) u32 RE5_SEQ_CC n(void*,u32) noexcept;
#define RE5_PROPAGATE(n,...) void RE5_SEQ_CC n(void*,u8) noexcept;
#define RE5_FIXED_STORE(n,...) void RE5_SEQ_CC n(void*,u8) noexcept;
#define RE5_FIND_NODE(n,...) u32 RE5_SEQ_CC n(const void*,u32) noexcept;
#define RE5_PREVIOUS(n,...) u32 RE5_SEQ_CC n(const void*,u32) noexcept;
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
