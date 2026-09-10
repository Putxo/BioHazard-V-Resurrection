#include "re5/recovered/fun_00401860.hpp"

namespace re5::recovered {
namespace {
const FUN_00401860_Services* g_services = nullptr;
}

void FUN_00401860_SetServices(const FUN_00401860_Services* services) noexcept {
    g_services = services;
}

bool FUN_00401860(void* object) noexcept {
    if (object == nullptr) {
        return false;
    }

    const auto* services = g_services;
    if (services == nullptr || services->invoke_slot_04 == nullptr) {
        return false;
    }

    return services->invoke_slot_04(object);
}

} // namespace re5::recovered
