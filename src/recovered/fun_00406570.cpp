#include "re5/recovered/fun_00406570.hpp"

namespace re5::recovered {
namespace {
const FUN_00406570_Services* g_services = nullptr;

constexpr const char* k_log_marker = ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
constexpr const char* k_archive_name = "Image\\Archive\\TitleResource";
constexpr std::int32_t k_message_index = -1;
constexpr std::uintptr_t k_direct_table = 0x01567708U;
constexpr std::uint32_t k_direct_count = 24U;
constexpr std::uintptr_t k_resolved_table = 0U;
constexpr std::uint32_t k_resolved_count = 0U;
} // namespace

void FUN_00406570_SetServices(const FUN_00406570_Services* services) noexcept {
    g_services = services;
}

void FUN_00406570() noexcept {
    const auto* services = g_services;
    if (services == nullptr) {
        return;
    }

    if (services->log_marker != nullptr) {
        services->log_marker(services->context, k_log_marker);
    }

    if (services->dispatch_resource_group != nullptr) {
        services->dispatch_resource_group(
            services->context,
            k_message_index,
            k_archive_name,
            k_direct_table,
            k_direct_count,
            k_resolved_table,
            k_resolved_count);
    }
}

} // namespace re5::recovered
