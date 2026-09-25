#pragma once
// Reconstructed only from SHA-256 323d1aab...658e67815.
// No system/game headers, no executable embedding, no fixed-address calls.
// Names describe inferred views; they are not recovered original class names.

#if defined(_MSC_VER) && defined(_M_IX86)
#  define RE5_MEMBERCALL __thiscall
#elif defined(__i386__) && (defined(__GNUC__) || defined(__clang__))
#  define RE5_MEMBERCALL __attribute__((thiscall))
#else
#  define RE5_MEMBERCALL
#endif

namespace re5::recovered {
using byte = unsigned char;
using u32 = unsigned int;
using s32 = int;
static_assert(static_cast<byte>(255u) == 255u && static_cast<byte>(256u) == 0u &&
              sizeof(u32) == 4 && sizeof(s32) == 4,
              "These reconstructions require 32-bit int and 8-bit bytes.");

// Original view: [this+0] is a pointer to storage. Storage begins with two
// little-endian 32-bit fields, then bytes at +8. Ownership is not implemented
// by these read-only routines. A non-null storage pointer must be readable.
struct RawStringObject {
    const byte* storage;

    // Compare ARGUMENT against stored bytes (not the other way around).
    // Returns -1, 0, +1; does not use the cached length. Argument cannot be null.
    s32 RE5_MEMBERCALL FUN_004022F0(const byte* argument) const noexcept;
    u32 RE5_MEMBERCALL FUN_00402350() const noexcept;
    const byte* RE5_MEMBERCALL FUN_00402420() const noexcept;
};

// Observed byte layout only: the interpretation as RGBA/BGRA is not assumed.
struct FourByteObject {
    byte fields[4];
    FourByteObject* RE5_MEMBERCALL FUN_004022A0(u32 arg1, u32 arg2,
                                              u32 arg3, u32 arg4) noexcept;
};
static_assert(sizeof(FourByteObject) == 4, "Original object writes cover 4 bytes.");
#if defined(__i386__) || defined(_M_IX86)
static_assert(sizeof(RawStringObject) == 4, "Original x86 object view is 4 bytes.");
#endif
} // namespace re5::recovered
