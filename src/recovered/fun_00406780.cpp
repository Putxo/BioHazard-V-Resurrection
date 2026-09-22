#include "re5/recovered/fun_00406780.hpp"

namespace re5::recovered {
namespace {
const FUN_00406780_Services* g_services = nullptr;
constexpr std::int32_t k_message_index = -1;
constexpr const char* k_archive_name = "Image\\Archive\\Merce2Resource";
constexpr std::uintptr_t k_direct_table = 0x01567B30U;
constexpr std::int32_t k_direct_count = 13;
constexpr std::uintptr_t k_resolved_table = 0U;
constexpr std::int32_t k_resolved_count = 0;
}
void FUN_00406780_SetServices(const FUN_00406780_Services* services) noexcept { g_services = services; }
void FUN_00406780() noexcept {
    const auto* services = g_services;
    if (services == nullptr || services->dispatch_resource_group == nullptr) return;
    services->dispatch_resource_group(services->context,k_message_index,k_archive_name,k_direct_table,k_direct_count,k_resolved_table,k_resolved_count);
}
} // namespace re5::recovered
