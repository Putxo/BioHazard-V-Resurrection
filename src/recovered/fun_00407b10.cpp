#include "re5/recovered/fun_00407b10.hpp"

namespace re5::recovered {
namespace {
const FUN_00407B10_Services* g_services = nullptr;
} // namespace

void FUN_00407B10_SetServices(const FUN_00407B10_Services* services) noexcept {
    g_services = services;
}

void FUN_00407B10(FUN_00407B10_Object& object) noexcept {
    const auto* services = g_services;
    if (services == nullptr) {
        return;
    }

    bool should_enter = object.active_flag != 0U;
    if (!should_enter) {
        if (services->read_global_force_flag == nullptr) {
            return;
        }
        should_enter = services->read_global_force_flag(services->context) != 0U;
    }

    if (!should_enter) {
        return;
    }

    if (services->enter_critical_section == nullptr) {
        return;
    }

    services->enter_critical_section(
        services->context,
        static_cast<void*>(object.critical_section));
}

} // namespace re5::recovered
