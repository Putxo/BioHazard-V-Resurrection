#include "re5/recovered/fun_00406600.hpp"

namespace re5::recovered {
namespace {
const FUN_00406600_Services* g_services = nullptr;

constexpr std::int32_t k_message_index = -1;
constexpr const char* k_archive_name = "Image\\Archive\\MenuResource";
constexpr std::uintptr_t k_direct_table = 0x01567800U;
constexpr std::uint32_t k_direct_count = 22U;
constexpr std::uintptr_t k_resolved_table = 0U;
constexpr std::uint32_t k_resolved_count = 0U;
} // namespace

void FUN_00406600_SetServices(const FUN_00406600_Services* services) noexcept {
    g_services = services;
}

void FUN_00406600() noexcept {
    const auto* services = g_services;
    if (services == nullptr || services->dispatch_resource_group == nullptr) {
        return;
    }

    services->dispatch_resource_group(
        services->context,
        k_message_index,
        k_archive_name,
        k_direct_table,
        k_direct_count,
        k_resolved_table,
        k_resolved_count);
}

} // namespace re5::recovered
