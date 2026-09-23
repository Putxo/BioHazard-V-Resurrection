#include "re5/recovered/fun_00406870.hpp"

namespace re5::recovered {
namespace {
const FUN_00406870_Services* g_services = nullptr;

constexpr std::int32_t k_message_index = -1;
constexpr const char* k_archive_names[] = {
    "Image\\Archive\\VersusResult01",
    "Image\\Archive\\VersusResult02",
    "Image\\Archive\\VersusResult03",
    "Image\\Archive\\VersusResult04",
    "Image\\Archive\\VersusResult05",
    "Image\\Archive\\VersusResult06",
    "Image\\Archive\\VersusResult07",
    "Image\\Archive\\VersusResult08",
};
constexpr std::uintptr_t k_direct_table_base = 0x01567D50U;
constexpr std::uintptr_t k_direct_table_stride = 0x10U;
constexpr std::int32_t k_direct_count = 2;
constexpr std::uintptr_t k_resolved_table = 0U;
constexpr std::int32_t k_resolved_count = 0;
} // namespace

void FUN_00406870_SetServices(const FUN_00406870_Services* services) noexcept {
    g_services = services;
}

void FUN_00406870(std::uint32_t variant) noexcept {
    const auto* services = g_services;
    if (services == nullptr || services->dispatch_resource_group == nullptr) {
        return;
    }

    services->dispatch_resource_group(
        services->context,
        k_message_index,
        k_archive_names[variant],
        k_direct_table_base + static_cast<std::uintptr_t>(variant) * k_direct_table_stride,
        k_direct_count,
        k_resolved_table,
        k_resolved_count);
}

} // namespace re5::recovered
