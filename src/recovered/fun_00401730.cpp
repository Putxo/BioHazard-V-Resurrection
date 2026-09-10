#include "re5/recovered/fun_00401730.hpp"

#include <array>

namespace re5::recovered {
namespace {
const FUN_00401730_Services* g_services = nullptr;
constexpr std::uint32_t kSeedSize = 8U;
}

void FUN_00401730_SetServices(const FUN_00401730_Services* services) noexcept {
    g_services = services;
}

std::uint32_t FUN_00401730(
    std::uint8_t* destination,
    std::uint32_t destination_size,
    const std::uint8_t* source,
    std::uint32_t length) noexcept {
    const auto* services = g_services;
    if (services == nullptr || services->get_seed8 == nullptr) {
        return 0U;
    }

    if (destination_size < length || length == 0U) {
        return 0U;
    }

    std::array<std::uint8_t, kSeedSize> feedback{};
    services->get_seed8(services->context, feedback.data());

    std::uint32_t processed = 0U;
    while (processed < length) {
        const std::uint32_t lane = processed & (kSeedSize - 1U);
        feedback[lane] ^= source[processed];
        destination[processed] = feedback[lane];
        ++processed;
    }

    return processed;
}

} // namespace re5::recovered
