#include "re5/recovered/fun_00401e60.hpp"

namespace re5::recovered {
namespace {
constexpr std::uint32_t kResidentEvil5AppId = 0x0004F8A8U;
constexpr std::uintptr_t kSteamWarningHookCallback = 0x00401210U;

const FUN_00401E60_Services* g_services = nullptr;

const char* title_for_language(RE5_LanguageId language) noexcept {
    if (language == RE5_LanguageId::Japanese) {
        return "BIOHAZARD 5";
    }
    return "RESIDENT EVIL 5";
}
} // namespace

void FUN_00401E60_SetServices(const FUN_00401E60_Services* services) noexcept {
    g_services = services;
}

bool FUN_00401E60(FUN_00401E60_State& state) noexcept {
    const auto* services = g_services;
    const bool preinit_ok = services != nullptr && services->platform_preinit != nullptr &&
                            services->platform_preinit(services->context);
    state.platform_preinit_ok = preinit_ok;

    if (!preinit_ok) {
        if (services != nullptr && services->show_title != nullptr) {
            services->show_title(
                services->context,
                "BIOHAZARD 5",
                "CAPCOM\\BIOHAZARD 5");
        }
        return true;
    }

    if (services != nullptr && services->steam_restart_app_if_necessary != nullptr &&
        services->steam_restart_app_if_necessary(services->context, kResidentEvil5AppId)) {
        if (services->log_message != nullptr) {
            services->log_message(services->context, "Steam will relaunch the game now.");
        }
        return false;
    }

    if (services == nullptr || services->steam_init == nullptr ||
        !services->steam_init(services->context)) {
        if (services != nullptr && services->log_message != nullptr) {
            services->log_message(
                services->context,
                "Steam must be running to play this game (SteamAPI_Init() failed).");
        }
        return false;
    }

    if (services->install_warning_hook != nullptr) {
        services->install_warning_hook(services->context, kSteamWarningHookCallback);
    }
    if (services->set_overlay_notification_position != nullptr) {
        services->set_overlay_notification_position(services->context, 3);
    }
    if (services->map_country != nullptr) {
        state.country_id = services->map_country(services->context);
    }
    if (services->read_steam_id != nullptr) {
        services->read_steam_id(services->context, &state.steam_id_low, &state.steam_id_high);
    }
    if (services->ensure_achievement_singleton != nullptr) {
        services->ensure_achievement_singleton(services->context);
    }

    RE5_LanguageId language = RE5_LanguageId::TraditionalChinese;
    if (services->get_language_id != nullptr) {
        language = services->get_language_id(services->context);
    }
    if (services->show_title != nullptr) {
        services->show_title(
            services->context,
            title_for_language(language),
            "CAPCOM\\RESIDENT EVIL 5");
    }
    return true;
}

} // namespace re5::recovered
