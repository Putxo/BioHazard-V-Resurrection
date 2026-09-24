#pragma once

#include <cstdint>

namespace re5::recovered {

struct FUN_004085D0_Services {
    void* context;
    void* (*allocate_aligned)(
        void* context,
        std::uint32_t size,
        std::uint32_t alignment) noexcept;
};

void FUN_004085D0_SetServices(
    const FUN_004085D0_Services* services) noexcept;

[[nodiscard]] void* FUN_004085D0(std::uint32_t size) noexcept;

} // namespace re5::recovered
