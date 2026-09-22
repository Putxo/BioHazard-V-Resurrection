#include "re5/recovered/fun_004067e0.hpp"

namespace re5::recovered {
namespace {
const FUN_004067E0_Services* g_services = nullptr;
constexpr std::int32_t k_message_index = -1;
constexpr const char* k_archive_name = "Image\\Archive\\FigureResource";
constexpr std::uintptr_t k_direct_table = 0x01567BC8U;
constexpr std::int32_t k_direct_count = 7;
}
void FUN_004067E0_SetServices(const FUN_004067E0_Services* services) noexcept { g_services = services; }
void FUN_004067E0() noexcept {
    const auto* services = g_services;
    if (services == nullptr || services->dispatch_resource_group == nullptr) return;
    services->dispatch_resource_group(services->context, k_message_index, k_archive_name, k_direct_table, k_direct_count, 0U, 0);
}
} // namespace re5::recovered
