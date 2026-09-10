#include "re5/recovered/fun_004016c0.hpp"
#include "re5/recovered/fun_00401670.hpp"

namespace re5::recovered {
namespace {
const FUN_004016C0_Services* g_services = nullptr;
constexpr std::uint32_t kAchievementLimit = 0x47U;
}

void FUN_004016C0_SetServices(const FUN_004016C0_Services* services) noexcept {
    g_services = services;
}

bool FUN_004016C0(std::uint32_t achievement_index) noexcept {
    const auto* services = g_services;
    if (services == nullptr || services->log_invalid_index == nullptr ||
        services->touch_steam_user == nullptr || services->get_user_stats == nullptr ||
        services->get_achievement_name == nullptr || services->set_achievement == nullptr ||
        services->store_stats == nullptr) {
        return false;
    }

    if (achievement_index >= kAchievementLimit) {
        services->log_invalid_index(services->context, achievement_index, kAchievementLimit);
        return false;
    }

    services->touch_steam_user(services->context);
    void* stats = services->get_user_stats(services->context);
    if (stats == nullptr) {
        return false;
    }

    (void)services->get_achievement_name(stats, achievement_index);

    const char* api_name = FUN_00401670_AchievementApiName(achievement_index);
    if (api_name == nullptr || !services->set_achievement(stats, api_name)) {
        return false;
    }

    return services->store_stats(stats);
}

} // namespace re5::recovered
