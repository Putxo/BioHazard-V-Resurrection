#include "re5/recovered/fun_00408610.hpp"

namespace re5::recovered {
namespace {
const FUN_00408610_Services* g_services = nullptr;
} // namespace

void FUN_00408610_SetServices(
    const FUN_00408610_Services* services) noexcept {
    g_services = services;
}

std::uint32_t FUN_00408610(
    FUN_00408610_Object& object) noexcept {
    // The native thunk is called as a no-stack-argument thiscall, but its
    // tail target ignores ECX and advances four fixed global state words.
    (void)object;

    const auto* services = g_services;

    // Host-only guard. The native target assumes its four global state words
    // at 0x01653058..0x01653064 are always present.
    if (services == nullptr || services->state == nullptr) {
        return 0U;
    }

    auto& state = *services->state;

    const std::uint32_t first = state.word0;
    const std::uint32_t second = state.word1;
    const std::uint32_t third = state.word2;
    const std::uint32_t fourth = state.word3;

    const std::uint32_t mixed =
        first ^ (first << 15U);

    state.word0 = second;
    state.word1 = third;

    std::uint32_t next = fourth >> 17U;
    next ^= mixed;
    next >>= 4U;
    next ^= fourth;
    next ^= mixed;

    state.word2 = fourth;
    state.word3 = next;
    return next;
}

} // namespace re5::recovered
