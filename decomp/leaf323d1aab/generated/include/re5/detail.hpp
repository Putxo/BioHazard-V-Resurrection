#pragma once
#include "re5/leaves.hpp"
namespace re5::leaf323d1aab::detail {
// Unsigned byte access permits unaligned storage and does not assume a C++ class layout.
inline u32 load8(const void* self, u32 offset) noexcept {
    return static_cast<const u8*>(self)[offset];
}
inline u32 load16(const void* self, u32 offset) noexcept {
    const auto* p = static_cast<const u8*>(self) + offset;
    return u32(p[0]) | (u32(p[1]) << 8U);
}
inline u32 load32(const void* self, u32 offset) noexcept {
    const auto* p = static_cast<const u8*>(self) + offset;
    return u32(p[0]) | (u32(p[1]) << 8U) | (u32(p[2]) << 16U) | (u32(p[3]) << 24U);
}
inline u32 sign8(const void* self, u32 offset) noexcept {
    const u32 v = load8(self, offset);
    return (v & 0x80U) != 0U ? v | 0xFFFFFF00U : v;
}
inline u32 sign16(const void* self, u32 offset) noexcept {
    const u32 v = load16(self, offset);
    return (v & 0x8000U) != 0U ? v | 0xFFFF0000U : v;
}
} // namespace re5::leaf323d1aab::detail
