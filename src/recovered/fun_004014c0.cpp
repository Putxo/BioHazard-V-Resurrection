#include "re5/recovered/fun_004014c0.hpp"

namespace re5::recovered {
namespace {
constexpr std::uintptr_t kPrimaryGateToken = 0x0004F8A8U;
constexpr std::uintptr_t kLogContext = 0x01652E00U;
constexpr std::uintptr_t kPrimaryMessage = 0x0137A744U;
constexpr std::uintptr_t kSecondaryGateFailureMessage = 0x0137A724U;
constexpr std::uintptr_t kSecondaryObjectMissingMessage = 0x0137A6E0U;
constexpr std::uintptr_t kSecondaryNotReadyMessage = 0x0137A6FCU;
constexpr std::uint32_t kPollSleepMilliseconds = 1000U;
constexpr std::uint32_t kPollSleeps = 5U;

const FUN_004014C0_Services* g_FUN_004014C0_services = nullptr;
} // namespace

void FUN_004014C0_SetServices(const FUN_004014C0_Services* services) noexcept {
    g_FUN_004014C0_services = services;
}

bool FUN_004014C0() noexcept {
    const auto* services = g_FUN_004014C0_services;
    if (services == nullptr || services->primary_ready == nullptr ||
        services->primary_gate == nullptr || services->primary_log == nullptr ||
        services->set_wait_flag == nullptr || services->sleep_ms == nullptr ||
        services->secondary_gate == nullptr || services->get_secondary_object == nullptr ||
        services->secondary_ready == nullptr || services->secondary_log == nullptr) {
        return false;
    }

    if (!services->primary_ready(services->context) &&
        services->primary_gate(services->context, kPrimaryGateToken)) {
        services->primary_log(services->context, kLogContext, kPrimaryMessage);
        services->set_wait_flag(services->context, true);

        if (!services->primary_ready(services->context)) {
            for (std::uint32_t i = 0; i < kPollSleeps; ++i) {
                services->sleep_ms(services->context, kPollSleepMilliseconds);
                if (services->primary_ready(services->context)) {
                    break;
                }
            }
        }
    }

    // The original writes zero to 0x01655B57 on every path reaching 0x0040151F,
    // including paths where the flag was never first set to one.
    services->set_wait_flag(services->context, false);

    if (!services->secondary_gate(services->context)) {
        services->secondary_log(
            services->context, kLogContext, kSecondaryGateFailureMessage);
        return false;
    }

    void* object = services->get_secondary_object(services->context);
    if (!services->secondary_ready(object)) {
        for (std::uint32_t i = 0; i < kPollSleeps; ++i) {
            services->sleep_ms(services->context, kPollSleepMilliseconds);
            object = services->get_secondary_object(services->context);
            if (services->secondary_ready(object)) {
                break;
            }
        }
    }

    object = services->get_secondary_object(services->context);
    if (object == nullptr) {
        services->secondary_log(
            services->context, kLogContext, kSecondaryObjectMissingMessage);
        return true;
    }

    object = services->get_secondary_object(services->context);
    if (!services->secondary_ready(object)) {
        services->secondary_log(
            services->context, kLogContext, kSecondaryNotReadyMessage);
    }
    return true;
}

} // namespace re5::recovered
