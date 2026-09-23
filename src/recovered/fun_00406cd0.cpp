#include "re5/recovered/fun_00406cd0.hpp"

#include <array>

namespace re5::recovered {
namespace {
const FUN_00406CD0_Services* g_services = nullptr;

constexpr const char* k_log_marker = ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
constexpr std::int32_t k_message_index = -1;
constexpr std::uintptr_t k_resolved_table = 0U;
constexpr std::uint32_t k_resolved_count = 0U;

struct ResourceGroup {
    const char* archive_name;
    std::uintptr_t direct_table;
    std::uint32_t direct_count;
};

constexpr std::array<ResourceGroup, 8> k_groups{{
    {"Image\\Archive\\TitleResource", 0x01567708U, 24U},
    {"Image\\Archive\\MenuResource", 0x01567800U, 22U},
    {"Image\\Archive\\ShopResource", 0x01567928U, 3U},
    {"Image\\Archive\\VersusResource", 0x01567958U, 9U},
    {"Image\\Archive\\MerceResource", 0x015679A0U, 8U},
    {"Image\\Archive\\RecordResource", 0x01567B98U, 1U},
    {"Image\\Archive\\ViewerResource", 0x01567BA0U, 5U},
    {"Image\\Archive\\CreditResource", 0x01567C00U, 1U},
}};
} // namespace

void FUN_00406CD0_SetServices(const FUN_00406CD0_Services* services) noexcept {
    g_services = services;
}

void FUN_00406CD0() noexcept {
    const auto* services = g_services;
    if (services == nullptr) {
        return;
    }

    if (services->log_marker != nullptr) {
        services->log_marker(services->context, k_log_marker);
    }

    if (services->dispatch_resource_group == nullptr) {
        return;
    }

    for (const auto& group : k_groups) {
        services->dispatch_resource_group(
            services->context,
            k_message_index,
            group.archive_name,
            group.direct_table,
            group.direct_count,
            k_resolved_table,
            k_resolved_count);
    }
}

} // namespace re5::recovered
