#include "re5/recovered/fun_00407fd0.hpp"

namespace re5::recovered {
namespace {
const FUN_00407FD0_Services* g_services = nullptr;
} // namespace

void FUN_00407FD0_SetServices(
    const FUN_00407FD0_Services* services) noexcept {
    g_services = services;
}

void FUN_00407FD0(
    FUN_00402360_String& destination,
    const char* suffix) noexcept {
    // The native routine reuses its caller-owned suffix stack slot as this
    // temporary holder. The portable form models the same lifetime directly.
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

        // Host-only guard. The native target calls 0x01199FB0 directly and
        // assumes the allocator-backed concatenate helper is available.
        if (services == nullptr || services->concatenate == nullptr) {
            FUN_004023E0(temporary);
            return;
        }

        FUN_00402360_Block* combined =
            services->concatenate(
                services->context,
                current,
                incoming);

        // Host-only allocation-failure guard. Native code assumes the opaque
        // concatenate helper succeeds before releasing the old destination.
        if (combined == nullptr) {
            FUN_004023E0(temporary);
            return;
        }

        FUN_004023E0(destination);
        destination.pointer = combined;
    }

    FUN_004023E0(temporary);
}

} // namespace re5::recovered
