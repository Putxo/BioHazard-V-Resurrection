#include "re5/recovered/fun_004067a0.hpp"

namespace re5::recovered {
namespace {
const FUN_004067A0_Services* g_services = nullptr;

constexpr std::int32_t k_message_index = -1;
constexpr const char* k_archive_name = "Image\\Archive\\RecordResource";
constexpr std::uintptr_t k_direct_table = 0x01567B98U;
constexpr std::int32_t k_direct_count = 1;
constexpr std::uintptr_t k_resolved_table = 0U;
constexpr std::int32_t k_resolved_count = 0;
} // namespace

void FUN_004067A0_SetServices(const FUN_004067A0_Services* services) noexcept {
    g_services = services;
}

void FUN_004067A0() noexcept {
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
