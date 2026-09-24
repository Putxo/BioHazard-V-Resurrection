#include "re5/recovered/fun_00407d10.hpp"

namespace re5::recovered {
namespace {
const FUN_00407D10_Services* g_services = nullptr;
} // namespace

void FUN_00407D10_SetServices(
    const FUN_00407D10_Services* services) noexcept {
    g_services = services;
}

void FUN_00407D10(
    FUN_00402360_String& destination,
    const char* suffix) noexcept {
    FUN_00402360_String temporary{nullptr};
    FUN_00402360(temporary, suffix);

    auto* incoming =
        static_cast<FUN_00402360_Block*>(temporary.pointer);
    if (incoming == nullptr) {
        return;
    }

    auto* current =
        static_cast<FUN_00402360_Block*>(destination.pointer);
    if (current == nullptr) {
        destination.pointer = incoming;
        ++incoming->ref_count;
    } else {
        const auto* services = g_services;

        // Harness-only guard. The native target calls 0x01199FB0 directly
        // and assumes its allocator-backed concatenate path is available.
        if (services == nullptr || services->concatenate == nullptr) {
            FUN_004023E0(temporary);
            return;
        }

        FUN_00402360_Block* combined =
            services->concatenate(services->context, current, incoming);

        // Harness-only allocation-failure guard. The native helper assumes
        // the global allocator succeeds before it writes the new block.
        if (combined == nullptr) {
            FUN_004023E0(temporary);
            return;
        }

        // The target inlines the same leading-refcount release contract
        // recovered as FUN_004023E0 before installing the combined block.
        FUN_004023E0(destination);
        destination.pointer = combined;
    }

    // Native epilogue destroys the temporary suffix reference after either
    // adoption or concatenation.
    FUN_004023E0(temporary);
}

} // namespace re5::recovered
