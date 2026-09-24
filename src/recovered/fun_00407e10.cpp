#include "re5/recovered/fun_00407e10.hpp"

#include <cstring>

namespace re5::recovered {
namespace {
const FUN_00407E10_Services* g_services = nullptr;
} // namespace

void FUN_00407E10_SetServices(
    const FUN_00407E10_Services* services) noexcept {
    g_services = services;
}

void FUN_00407E10(
    FUN_00407E10_Object& object,
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
    // matching leave (or vice versa). The indexed write remains unconditional.
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

    // Keep the native x86 multiply/add arithmetic in 32 bits.
    const std::uint32_t offset =
        FUN_00407E10_ENTRY_BASE +
        object.current_index * FUN_00407E10_ENTRY_STRIDE;
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
