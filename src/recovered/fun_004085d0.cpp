#include "re5/recovered/fun_004085d0.hpp"

namespace re5::recovered {
namespace {
const FUN_004085D0_Services* g_services = nullptr;

constexpr std::uint32_t kAlignment = 0x10U;
}

void FUN_004085D0_SetServices(
    const FUN_004085D0_Services* services) noexcept {
    g_services = services;
}

void* FUN_004085D0(std::uint32_t size) noexcept {
    const auto* services = g_services;
    if (services == nullptr || services->allocate_aligned == nullptr) {
        return nullptr;
    }

    return services->allocate_aligned(
        services->context,
        size,
        kAlignment);
}

} // namespace re5::recovered
