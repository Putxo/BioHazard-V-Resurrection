#include "re5/recovered/fun_00404e60.hpp"

namespace re5::recovered {
namespace {
const FUN_00404E60_Services* g_services = nullptr;

constexpr const char* k_effect_root_suffix = "\\effect";
constexpr const char* k_effect_keys[] = {
    "0x137AF48",
    "0x137AF38",
    "0x137AF2C",
    "0x137AF20",
    "0x137AF14",
    "0x137AF04",
};

const char* active_root(const FUN_00404E60_Services& services) noexcept {
    if (services.active_name != nullptr) {
        return services.active_name;
    }
    return services.fallback_name != nullptr ? services.fallback_name : "";
}
} // namespace

void FUN_00404E60_SetServices(const FUN_00404E60_Services* services) noexcept {
    g_services = services;
}

bool FUN_00404E60(FUN_00404E60_Object& object) noexcept {
    const auto* services = g_services;
    if (services == nullptr) {
        return false;
    }

    if (services->scan_resource != nullptr) {
        const char* root = active_root(*services);
        for (const char* key : k_effect_keys) {
            FUN_00404E60_Request request{root, k_effect_root_suffix, key};
            services->scan_resource(services->context, object, request);
        }
    }

    return true;
}

} // namespace re5::recovered
