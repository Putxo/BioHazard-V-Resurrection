#include "re5/recovered/fun_00406ca0.hpp"

namespace re5::recovered {
namespace {
const FUN_00406CA0_Services* g_services = nullptr;

constexpr const char* k_archive_name = "Image\\Archive\\Game2Resource";
constexpr std::uintptr_t k_direct_table = 0x015676D0U;
constexpr std::int32_t k_direct_count = 2;
constexpr std::uintptr_t k_resolved_table = 0x015676E0U;
constexpr std::int32_t k_resolved_count = 5;
constexpr std::int32_t k_message_count = 11;
} // namespace

void FUN_00406CA0_SetServices(const FUN_00406CA0_Services* services) noexcept {
    g_services = services;
}

void FUN_00406CA0() noexcept {
    const auto* services = g_services;
    if (services == nullptr || services->dispatch_resource_group == nullptr) {
        return;
    }

    for (std::int32_t message_index = 0; message_index < k_message_count; ++message_index) {
        services->dispatch_resource_group(
            services->context,
            message_index,
            k_archive_name,
            k_direct_table,
            k_direct_count,
            k_resolved_table,
            k_resolved_count);
    }
}

} // namespace re5::recovered
