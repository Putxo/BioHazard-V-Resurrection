#include "re5/recovered/fun_004065d0.hpp"

namespace re5::recovered {
namespace {
const FUN_004065D0_Services* g_services = nullptr;

constexpr std::uintptr_t k_logger_token = 0x01652E00U;
constexpr std::uintptr_t k_trace_format_token = 0x0137AFECU;
constexpr std::int32_t k_message_index = -1;
constexpr const char* k_archive_name = "Image\\Archive\\ShowTitleResource";
constexpr std::uintptr_t k_direct_table = 0x015677D8U;
constexpr std::int32_t k_direct_count = 5;
constexpr std::uintptr_t k_resolved_table = 0U;
constexpr std::int32_t k_resolved_count = 0;
} // namespace

void FUN_004065D0_SetServices(const FUN_004065D0_Services* services) noexcept {
    g_services = services;
}

void FUN_004065D0() noexcept {
    const auto* services = g_services;
    if (services == nullptr) {
        return;
    }

    if (services->trace_marker != nullptr) {
        services->trace_marker(services->context, k_logger_token, k_trace_format_token);
    }

    if (services->dispatch_resource_group == nullptr) {
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
