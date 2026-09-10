#pragma once

#include <cstdint>

namespace re5::recovered {

struct FUN_00401670_Services {
    void* context;
    void (*touch_steam_user)(void* context) noexcept;
    void* (*get_user_stats)(void* context) noexcept;
    const char* (*get_achievement_name)(void* stats, std::uint32_t index) noexcept;
    bool (*clear_achievement)(void* stats, const char* api_name) noexcept;
    bool (*store_stats)(void* stats) noexcept;
};

void FUN_00401670_SetServices(const FUN_00401670_Services* services) noexcept;
[[nodiscard]] bool FUN_00401670(std::uint32_t achievement_index) noexcept;
[[nodiscard]] const char* FUN_00401670_AchievementApiName(std::uint32_t achievement_index) noexcept;

} // namespace re5::recovered
