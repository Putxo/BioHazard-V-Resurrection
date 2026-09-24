#include "re5/recovered/fun_00407dc0.hpp"

#include <cstring>

namespace re5::recovered {
namespace {
const FUN_00407DC0_Services* g_services = nullptr;
} // namespace

void FUN_00407DC0_SetServices(
    const FUN_00407DC0_Services* services) noexcept {
    g_services = services;
}

void FUN_00407DC0(
    FUN_00407DC0_Object& object,
    std::uint32_t index,
    std::uint32_t value) noexcept {
    const auto* services = g_services;

    bool should_lock = object.active_flag != 0U;
    if (!should_lock &&
        services != nullptr &&
        services->read_global_force_flag != nullptr) {
        should_lock =
            services->read_global_force_flag(services->context) != 0U;
    }

    // Native imports are always present. Requiring both callbacks is only a
    // host-harness safety rule so an incomplete shim cannot enter without a
    // matching leave (or vice versa). The indexed write still happens.
    const bool can_lock =
        should_lock &&
        services != nullptr &&
        services->enter_critical_section != nullptr &&
        services->leave_critical_section != nullptr;

    if (can_lock) {
        services->enter_critical_section(
            services->context,
            static_cast<void*>(object.critical_section));
    }

    // Keep the native x86 arithmetic in 32 bits before applying the offset.
    const std::uint32_t offset =
        FUN_00407DC0_ENTRY_BASE +
        index * FUN_00407DC0_ENTRY_STRIDE;
    std::memcpy(
        reinterpret_cast<std::uint8_t*>(&object) + offset,
        &value,
        sizeof(value));

    if (can_lock) {
        services->leave_critical_section(
            services->context,
            static_cast<void*>(object.critical_section));
    }
}

} // namespace re5::recovered
