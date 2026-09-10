#include "re5/recovered/fun_00401d10.hpp"

namespace re5::recovered {
namespace {
constexpr std::uint32_t kAchievementEnumerationCount = 0x46U;
const FUN_00401D10_Services* g_services = nullptr;
}

void FUN_00401D10_SetServices(const FUN_00401D10_Services* services) noexcept {
    g_services = services;
}

std::uint32_t FUN_00401D10(FUN_00401B50_State& state) noexcept {
    const auto* services = g_services;
    const FUN_00401B50_AwardRecord* records = nullptr;
    std::uint32_t enumerated_count = 0;

    if (services != nullptr && services->enumerate_awards != nullptr) {
        enumerated_count = services->enumerate_awards(
            services->context,
            &records,
            kAchievementEnumerationCount,
            0U);
    }

    state.award_records = records;

    if (services != nullptr && services->refresh_user_stats != nullptr) {
        services->refresh_user_stats(services->context, 1);
    }

    (void)FUN_00401B50(state, state.award_records, enumerated_count);
    return enumerated_count;
}

} // namespace re5::recovered
