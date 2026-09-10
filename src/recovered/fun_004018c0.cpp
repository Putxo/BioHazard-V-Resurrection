#include "re5/recovered/fun_004018c0.hpp"

namespace re5::recovered {
namespace {
const FUN_004018C0_Services* g_services = nullptr;
}

void FUN_004018C0_SetServices(const FUN_004018C0_Services* services) noexcept {
    g_services = services;
}

bool FUN_004018C0() noexcept {
    const auto* services = g_services;
    bool result = false;
    if (services != nullptr && services->monitored_object != nullptr &&
        services->cached_state != nullptr && services->probe_object != nullptr) {
        result = services->probe_object(services->context, services->monitored_object);
    }

    if (services != nullptr && services->cached_state != nullptr) {
        *services->cached_state = result;
    }
    return result;
}

} // namespace re5::recovered
