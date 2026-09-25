#include "accessors.hpp"
namespace re5::accessor323d1aab {
namespace {
u32 load32(const void* object, u32 offset) noexcept {
    const auto* p = static_cast<const u8*>(object) + offset;
    return u32(p[0]) | (u32(p[1]) << 8U) | (u32(p[2]) << 16U) | (u32(p[3]) << 24U);
}
void store32(void* object, u32 offset, u32 value) noexcept {
    auto* p = static_cast<u8*>(object) + offset;
#if defined(__GNUC__) || defined(__clang__)
    __builtin_memcpy(p, &value, 4);
#else
    p[0] = u8(value); p[1] = u8(value >> 8U);
    p[2] = u8(value >> 16U); p[3] = u8(value >> 24U);
#endif
}
// Original memory references contain 32-bit addresses, even on the host harness.
const void* address(u32 value) noexcept { return reinterpret_cast<const void*>(uptr(value)); }
}
// Each row expands to a separately named complete function, not a dispatch stub.
#define RE5_FLAG(name, offset, set_mask, clear_mask) \
void RE5_ACCESS_CC name(void* self, u8 enabled) noexcept { \
    const u32 previous = load32(self, offset); \
    store32(self, offset, enabled != 0U ? previous | set_mask : previous & clear_mask); \
}
#define RE5_ARRAY(name, count, pointer) \
u32 RE5_ACCESS_CC name(const void* self, u32 index) noexcept { \
    if (index >= load32(self, count)) return 0U; \
    const u32 data = load32(self, pointer); \
    return load32(address(u32(data + index * 4U)), 0U); \
}
#define RE5_NESTED(name, table, index, count, entries, field) \
u32 RE5_ACCESS_CC name(const void* self) noexcept { \
    const u32 owner = load32(self, table); \
    if (owner == 0U) return 0U; \
    const u32 selected = load32(self, index); \
    if ((selected & 0x80000000U) != 0U) return 0U; \
    const u32 length = load32(address(u32(owner + count)), 0U); \
    if ((length & 0x80000000U) != 0U || selected >= length) return 0U; \
    const u32 item = load32(address(u32(owner + selected * 4U + entries)), 0U); \
    if (item == 0U) return 0U; \
    return load32(address(u32(item + field)), 0U); \
}
#include "records.inc"
#undef RE5_FLAG
#undef RE5_ARRAY
#undef RE5_NESTED
}
