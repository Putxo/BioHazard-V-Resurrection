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

void* FUN_004024A0() noexcept {
    const auto* services = g_services;
    return services != nullptr ? services->global_16e201c : nullptr;
}

void FUN_004024B0(
    std::uint8_t* base,
    std::uint32_t element_size,
    std::int32_t count,
    void (*destroy)(void* element) noexcept) noexcept {
    if (base == nullptr || destroy == nullptr || count <= 0) {
        return;
    }

    std::uint8_t* current = base + static_cast<std::size_t>(element_size) * static_cast<std::size_t>(count);
    for (std::int32_t remaining = count; remaining > 0; --remaining) {
        current -= element_size;
        destroy(current);
    }
}

void FUN_004024F0() noexcept {}

std::uintptr_t FUN_00402500(void* object, void* argument) noexcept {
    const auto* services = g_services;
    if (services == nullptr || services->invoke_00402500 == nullptr) {
        return 0U;
    }
    return services->invoke_00402500(services->context, object, argument);
}

void FUN_00402510() noexcept {}

} // namespace re5::recovered
