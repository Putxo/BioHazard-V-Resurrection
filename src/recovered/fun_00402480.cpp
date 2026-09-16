#include "re5/recovered/fun_00402480.hpp"

namespace re5::recovered {
namespace {
const FUN_00402480_Services* g_services = nullptr;
}

void FUN_00402480_SetServices(const FUN_00402480_Services* services) noexcept {
    g_services = services;
}

void* FUN_00402480(std::size_t size) noexcept {
    const auto* services = g_services;
    if (services == nullptr || services->allocate_aligned == nullptr) {
        return nullptr;
    }

    return services->allocate_aligned(services->context, size, 0x10U);
}

std::uintptr_t FUN_004024A0() noexcept {
    const auto* services = g_services;
    return services != nullptr ? services->value_016e201c : 0U;
}

void FUN_004024B0(void* base, std::size_t stride, std::int32_t count, FUN_004024B0_Callback callback) noexcept {
    if (base == nullptr || callback == nullptr || count <= 0) {
        return;
    }

    auto* cursor = static_cast<unsigned char*>(base) + (stride * static_cast<std::size_t>(count));
    for (std::int32_t remaining = count - 1; remaining >= 0; --remaining) {
        cursor -= stride;
        callback(cursor);
    }
}

} // namespace re5::recovered
