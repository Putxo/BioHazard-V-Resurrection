#include "re5/recovered/fun_00403f50.hpp"

namespace re5::recovered {
namespace {
const FUN_00403F50_Services* g_services = nullptr;

constexpr const char* k_shared_suffixes[] = {
    "0x137AF60",
    "0x137AEF0",
    "0x137AEE8",
    "0x137AEE0",
    "0x137AED8",
    "0x137AECC",
    "0x137AEC4",
    "0x137AEB8",
};

constexpr const char* k_group_tags[] = {
    "0x137AF48",
    "0x137AEAC",
    "0x137AEA0",
    "0x137AE94",
    "0x137AE88",
    "0x137AE78",
};

const char* safe_active(const FUN_00403F50_Services& services) noexcept {
    if (services.active_name != nullptr) {
        return services.active_name;
    }
    return services.fallback_name != nullptr ? services.fallback_name : "";
}
} // namespace

void FUN_00403F50_SetServices(const FUN_00403F50_Services* services) noexcept {
    g_services = services;
}

void FUN_00403F50(FUN_00403F50_Object& object) noexcept {
    const auto* services = g_services;
    if (services == nullptr) {
        return;
    }

    void* state = services->allocate_state != nullptr
        ? services->allocate_state(services->context, 0x130U)
        : nullptr;
    object.state_19c = state;

    if (services->scan_resource != nullptr) {
        const char* root = safe_active(*services);
        for (const char* group : k_group_tags) {
            FUN_00403F50_Request group_request{root, group, "group"};
            services->scan_resource(services->context, object, group_request);
            for (const char* suffix : k_shared_suffixes) {
                FUN_00403F50_Request request{root, suffix, group};
                services->scan_resource(services->context, object, request);
            }
        }
        FUN_00403F50_Request tail_a{root, "0x137AF20", "tail"};
        FUN_00403F50_Request tail_b{root, "0x137AE5C", "tail"};
        services->scan_resource(services->context, object, tail_a);
        services->scan_resource(services->context, object, tail_b);
    }

    if (state != nullptr && services->release_state != nullptr) {
        services->release_state(services->context, state);
    }
    object.state_19c = nullptr;
}

} // namespace re5::recovered
