#pragma once

#include <cstddef>
#include <cstdint>

namespace re5::recovered {

inline constexpr std::uint32_t
    FUN_004085F0_FIX_MASK = 0x00002000U;

struct FUN_004085F0_Object {
    std::uint32_t reserved_00{};
    std::uint32_t flags{};
};

static_assert(
    offsetof(FUN_004085F0_Object, flags) == 0x04U);
static_assert(sizeof(FUN_004085F0_Object) == 0x08U);

void FUN_004085F0(
    FUN_004085F0_Object& object,
    std::uint8_t enabled) noexcept;

} // namespace re5::recovered
