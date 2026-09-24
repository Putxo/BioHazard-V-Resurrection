#include "re5/recovered/fun_00407cf0.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <cstring>

namespace {
struct State {
    alignas(16) std::array<std::byte, 128> storage{};
    unsigned alloc_calls = 0U;
    std::size_t requested_size = 0U;
    std::size_t requested_alignment = 0U;
};

void* allocate_aligned(
    void* context,
    std::size_t size,
    std::size_t alignment) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.alloc_calls;
    state.requested_size = size;
    state.requested_alignment = alignment;
    return state.storage.data();
}

void free_aligned(void*, void*) noexcept {
}
} // namespace

void test_fun_00407cf0() {
    using namespace re5::recovered;

    // The wrapper has no native conditional branch. These cases exercise the
    // exact delegated FUN_00402360 early-return behavior after the native
    // pre-clear store at [this].
    FUN_00407CF0_Object destination{reinterpret_cast<void*>(1)};

    FUN_00402360_SetServices(nullptr);
    assert(FUN_00407CF0(destination, nullptr) == &destination);
    assert(destination.pointer == nullptr);

    destination.pointer = reinterpret_cast<void*>(1);
    assert(FUN_00407CF0(destination, "") == &destination);
    assert(destination.pointer == nullptr);

    // Portable-host allocator guard is owned by FUN_00402360. The CF0 wrapper
    // still pre-clears the holder, delegates, and returns this.
    destination.pointer = reinterpret_cast<void*>(1);
    assert(FUN_00407CF0(destination, "Jill") == &destination);
    assert(destination.pointer == nullptr);

    State state{};
    static constexpr char fallback[] = "";
    FUN_00402360_Services services{
        '\0',
        &state,
        allocate_aligned,
        free_aligned,
        fallback,
    };
    FUN_00402360_SetServices(&services);

    destination.pointer = reinterpret_cast<void*>(1);
    assert(FUN_00407CF0(destination, "Jill") == &destination);
    assert(state.alloc_calls == 1U);
    assert(state.requested_size == 4U + 0x0CU);
    assert(state.requested_alignment == 0x10U);

    auto* block = static_cast<FUN_00402360_Block*>(destination.pointer);
    assert(block != nullptr);
    assert(block->ref_count == 1U);
    assert(block->length == 4U);
    assert(std::strcmp(block->data, "Jill") == 0);

    FUN_00402360_SetServices(nullptr);
}
