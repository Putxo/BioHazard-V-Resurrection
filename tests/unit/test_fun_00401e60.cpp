#include "re5/recovered/fun_00401e60.hpp"

#include <cassert>
#include <cstdint>

namespace {
struct Probe {
    bool preinit = true;
    bool restart = false;
    bool steam_init = true;
    int warning_hook_calls = 0;
    int overlay_position = -1;
    int achievement_singleton_calls = 0;
    int title_calls = 0;
    const char* title = nullptr;
    const char* registry = nullptr;
    const char* last_log = nullptr;
};

bool platform_preinit(void* context) noexcept {
    return static_cast<Probe*>(context)->preinit;
}

bool steam_restart_app_if_necessary(void* context, std::uint32_t app_id) noexcept {
    assert(app_id == 0x0004F8A8U);
    return static_cast<Probe*>(context)->restart;
}

void log_message(void* context, const char* message) noexcept {
    static_cast<Probe*>(context)->last_log = message;
}

bool steam_init(void* context) noexcept {
    return static_cast<Probe*>(context)->steam_init;
}

void install_warning_hook(void* context, std::uintptr_t callback_va) noexcept {
    assert(callback_va == 0x00401210U);
    ++static_cast<Probe*>(context)->warning_hook_calls;
}

void set_overlay_notification_position(void* context, int position) noexcept {
    static_cast<Probe*>(context)->overlay_position = position;
}

std::uint32_t map_country(void*) noexcept {
    return 33;
}

void read_steam_id(void*, std::uint32_t* low, std::uint32_t* high) noexcept {
    *low = 0x11223344U;
    *high = 0x55667788U;
}

void ensure_achievement_singleton(void* context) noexcept {
    ++static_cast<Probe*>(context)->achievement_singleton_calls;
}

re5::recovered::RE5_LanguageId get_language_id(void*) noexcept {
    return re5::recovered::RE5_LanguageId::English;
}

void show_title(void* context, const char* product_name, const char* registry_name) noexcept {
    auto& probe = *static_cast<Probe*>(context);
    ++probe.title_calls;
    probe.title = product_name;
    probe.registry = registry_name;
}
} // namespace

void test_fun_00401e60() {
    using namespace re5::recovered;

    Probe probe{};
    const FUN_00401E60_Services services{
        &probe,
        &platform_preinit,
        &steam_restart_app_if_necessary,
        &log_message,
        &steam_init,
        &install_warning_hook,
        &set_overlay_notification_position,
        &map_country,
        &read_steam_id,
        &ensure_achievement_singleton,
        &get_language_id,
        &show_title,
    };

    FUN_00401E60_State state{};
    FUN_00401E60_SetServices(&services);
    assert(FUN_00401E60(state));
    assert(state.platform_preinit_ok);
    assert(state.country_id == 33);
    assert(state.steam_id_low == 0x11223344U);
    assert(state.steam_id_high == 0x55667788U);
    assert(probe.warning_hook_calls == 1);
    assert(probe.overlay_position == 3);
    assert(probe.achievement_singleton_calls == 1);
    assert(probe.title_calls == 1);

    Probe relaunch{};
    relaunch.restart = true;
    FUN_00401E60_Services relaunch_services = services;
    relaunch_services.context = &relaunch;
    FUN_00401E60_SetServices(&relaunch_services);
    assert(!FUN_00401E60(state));
    assert(relaunch.last_log != nullptr);

    Probe init_failed{};
    init_failed.steam_init = false;
    FUN_00401E60_Services init_failed_services = services;
    init_failed_services.context = &init_failed;
    FUN_00401E60_SetServices(&init_failed_services);
    assert(!FUN_00401E60(state));
    assert(init_failed.last_log != nullptr);

    FUN_00401E60_SetServices(nullptr);
}
