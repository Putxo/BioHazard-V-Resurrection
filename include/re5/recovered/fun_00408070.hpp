#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace re5::recovered {

struct FUN_00408070_String {
    std::uint32_t allocator_state;
    std::array<std::byte, 0x10> storage;
    std::uint32_t length;
    std::uint32_t capacity;
};

static_assert(offsetof(FUN_00408070_String, storage) == 0x04);
static_assert(offsetof(FUN_00408070_String, length) == 0x14);
static_assert(offsetof(FUN_00408070_String, capacity) == 0x18);
static_assert(sizeof(FUN_00408070_String) == 0x1C);

[[nodiscard]] FUN_00408070_String* FUN_00408070(
    FUN_00408070_String& value,
    std::uint32_t position,
    std::uint32_t count);

} // namespace re5::recovered
