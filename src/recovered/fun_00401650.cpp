#include "re5/recovered/fun_00401650.hpp"

namespace re5::recovered {
namespace {
const FUN_00401650_Services* g_FUN_00401650_services = nullptr;
}

void FUN_00401650_SetServices(const FUN_00401650_Services* services) noexcept {
    g_FUN_00401650_services = services;
}

std::uint32_t FUN_00401650(std::uint32_t argument) noexcept {
    const auto* services = g_FUN_00401650_services;
    if (services == nullptr || services->get_user_stats == nullptr ||
        services->invoke_slot_54 == nullptr) {
        return 0U;
    }

    void* user_stats = services->get_user_stats(services->context);
    return services->invoke_slot_54(user_stats, argument);
}

} // namespace re5::recovered
