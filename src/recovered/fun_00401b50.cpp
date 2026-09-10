#include "re5/recovered/fun_00401b50.hpp"

namespace re5::recovered {
namespace {
constexpr std::uint32_t kAwardedMask = 0x00030000U;
constexpr std::uint32_t kOfflineAwardMask = 0x00020000U;
constexpr std::uint32_t kOnlineAwardMask = 0x00010000U;

const FUN_00401B50_Services* g_services = nullptr;

const char* award_state_text(std::uint32_t flags) noexcept {
    if ((flags & kOfflineAwardMask) != 0U) {
        return "Offline";
    }
    if ((flags & kOnlineAwardMask) != 0U) {
        return "Online";
    }
    return "";
}
} // namespace

void FUN_00401B50_SetServices(const FUN_00401B50_Services* services) noexcept {
    g_services = services;
}

std::uint32_t FUN_00401B50(
    FUN_00401B50_State& state,
    const FUN_00401B50_AwardRecord* records,
    std::uint32_t record_count) noexcept {
    state.awarded_count = 0;
    state.saved_count = 0;

    const auto* services = g_services;
    if (services != nullptr && services->log_header != nullptr) {
        services->log_header(services->context, nullptr);
    }

    if (records != nullptr) {
        for (std::uint32_t index = 0; index < record_count; ++index) {
            const auto& record = records[index];
            if ((record.flags & kAwardedMask) == 0U) {
                continue;
            }

            const char* state_text = award_state_text(record.flags);
            if (services != nullptr && services->log_award != nullptr) {
                services->log_award(
                    services->context,
                    record.achievement_id,
                    state_text,
                    record.label);
            }

            ++state.awarded_count;
            if (services != nullptr && services->store_award != nullptr &&
                services->store_award(services->context, record.achievement_id)) {
                ++state.saved_count;
            }
        }
    }

    if (services != nullptr && services->log_summary != nullptr) {
        services->log_summary(services->context, state.saved_count, state.awarded_count);
    }
    return state.saved_count;
}

} // namespace re5::recovered
