#include "re5/recovered/fun_00401940.hpp"

namespace re5::recovered {
namespace {
const FUN_00401940_Services* g_services = nullptr;
}

void FUN_00401940_SetServices(const FUN_00401940_Services* services) noexcept {
    g_services = services;
}

void FUN_00401940(bool value) noexcept {
    const auto* services = g_services;
    if (services == nullptr || services->positive_return_state == nullptr) {
        return;
    }
    *services->positive_return_state = value;
}

} // namespace re5::recovered
