#include "re5/recovered/fun_004067e0.hpp"
namespace re5::recovered {
namespace { const FUN_004067E0_Services* g_services=nullptr; constexpr std::int32_t k_index=-1; constexpr const char* k_name="Image\\Archive\\FigureResource"; constexpr std::uintptr_t k_table=0x01567BC8U; constexpr std::int32_t k_count=7; }
void FUN_004067E0_SetServices(const FUN_004067E0_Services* s) noexcept { g_services=s; }
void FUN_004067E0() noexcept { const auto* s=g_services; if (!s || !s->dispatch_resource_group) return; s->dispatch_resource_group(s->context,k_index,k_name,k_table,k_count,0U,0); }
}
