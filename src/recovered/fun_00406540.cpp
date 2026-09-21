#include "re5/recovered/fun_00406540.hpp"

namespace re5::recovered {
namespace {
const FUN_00406540_Services* g_services = nullptr;

constexpr const char* k_archive_name = "Image\\Archive\\Game2Resource";
constexpr std::uintptr_t k_primary_table = 0x015676D0U;
constexpr std::uint32_t k_primary_count = 2U;
constexpr std::uintptr_t k_secondary_table = 0x015676E0U;
constexpr std::uint32_t k_secondary_count = 5U;
} // namespace

void FUN_00406540_SetServices(const FUN_00406540_Services* services) noexcept {
    g_services = services;
}

void FUN_00406540(std::uint32_t selector) noexcept {
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
