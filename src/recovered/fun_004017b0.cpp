#include "re5/recovered/fun_004017b0.hpp"

#include <array>

namespace re5::recovered {
namespace {
const FUN_004017B0_Services* g_services = nullptr;
constexpr std::uint32_t kSeedSize = 8U;
}

void FUN_004017B0_SetServices(const FUN_004017B0_Services* services) noexcept {
    g_services = services;
}

std::uint32_t FUN_004017B0(
    std::uint8_t* destination,
    std::uint32_t destination_size,
    const std::uint8_t* source,
    std::uint32_t length) noexcept {
    const auto* services = g_services;
    if (services == nullptr || services->get_seed8 == nullptr) {
        return 0U;
    }

    if (destination_size < length) {
        return 0U;
    }

    std::array<std::uint8_t, kSeedSize> feedback{};
    services->get_seed8(services->context, feedback.data());

    if (length == 0U) {
        return 0U;
    }

    std::uint32_t processed = 0U;
    while (processed < length) {
        const std::uint32_t lane = processed & (kSeedSize - 1U);
        const std::uint8_t ciphertext = source[processed];
        destination[processed] = static_cast<std::uint8_t>(ciphertext ^ feedback[lane]);
        feedback[lane] = ciphertext;
        ++processed;
    }

    return processed;
}

} // namespace re5::recovered
