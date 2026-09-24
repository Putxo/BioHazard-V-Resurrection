#include "re5/recovered/fun_00407a90.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>

namespace {
struct AllocatorState {
    std::array<std::byte, 64> storage{};
    std::uint32_t requested_size = 0U;
    std::uint32_t requested_alignment = 0U;
    unsigned calls = 0U;
};

void* allocate_aligned(
    void* context,
    std::uint32_t requested_size,
    std::uint32_t alignment) {
    auto& state = *static_cast<AllocatorState*>(context);
    ++state.calls;
    state.requested_size = requested_size;
    state.requested_alignment = alignment;
    return state.storage.data();
}
} // namespace

void test_fun_00407a90() {
    using namespace re5::recovered;

    // Portable-host guard: the native body has no services abstraction.
    FUN_00407A90_SetServices(nullptr);
    assert(FUN_00407A90(0x20U, nullptr) == nullptr);

    // Portable-host guard: do not call through a missing allocator callback.
    FUN_00407A90_Services missing_services{nullptr, nullptr};
    FUN_00407A90_SetServices(&missing_services);
    assert(FUN_00407A90(0x20U, nullptr) == nullptr);

    AllocatorState state{};
    FUN_00407A90_Services services{&state, allocate_aligned};
    FUN_00407A90_SetServices(&services);

    // Native zero branch: zero is still forwarded to the aligned allocator.
    int ignored_tag = 7;
    assert(FUN_00407A90(0U, &ignored_tag) == state.storage.data());
    assert(state.calls == 1U);
    assert(state.requested_size == 0U);
    assert(state.requested_alignment == 0x10U);

    // Normal nonzero path. The second ABI argument is intentionally ignored.
    assert(FUN_00407A90(0x1234U, nullptr) == state.storage.data());
    assert(state.calls == 2U);
    assert(state.requested_size == 0x1234U);
    assert(state.requested_alignment == 0x10U);

    // Boundary proof for the native unsigned division guard: UINT32_MAX /
    // UINT32_MAX == 1, so even the largest representable input allocates.
    assert(FUN_00407A90(
               std::numeric_limits<std::uint32_t>::max(),
               &ignored_tag) == state.storage.data());
    assert(state.calls == 3U);
    assert(state.requested_size == std::numeric_limits<std::uint32_t>::max());
    assert(state.requested_alignment == 0x10U);

    FUN_00407A90_SetServices(nullptr);
}
