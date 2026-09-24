#include "re5/recovered/fun_00407cd0.hpp"

#include <cassert>

namespace {
struct State {
    unsigned free_calls = 0U;
    void* freed_pointer = nullptr;
};

void free_aligned(void* context, void* pointer) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.free_calls;
    state.freed_pointer = pointer;
}
} // namespace

void test_fun_00407cd0() {
    using namespace re5::recovered;

    State state{};
    FUN_00407CD0_Services services{&state, free_aligned};
    FUN_00407CD0_SetServices(&services);

    // Native early exit: a null holder does not touch the allocator.
    FUN_00407CD0_Object empty{};
    FUN_00407CD0(empty);
    assert(empty.pointer == nullptr);
    assert(state.free_calls == 0U);

    // Native early exit: a shared block is decremented but retained.
    FUN_00402360_Block shared{};
    shared.ref_count = 2U;
    FUN_00407CD0_Object shared_object{&shared};
    FUN_00407CD0(shared_object);
    assert(shared.ref_count == 1U);
    assert(shared_object.pointer == &shared);
    assert(state.free_calls == 0U);

    // Native zero-ref path: decrement to zero and free the original block.
    // The holder itself is deliberately not cleared by the native routine.
    FUN_00402360_Block unique{};
    unique.ref_count = 1U;
    FUN_00407CD0_Object unique_object{&unique};
    FUN_00407CD0(unique_object);
    assert(unique.ref_count == 0U);
    assert(unique_object.pointer == &unique);
    assert(state.free_calls == 1U);
    assert(state.freed_pointer == &unique);

    // Host-only safety guard: without an injected allocator the exact
    // decrement/zero decision still occurs, but no invalid host call is made.
    FUN_00407CD0_SetServices(nullptr);
    FUN_00402360_Block no_service{};
    no_service.ref_count = 1U;
    FUN_00407CD0_Object no_service_object{&no_service};
    FUN_00407CD0(no_service_object);
    assert(no_service.ref_count == 0U);
    assert(no_service_object.pointer == &no_service);
    assert(state.free_calls == 1U);
}
