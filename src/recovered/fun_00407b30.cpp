#include "re5/recovered/fun_00407b30.hpp"

namespace re5::recovered {
namespace {
const FUN_00407B30_Services* g_services = nullptr;
} // namespace

void FUN_00407B30_SetServices(const FUN_00407B30_Services* services) noexcept {
    g_services = services;
}

void FUN_00407B30(FUN_00407B30_Object& object) noexcept {
    const auto* services = g_services;
    if (services == nullptr) {
        return;
    }

    bool should_leave = object.active_flag != 0U;
    if (!should_leave) {
        if (services->read_global_force_flag == nullptr) {
            return;
        }
        should_leave = services->read_global_force_flag(services->context) != 0U;
    }

    if (!should_leave) {
        return;
    }

    if (services->leave_critical_section == nullptr) {
        return;
    }

    services->leave_critical_section(
        services->context,
        static_cast<void*>(object.critical_section));
}

} // namespace re5::recovered
