#include "re5/recovered/fun_00407a30.hpp"

namespace re5::recovered {
namespace {
const FUN_00407A30_Services* g_services = nullptr;

constexpr const char* k_conversion_marker = "CONVERT GAME RESOURCE ...";
constexpr std::uintptr_t k_manager_token = 0x01657F80U;
} // namespace

void FUN_00407A30_SetServices(const FUN_00407A30_Services* services) noexcept {
    g_services = services;
}

void FUN_00407A30(FUN_00407A30_Object& object) noexcept {
    const auto* services = g_services;
    if (services == nullptr) {
        return;
    }

    const std::uint32_t state = object.state;
    if (state <= 2U) {
        if (services->emit_conversion_marker == nullptr) {
            return;
        }
        services->emit_conversion_marker(services->context, k_conversion_marker);
        ++object.state;
        return;
    }

    if (state != 3U) {
        return;
    }

    if (services->convert_game_resources == nullptr ||
        services->dispatch_manager_slot_24 == nullptr) {
        return;
    }

    services->convert_game_resources(services->context);
    services->dispatch_manager_slot_24(services->context, k_manager_token);
}

} // namespace re5::recovered
