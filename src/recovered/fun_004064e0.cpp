#include "re5/recovered/fun_004064e0.hpp"

namespace re5::recovered {
namespace {
const FUN_004064E0_Services* g_services = nullptr;

constexpr const char* k_archive_name = "Image\\Archive\\GameResource";
constexpr std::uintptr_t k_primary_table = 0x01567590U;
constexpr std::uint32_t k_primary_count = 15U;
constexpr std::uintptr_t k_secondary_table = 0x01567608U;
constexpr std::uint32_t k_secondary_count = 7U;
} // namespace

void FUN_004064E0_SetServices(const FUN_004064E0_Services* services) noexcept {
    g_services = services;
}

void FUN_004064E0(std::uint32_t selector) noexcept {
    const auto* services = g_services;
    if (services == nullptr || services->dispatch_resource_group == nullptr) {
        return;
    }

    services->dispatch_resource_group(
        services->context,
        selector,
        k_archive_name,
        k_primary_table,
        k_primary_count,
        k_secondary_table,
        k_secondary_count);
}

} // namespace re5::recovered
