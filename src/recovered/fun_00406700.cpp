#include "re5/recovered/fun_00406700.hpp"

namespace re5::recovered {
namespace {
const FUN_00406700_Services* g_services = nullptr;

constexpr std::int32_t k_message_index = -1;
constexpr const char* k_archive_name = "Image\\Archive\\Extra1Resource";
constexpr std::uintptr_t k_direct_table = 0x015679E0U;
constexpr std::int32_t k_direct_count = 20;
constexpr std::uintptr_t k_resolved_table = 0U;
constexpr std::int32_t k_resolved_count = 0;
} // namespace

void FUN_00406700_SetServices(const FUN_00406700_Services* services) noexcept {
    g_services = services;
}

void FUN_00406700() noexcept {
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
