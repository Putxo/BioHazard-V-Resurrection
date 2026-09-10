#pragma once

#include <cstdint>

namespace re5::recovered {

struct FUN_004017B0_Services {
    void* context;
    void (*get_seed8)(void* context, std::uint8_t* out_seed8) noexcept;
};

void FUN_004017B0_SetServices(const FUN_004017B0_Services* services) noexcept;
[[nodiscard]] std::uint32_t FUN_004017B0(
    std::uint8_t* destination,
    std::uint32_t destination_size,
    const std::uint8_t* source,
    std::uint32_t length) noexcept;

} // namespace re5::recovered
