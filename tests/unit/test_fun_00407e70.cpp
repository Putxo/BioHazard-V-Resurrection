#include "re5/recovered/fun_00407e70.hpp"

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

void test_fun_00407e70() {
    using namespace re5::recovered;

    FUN_00407E70_Object object{};

    // E70 itself has no native branch. These first two cases deliberately
    // exercise the portable-host guards of the exact delegated A90 path.
    FUN_00407A90_SetServices(nullptr);
    assert(FUN_00407E70(object, 0x20U) == nullptr);

    FUN_00407A90_Services missing_service{nullptr, nullptr};
    FUN_00407A90_SetServices(&missing_service);
    assert(FUN_00407E70(object, 0x20U) == nullptr);

    AllocatorState state{};
    FUN_00407A90_Services services{&state, allocate_aligned};
    FUN_00407A90_SetServices(&services);

    // Native zero-size branch in A90: E70 forwards zero unchanged and A90
    // still invokes the aligned allocator with fixed alignment 0x10.
    assert(FUN_00407E70(object, 0U) == state.storage.data());
    assert(state.calls == 1U);
    assert(state.requested_size == 0U);
    assert(state.requested_alignment == 0x10U);

    // Normal wrapper path: no size adjustment and the delegated return value
    // is propagated unchanged in EAX.
    assert(FUN_00407E70(object, 0x1234U) == state.storage.data());
    assert(state.calls == 2U);
    assert(state.requested_size == 0x1234U);
    assert(state.requested_alignment == 0x10U);

    // Exact target-width boundary inherited from A90. UINT32_MAX is legal
    // because UINT32_MAX / UINT32_MAX == 1.
    assert(FUN_00407E70(
               object,
               std::numeric_limits<std::uint32_t>::max()) ==
           state.storage.data());
    assert(state.calls == 3U);
    assert(state.requested_size ==
           std::numeric_limits<std::uint32_t>::max());
    assert(state.requested_alignment == 0x10U);

    FUN_00407A90_SetServices(nullptr);
}
