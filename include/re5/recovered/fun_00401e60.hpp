#pragma once

#include "re5/recovered/fun_00401950.hpp"

#include <cstdint>

namespace re5::recovered {

struct FUN_00401E60_State {
    bool platform_preinit_ok;
    std::uint32_t country_id;
    std::uint32_t steam_id_low;
    std::uint32_t steam_id_high;
};

struct FUN_00401E60_Services {
    void* context;
    bool (*platform_preinit)(void* context) noexcept;
    bool (*steam_restart_app_if_necessary)(void* context, std::uint32_t app_id) noexcept;
    void (*log_message)(void* context, const char* message) noexcept;
    bool (*steam_init)(void* context) noexcept;
    void (*install_warning_hook)(void* context, std::uintptr_t callback_va) noexcept;
    void (*set_overlay_notification_position)(void* context, int position) noexcept;
    std::uint32_t (*map_country)(void* context) noexcept;
    void (*read_steam_id)(void* context, std::uint32_t* low, std::uint32_t* high) noexcept;
    void (*ensure_achievement_singleton)(void* context) noexcept;
    RE5_LanguageId (*get_language_id)(void* context) noexcept;
    void (*show_title)(void* context, const char* product_name, const char* registry_name) noexcept;
};

void FUN_00401E60_SetServices(const FUN_00401E60_Services* services) noexcept;
bool FUN_00401E60(FUN_00401E60_State& state) noexcept;

} // namespace re5::recovered
