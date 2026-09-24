#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace re5::recovered {

inline constexpr std::uint32_t FUN_00408100_SSO_THRESHOLD = 0x10U;
inline constexpr std::uint32_t FUN_00408100_INLINE_CAPACITY = 0x0FU;

struct FUN_00408100_Object {
    std::uint32_t reserved_00_03{};
    std::array<std::byte, 0x10> storage{};
    std::uint32_t size{};
    std::uint32_t capacity{FUN_00408100_INLINE_CAPACITY};
};

static_assert(offsetof(FUN_00408100_Object, storage) == 0x04U);
static_assert(offsetof(FUN_00408100_Object, size) == 0x14U);
static_assert(offsetof(FUN_00408100_Object, capacity) == 0x18U);
static_assert(sizeof(FUN_00408100_Object) == 0x1CU);

struct FUN_00408100_Services {
    void* context;
    void (*free_aligned)(void* context, void* pointer) noexcept;
};

void FUN_00408100_SetServices(
    const FUN_00408100_Services* services) noexcept;

void FUN_00408100(
    FUN_00408100_Object& object,
    std::uint32_t requested_capacity,
    std::uint32_t preserved_size);

} // namespace re5::recovered
