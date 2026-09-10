#include "re5/recovered/fun_00402020.hpp"

namespace re5::recovered {
namespace {
constexpr std::uint32_t kCallbackId = 0x44DU;
const FUN_00402020_Services* g_services = nullptr;
}

void FUN_00402020_SetServices(const FUN_00402020_Services* services) noexcept {
    g_services = services;
}

void FUN_00402020(FUN_00402020_CallbackSlot& slot, void* owner, std::uintptr_t callback_va) noexcept {
    if (owner == nullptr || callback_va == 0U) {
        return;
    }

    const auto* services = g_services;
    if (slot.active && services != nullptr && services->unregister_callback != nullptr) {
        services->unregister_callback(services->context, &slot);
    }

    slot.owner = owner;
    slot.callback_va = callback_va;

    if (services != nullptr && services->register_callback != nullptr) {
        services->register_callback(services->context, &slot, kCallbackId);
    }
}

} // namespace re5::recovered
