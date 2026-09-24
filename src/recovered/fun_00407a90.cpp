#include "re5/recovered/fun_00407a90.hpp"

#include <limits>
#include <new>

namespace re5::recovered {
namespace {
const FUN_00407A90_Services* g_services = nullptr;

constexpr std::uint32_t k_alignment = 0x10U;
} // namespace

void FUN_00407A90_SetServices(const FUN_00407A90_Services* services) noexcept {
    g_services = services;
}

void* FUN_00407A90(
    std::uint32_t requested_size,
    const void* unused_type_tag) {
    // The second native stack argument is a type/tag placeholder. The body
    // never reads it and overwrites that stack slot with the fixed alignment
    // immediately before its tail jump to the allocator.
    (void)unused_type_tag;

    // Preserve the target's exact unsigned limit check. With a 32-bit
    // requested_size and element size 1, every nonzero input yields a
    // quotient >= 1, so the std::bad_alloc construction path is unreachable
    // for the observed ABI even though it is present in the native body.
    if (requested_size != 0U) {
        const std::uint32_t quotient =
            std::numeric_limits<std::uint32_t>::max() / requested_size;
        if (quotient < 1U) {
            throw std::bad_alloc{};
        }
    }

    const auto* services = g_services;
    if (services == nullptr || services->allocate_aligned == nullptr) {
        return nullptr;
    }

    return services->allocate_aligned(
        services->context,
        requested_size,
        k_alignment);
}

} // namespace re5::recovered
