#include "re5/recovered/fun_00401d50.hpp"

namespace re5::recovered {
namespace {
constexpr std::uintptr_t kCallbackA = 0x00401A70U;
constexpr std::uintptr_t kCallbackB = 0x00401230U;
constexpr std::uintptr_t kCallbackC = 0x00401240U;

const FUN_00401D50_Services* g_services = nullptr;

void initialize_slot(
    FUN_00401D50_Object& object,
    FUN_00401D50_CallbackSlot& slot,
    std::uintptr_t callback_va) noexcept {
    slot.name = "";
    slot.active = false;
    slot.value = 0;
    slot.owner = &object;
    slot.callback_va = callback_va;

    const auto* services = g_services;
    if (callback_va != 0U && services != nullptr && services->register_callback != nullptr) {
        services->register_callback(services->context, &object, &slot, callback_va);
    }
}
} // namespace

void FUN_00401D50_SetServices(const FUN_00401D50_Services* services) noexcept {
    g_services = services;
}

FUN_00401D50_Object* FUN_00401D50(FUN_00401D50_Object& object) noexcept {
    object.summary.award_records = nullptr;

    initialize_slot(object, object.slot_10, kCallbackA);
    initialize_slot(object, object.slot_24, kCallbackB);
    initialize_slot(object, object.slot_38, kCallbackC);

    const auto* services = g_services;
    if (services != nullptr && services->publish_global_instance != nullptr) {
        services->publish_global_instance(services->context, &object);
    }

    if (services != nullptr && services->has_user_stats != nullptr &&
        services->has_user_stats(services->context) &&
        services->has_user != nullptr && services->has_user(services->context) &&
        services->is_user_logged_on != nullptr && services->is_user_logged_on(services->context) &&
        services->request_current_stats != nullptr) {
        services->request_current_stats(services->context);
    }

    object.summary.awarded_count = 0;
    object.summary.saved_count = 0xFFFFFFFFU;
    return &object;
}

} // namespace re5::recovered
