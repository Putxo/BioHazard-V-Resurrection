#include "re5/recovered/fun_004085d0.hpp"

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
    bool return_null = false;
};

void* allocate_aligned(
    void* context,
    std::uint32_t size,
    std::uint32_t alignment) noexcept {
    auto& state = *static_cast<AllocatorState*>(context);
    ++state.calls;
    state.requested_size = size;
    state.requested_alignment = alignment;
    if (state.return_null) {
        return nullptr;
    }
    return state.storage.data();
}

} // namespace

void test_fun_004085d0() {
    using namespace re5::recovered;

    // Host-only guard: the native target assumes the global allocator owner
    // and its virtual slot are valid.
    FUN_004085D0_SetServices(nullptr);
    assert(FUN_004085D0(0x08U) == nullptr);

    // Host-only guard for a present service object with no callback.
    const FUN_004085D0_Services missing_service{
        nullptr,
        nullptr,
    };
    FUN_004085D0_SetServices(&missing_service);
    assert(FUN_004085D0(0x10U) == nullptr);

    AllocatorState state{};
    const FUN_004085D0_Services services{
        &state,
        allocate_aligned,
    };
    FUN_004085D0_SetServices(&services);

    // Native zero-size path: the wrapper has no size guard and forwards zero
    // unchanged with the fixed 0x10 alignment.
    assert(FUN_004085D0(0U) == state.storage.data());
    assert(state.calls == 1U);
    assert(state.requested_size == 0U);
    assert(state.requested_alignment == 0x10U);

    // Representative observed caller size.
    assert(FUN_004085D0(0x5CU) == state.storage.data());
    assert(state.calls == 2U);
    assert(state.requested_size == 0x5CU);
    assert(state.requested_alignment == 0x10U);

    // Exact 32-bit forwarding boundary.
    assert(FUN_004085D0(
               std::numeric_limits<std::uint32_t>::max()) ==
           state.storage.data());
    assert(state.calls == 3U);
    assert(
        state.requested_size ==
        std::numeric_limits<std::uint32_t>::max());
    assert(state.requested_alignment == 0x10U);

    // Native allocator result is returned without transformation, including
    // a null allocation result.
    state.return_null = true;
    assert(FUN_004085D0(0x1CU) == nullptr);
    assert(state.calls == 4U);
    assert(state.requested_size == 0x1CU);
    assert(state.requested_alignment == 0x10U);

    FUN_004085D0_SetServices(nullptr);
}
