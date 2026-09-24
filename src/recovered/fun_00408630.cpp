#include "re5/recovered/fun_00408630.hpp"

namespace re5::recovered {
namespace {
const FUN_00408630_Services* g_services = nullptr;
} // namespace

void FUN_00408630_SetServices(
    const FUN_00408630_Services* services) noexcept {
    g_services = services;
}

void* FUN_00408630() noexcept {
    // Native body is exactly:
    //   mov eax, dword ptr [0x016E2EFC]
    //   ret
    // The service indirection models that fixed global for portable tests.
    const auto* services = g_services;
    if (services == nullptr) {
        return nullptr;
    }
    return services->global_value;
}

} // namespace re5::recovered
