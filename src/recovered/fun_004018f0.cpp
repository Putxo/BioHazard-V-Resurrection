#include "re5/recovered/fun_004018f0.hpp"

namespace re5::recovered {
namespace {
const FUN_004018F0_Services* g_services = nullptr;
}

void FUN_004018F0_SetServices(const FUN_004018F0_Services* services) noexcept {
    g_services = services;
}

void FUN_004018F0(bool flag) noexcept {
    const auto* services = g_services;
    if (services == nullptr) {
        return;
    }

    void* object = services->global_object;
    if (!flag) {
        if (services->dispatch_false_path != nullptr) {
            services->dispatch_false_path(services->context, object);
        }
        return;
    }

    if (services->dispatch_true_path != nullptr) {
        services->dispatch_true_path(services->context, object, 1);
    }
}

} // namespace re5::recovered
