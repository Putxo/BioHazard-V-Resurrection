#include "re5/recovered/fun_00401e30.hpp"

namespace re5::recovered {
namespace {
constexpr unsigned kObjectSize = 0x4CU;
constexpr unsigned kObjectAlignment = 0x10U;

const FUN_00401E30_Services* g_services = nullptr;
}

void FUN_00401E30_SetServices(const FUN_00401E30_Services* services) noexcept {
    g_services = services;
}

FUN_00401D50_Object* FUN_00401E30() noexcept {
    const auto* services = g_services;
    if (services != nullptr && services->get_global_instance != nullptr) {
        if (FUN_00401D50_Object* existing = services->get_global_instance(services->context)) {
            return existing;
        }
    }

    if (services == nullptr || services->allocate_instance == nullptr) {
        return nullptr;
    }

    FUN_00401D50_Object* object = services->allocate_instance(
        services->context,
        kObjectSize,
        kObjectAlignment);
    if (object == nullptr) {
        return nullptr;
    }

    return FUN_00401D50(*object);
}

} // namespace re5::recovered
