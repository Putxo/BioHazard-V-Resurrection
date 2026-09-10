#include "re5/recovered/fun_00401670.hpp"

#include <array>

namespace re5::recovered {
namespace {
const FUN_00401670_Services* g_services = nullptr;

constexpr std::array<const char*, 71> kAchievementApiNames{{
    "Nothing. GFWL starts counting at 1",
    "ACH_CHAPTER11_CLEAR", "ACH_CHAPTER12_CLEAR", "ACH_CHAPTER21_CLEAR",
    "ACH_CHAPTER22_CLEAR", "ACH_CHAPTER23_CLEAR", "ACH_CHAPTER31_CLEAR",
    "ACH_CHAPTER32_CLEAR", "ACH_CHAPTER33_CLEAR", "ACH_CHAPTER41_CLEAR",
    "ACH_CHAPTER42_CLEAR", "ACH_CHAPTER51_CLEAR", "ACH_CHAPTER52_CLEAR",
    "ACH_CHAPTER53_CLEAR", "ACH_CHAPTER61_CLEAR", "ACH_CHAPTER62_CLEAR",
    "ACH_CHAPTER63_CLEAR", "ACH_AMATEUR_PLAYER", "ACH_NORMAL_PLAYER",
    "ACH_PROFESSIONAL_PLAYER", "ACH_AGENT_PLAYER", "ACH_EGG_COMPLETE",
    "ACH_COSTUME_COMPLETE", "ACH_WEAPON_COMPLETE", "ACH_UPGRADE_COMPLETE",
    "ACH_TREASURE_COMPLETE", "ACH_COIN_COMPLETE", "ACH_FIGURE_COMPLETE",
    "ACH_HELPER", "ACH_LIFESAVER", "ACH_CRITICAL_SNIPE", "ACH_KNIFE_MANIA",
    "ACH_THUNDERBOLT", "ACH_GATLING_SOLDIER", "ACH_BOW_SOLDIER",
    "ACH_MATERIAL_COMBAT", "ACH_COMBO_ATTACK", "ACH_ASCENSION", "ACH_HOTSHOT",
    "ACH_INSIGHT", "ACH_MEAT_SHOWER", "ACH_WEAK_POINT", "ACH_ELECTRO_SHOCK",
    "ACH_SEA_OF_FIRE", "ACH_EXPLOSION", "ACH_STAND_PLAY", "ACH_FATE",
    "ACH_CRASH_MAKER", "ACH_KILLER_SMELL", "ACH_HEART_ATTACK", "ACH_TRUE_TRUST",
    "ACH_SLAYERS", "ACH_SURVIVORS", "ACH_TEAM_SLAYERS", "ACH_TEAM_SURVIVORS",
    "ACH_COMBO_MASTER", "ACH_COMBO_MASTER_DUO", "ACH_SCORE_ATTACK",
    "ACH_SCORE_ATTACK_DUO", "ACH_CHARACTER_COMPLETE", "ACH_VAIR_KNUCKLE",
    "ACH_LN_CLEAR", "ACH_LN_CLEAR_S", "ACH_LN_CLEAR_PRO", "ACH_WESKERS_FURY",
    "ACH_SCORE_HANTER", "ACH_DE_CLEAR", "ACH_DE_CLEAR_S", "ACH_DE_CLEAR_PRO",
    "ACH_BATTLE_MASTER", "ACH_HEAD_HUNTING"
}};
} // namespace

void FUN_00401670_SetServices(const FUN_00401670_Services* services) noexcept {
    g_services = services;
}

const char* FUN_00401670_AchievementApiName(std::uint32_t achievement_index) noexcept {
    if (achievement_index >= kAchievementApiNames.size()) {
        return nullptr;
    }
    return kAchievementApiNames[achievement_index];
}

bool FUN_00401670(std::uint32_t achievement_index) noexcept {
    const auto* services = g_services;
    if (services == nullptr || services->touch_steam_user == nullptr ||
        services->get_user_stats == nullptr || services->get_achievement_name == nullptr ||
        services->clear_achievement == nullptr || services->store_stats == nullptr) {
        return false;
    }

    services->touch_steam_user(services->context);
    void* stats = services->get_user_stats(services->context);
    if (stats == nullptr) {
        return false;
    }

    (void)services->get_achievement_name(stats, achievement_index);

    const char* api_name = FUN_00401670_AchievementApiName(achievement_index);
    if (api_name == nullptr || !services->clear_achievement(stats, api_name)) {
        return false;
    }

    return services->store_stats(stats);
}

} // namespace re5::recovered
