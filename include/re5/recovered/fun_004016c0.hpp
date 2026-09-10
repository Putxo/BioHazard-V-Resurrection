#pragma once

#include <cstdint>

namespace re5::recovered {

struct FUN_004016C0_Services {
    void* context;
    void (*log_invalid_index)(void* context, std::uint32_t index, std::uint32_t limit) noexcept;
    void (*touch_steam_user)(void* context) noexcept;
    void* (*get_user_stats)(void* context) noexcept;
    const char* (*get_achievement_name)(void* stats, std::uint32_t index) noexcept;
    bool (*set_achievement)(void* stats, const char* api_name) noexcept;
    bool (*store_stats)(void* stats) noexcept;
};

void FUN_004016C0_SetServices(const FUN_004016C0_Services* services) noexcept;
[[nodiscard]] bool FUN_004016C0(std::uint32_t achievement_index) noexcept;

} // namespace re5::recovered
