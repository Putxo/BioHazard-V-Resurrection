#include "re5/recovered/fun_00407bc0.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>

namespace {
struct State {
    std::size_t calls = 0U;
    re5::recovered::FUN_00407BC0_Object* observed_object = nullptr;
    re5::recovered::FUN_00407BC0_Node* head = nullptr;
};

re5::recovered::FUN_00407BC0_Node* first_node(
    void* context,
    re5::recovered::FUN_00407BC0_Object* object) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.calls;
    state.observed_object = object;
    return state.head;
}
} // namespace

void test_fun_00407bc0() {
    using namespace re5::recovered;

    FUN_00407BC0_Object object{0x12345678U};
    const FUN_00407BC0_Key key{0xAAAAAAAAU, 0x11223344U};

    // Portable-host guard only: the native body always dispatches vslot +0x10.
    FUN_00407BC0_SetServices(nullptr);
    assert(!FUN_00407BC0(object, key));

    State missing_callback_state{};
    FUN_00407BC0_Services missing_callback{&missing_callback_state, nullptr};
    FUN_00407BC0_SetServices(&missing_callback);
    assert(!FUN_00407BC0(object, key));
    assert(missing_callback_state.calls == 0U);

    // Portable-host guard only: native callers are expected to provide a
    // valid first node; avoid dereferencing a null harness result.
    State null_head_state{};
    FUN_00407BC0_Services null_head{&null_head_state, first_node};
    FUN_00407BC0_SetServices(&null_head);
    assert(!FUN_00407BC0(object, key));
    assert(null_head_state.calls == 1U);
    assert(null_head_state.observed_object == &object);

    // Native first-node hit returns true immediately without following +0x10.
    FUN_00407BC0_Node first{
        0U,
        key.value,
        0U,
        0U,
        reinterpret_cast<FUN_00407BC0_Node*>(static_cast<std::uintptr_t>(1U)),
    };
    State first_hit_state{0U, nullptr, &first};
    FUN_00407BC0_Services services{&first_hit_state, first_node};
    FUN_00407BC0_SetServices(&services);
    assert(FUN_00407BC0(object, key));
    assert(first_hit_state.calls == 1U);

    // Later-node hit walks node+0x10 links and returns true.
    FUN_00407BC0_Node third{0U, key.value, 0U, 0U, nullptr};
    FUN_00407BC0_Node second{0U, 0x55667788U, 0U, 0U, &third};
    first = FUN_00407BC0_Node{0U, 0xDEADBEEFU, 0U, 0U, &second};
    State later_hit_state{0U, nullptr, &first};
    services.context = &later_hit_state;
    FUN_00407BC0_SetServices(&services);
    assert(FUN_00407BC0(object, key));
    assert(later_hit_state.calls == 1U);

    // Exhausting the chain without a key match is the native false return.
    third.value = 0xCAFEBABEU;
    State miss_state{0U, nullptr, &first};
    services.context = &miss_state;
    FUN_00407BC0_SetServices(&services);
    assert(!FUN_00407BC0(object, key));
    assert(miss_state.calls == 1U);

    FUN_00407BC0_SetServices(nullptr);
}
