#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace re5::recovered {

struct FUN_004082E0_String {
    std::uint32_t allocator_state;
    std::array<std::byte, 0x10> storage;
    std::uint32_t length;
    std::uint32_t capacity;
};

static_assert(offsetof(FUN_004082E0_String, storage) == 0x04);
static_assert(offsetof(FUN_004082E0_String, length) == 0x14);
static_assert(offsetof(FUN_004082E0_String, capacity) == 0x18);
static_assert(sizeof(FUN_004082E0_String) == 0x1C);

struct FUN_004082E0_Services {
    void* context;
    bool (*grow)(
        void* context,
        FUN_004082E0_String& value,
        std::uint32_t required_length,
        std::uint32_t preserved_length) noexcept;
};

void FUN_004082E0_SetServices(
    const FUN_004082E0_Services* services) noexcept;

[[nodiscard]] FUN_004082E0_String* FUN_004082E0(
    FUN_004082E0_String& destination,
    const FUN_004082E0_String& source,
    std::uint32_t position,
    std::uint32_t count);

} // namespace re5::recovered
