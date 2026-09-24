#include "re5/recovered/fun_00407c60.hpp"

#include <cstdarg>
#include <cstdio>

namespace re5::recovered {
namespace {
const FUN_00407C60_Services* g_services = nullptr;

constexpr std::size_t kScratchSize = 0x400U;
constexpr std::size_t kScratchAlignment = 0x10U;
} // namespace

void FUN_00407C60_SetServices(
    const FUN_00407C60_Services* services) noexcept {
    g_services = services;
}

FUN_00402360_String* FUN_00407C60(
    FUN_00402360_String& destination,
    const char* format,
    ...) noexcept {
    const auto* services = g_services;

    // Harness-only safety guards. The native target always has the global
    // allocator service installed and does not branch around these calls.
    if (services == nullptr ||
        services->allocate_aligned == nullptr ||
        services->free_aligned == nullptr) {
        return &destination;
    }

    void* allocation = services->allocate_aligned(
        services->context,
        kScratchSize,
        kScratchAlignment);
    if (allocation == nullptr) {
        return &destination;
    }

    auto* scratch = static_cast<char*>(allocation);

    va_list args;
    va_start(args, format);
    (void)std::vsnprintf(scratch, kScratchSize, format, args);
    va_end(args);

    // The target inlines the same release contract recovered as
    // FUN_004023E0, then invokes FUN_00402360 to construct a fresh block.
    FUN_004023E0(destination);
    FUN_00402360(destination, scratch);

    services->free_aligned(services->context, allocation);
    return &destination;
}

} // namespace re5::recovered
