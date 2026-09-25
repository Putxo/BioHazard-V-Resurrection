#pragma once
#if defined(__i386__) || defined(_M_IX86)
# if defined(_MSC_VER)
#  define RE5_TRANSFER_CC __thiscall
# else
#  define RE5_TRANSFER_CC __attribute__((thiscall))
# endif
#else
# define RE5_TRANSFER_CC
#endif
namespace re5::transfer323d1aab {
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
// Scalar arguments represent the original four stack bytes, not numeric conversions.
#define RE5_STORE(n,...) void RE5_TRANSFER_CC n(void*,u32) noexcept;
#define RE5_SNAPSHOT RE5_STORE
#define RE5_DIRTY RE5_STORE
#define RE5_INDIRECT RE5_STORE
#define RE5_DUPLICATE RE5_STORE
#define RE5_DUP_ZERO RE5_STORE
#define RE5_STORE_FLAG RE5_STORE
#define RE5_PAIR16(n,...) void RE5_TRANSFER_CC n(const void*,void*) noexcept;
#define RE5_PAIR32 RE5_PAIR16
#define RE5_NEST16 RE5_PAIR16
#include "records.inc"
#undef RE5_STORE
#undef RE5_SNAPSHOT
#undef RE5_DIRTY
#undef RE5_INDIRECT
#undef RE5_DUPLICATE
#undef RE5_DUP_ZERO
#undef RE5_STORE_FLAG
#undef RE5_PAIR16
#undef RE5_PAIR32
#undef RE5_NEST16
}
