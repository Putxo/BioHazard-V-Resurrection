#include "re5/recovered/fun_00401910.hpp"

namespace re5::recovered {
namespace {
const FUN_00401910_Services* g_services = nullptr;
}

void FUN_00401910_SetServices(const FUN_00401910_Services* services) noexcept {
    g_services = services;
}

bool FUN_00401910() noexcept {
    const auto* services = g_services;
    bool probe_result = false;
    if (services != nullptr && services->monitored_object != nullptr &&
        services->probe_object != nullptr) {
        probe_result = services->probe_object(services->context, services->monitored_object);
    }

    if (services != nullptr && services->cached_state != nullptr) {
        *services->cached_state = probe_result;
    }

    if (!probe_result || services == nullptr || services->positive_return_state == nullptr) {
        return false;
    }
    return *services->positive_return_state;
}

} // namespace re5::recovered
