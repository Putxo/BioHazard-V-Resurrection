#include "re5/recovered/fun_00407cd0.hpp"

namespace re5::recovered {
namespace {
const FUN_00407CD0_Services* g_services = nullptr;
} // namespace

void FUN_00407CD0_SetServices(
    const FUN_00407CD0_Services* services) noexcept {
    g_services = services;
}

void FUN_00407CD0(FUN_00407CD0_Object& object) noexcept {
    if (object.pointer == nullptr) {
        return;
    }

    auto* block = static_cast<FUN_00402360_Block*>(object.pointer);
    --block->ref_count;
    if (block->ref_count != 0U) {
        return;
    }

    // The native body assumes global allocator 0x01652F28 is valid and
    // dispatches virtual slot +0x18. This null check is host-harness safety
    // only; it does not add a source-level claim about native control flow.
    const auto* services = g_services;
    if (services != nullptr && services->free_aligned != nullptr) {
        services->free_aligned(services->context, object.pointer);
    }
}

} // namespace re5::recovered
