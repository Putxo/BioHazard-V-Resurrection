#include "re5/recovered/fun_004068a0.hpp"

namespace re5::recovered {
namespace {
const FUN_004068A0_Services* g_services = nullptr;

constexpr std::int32_t k_message_index = -1;
constexpr const char* k_archive_names[] = {
    "Image\\Archive\\MerceResult01",
    "Image\\Archive\\MerceResult02",
    "Image\\Archive\\MerceResult03",
    "Image\\Archive\\MerceResult04",
    "Image\\Archive\\MerceResult05",
    "Image\\Archive\\MerceResult06",
    "Image\\Archive\\MerceResult07",
    "Image\\Archive\\MerceResult08",
};
constexpr std::uintptr_t k_direct_table_base = 0x01567DF0U;
constexpr std::uintptr_t k_direct_table_stride = 0x10U;
constexpr std::int32_t k_direct_count = 2;
constexpr std::uintptr_t k_resolved_table = 0U;
constexpr std::int32_t k_resolved_count = 0;
} // namespace

void FUN_004068A0_SetServices(const FUN_004068A0_Services* services) noexcept {
    g_services = services;
}

void FUN_004068A0(std::uint32_t result_index) noexcept {
    const auto* services = g_services;
    if (services == nullptr || services->dispatch_resource_group == nullptr) {
        return;
    }

    services->dispatch_resource_group(
        services->context,
        k_message_index,
        k_archive_names[result_index],
        k_direct_table_base + static_cast<std::uintptr_t>(result_index) * k_direct_table_stride,
        k_direct_count,
        k_resolved_table,
        k_resolved_count);
}

} // namespace re5::recovered
