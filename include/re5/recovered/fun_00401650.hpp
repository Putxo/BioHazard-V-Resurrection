#pragma once

#include <cstdint>

namespace re5::recovered {

struct FUN_00401650_Services {
    void* context;
    void* (*get_user_stats)(void* context) noexcept;
    std::uint32_t (*invoke_slot_54)(void* user_stats, std::uint32_t argument) noexcept;
};

void FUN_00401650_SetServices(const FUN_00401650_Services* services) noexcept;
[[nodiscard]] std::uint32_t FUN_00401650(std::uint32_t argument) noexcept;

} // namespace re5::recovered
