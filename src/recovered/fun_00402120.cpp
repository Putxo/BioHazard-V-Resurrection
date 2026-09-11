#include "re5/recovered/fun_00402120.hpp"

namespace re5::recovered {
namespace {
const FUN_00402120_Services* g_services = nullptr;
}

void FUN_00402120_SetServices(const FUN_00402120_Services* services) noexcept {
    g_services = services;
}

void* FUN_00402120(std::size_t size) noexcept {
    const auto* services = g_services;
    if (services == nullptr || services->allocate_aligned == nullptr) {
        return nullptr;
    }

    return services->allocate_aligned(services->allocator_context, size, 0x10U);
}

} // namespace re5::recovered
