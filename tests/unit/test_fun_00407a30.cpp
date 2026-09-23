#include "re5/recovered/fun_00407a30.hpp"

#include <cassert>
#include <cstdint>
#include <cstring>

namespace {
struct State {
    unsigned marker_calls = 0U;
    unsigned convert_calls = 0U;
    unsigned manager_calls = 0U;
    const char* marker = nullptr;
    std::uintptr_t manager_token = 0U;
    char sequence[4] = {};
    unsigned sequence_size = 0U;
};

void emit_conversion_marker(void* context, const char* marker) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.marker_calls;
    state.marker = marker;
    state.sequence[state.sequence_size++] = 'L';
}

void convert_game_resources(void* context) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.convert_calls;
    state.sequence[state.sequence_size++] = 'C';
}

void dispatch_manager_slot_24(void* context, std::uintptr_t token) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.manager_calls;
    state.manager_token = token;
    state.sequence[state.sequence_size++] = 'M';
}

re5::recovered::FUN_00407A30_Object make_object(std::uint32_t value) {
    re5::recovered::FUN_00407A30_Object object{};
    object.state = value;
    return object;
}
} // namespace

void test_fun_00407a30() {
    using namespace re5::recovered;

    // Host-only guard: the native thiscall has no services pointer.
    auto object = make_object(1U);
    FUN_00407A30_SetServices(nullptr);
    FUN_00407A30(object);
    assert(object.state == 1U);

    // Host-only callback guard for the marker path.
    State missing_marker_state{};
    FUN_00407A30_Services missing_marker{
        &missing_marker_state, nullptr, convert_game_resources, dispatch_manager_slot_24};
    object = make_object(0U);
    FUN_00407A30_SetServices(&missing_marker);
    FUN_00407A30(object);
    assert(object.state == 0U);
    assert(missing_marker_state.marker_calls == 0U);

    // Native states 0, 1 and 2 all emit the same marker and increment state exactly once.
    State marker_state{};
    FUN_00407A30_Services services{
        &marker_state,
        emit_conversion_marker,
        convert_game_resources,
        dispatch_manager_slot_24};
    FUN_00407A30_SetServices(&services);
    for (std::uint32_t value = 0U; value <= 2U; ++value) {
        marker_state = State{};
        object = make_object(value);
        FUN_00407A30(object);
        assert(object.state == value + 1U);
        assert(marker_state.marker_calls == 1U);
        assert(marker_state.convert_calls == 0U);
        assert(marker_state.manager_calls == 0U);
        assert(marker_state.marker != nullptr);
        assert(std::strcmp(marker_state.marker, "CONVERT GAME RESOURCE ...") == 0);
        assert(marker_state.sequence_size == 1U);
        assert(marker_state.sequence[0] == 'L');
    }

    // Native state 3 converts the resource group, then invokes manager vslot +0x24.
    State phase3_state{};
    services.context = &phase3_state;
    FUN_00407A30_SetServices(&services);
    object = make_object(3U);
    FUN_00407A30(object);
    assert(object.state == 3U);
    assert(phase3_state.marker_calls == 0U);
    assert(phase3_state.convert_calls == 1U);
    assert(phase3_state.manager_calls == 1U);
    assert(phase3_state.manager_token == 0x01657F80U);
    assert(phase3_state.sequence_size == 2U);
    assert(phase3_state.sequence[0] == 'C');
    assert(phase3_state.sequence[1] == 'M');

    // Host-only guard: do not execute half of the native state-3 pair.
    State missing_dispatch_state{};
    FUN_00407A30_Services missing_dispatch{
        &missing_dispatch_state, emit_conversion_marker, convert_game_resources, nullptr};
    FUN_00407A30_SetServices(&missing_dispatch);
    object = make_object(3U);
    FUN_00407A30(object);
    assert(object.state == 3U);
    assert(missing_dispatch_state.convert_calls == 0U);
    assert(missing_dispatch_state.manager_calls == 0U);

    // Every unsigned state above 3 is a native no-op.
    State noop_state{};
    services.context = &noop_state;
    FUN_00407A30_SetServices(&services);
    object = make_object(4U);
    FUN_00407A30(object);
    assert(object.state == 4U);
    assert(noop_state.marker_calls == 0U);
    assert(noop_state.convert_calls == 0U);
    assert(noop_state.manager_calls == 0U);

    object = make_object(0xFFFFFFFFU);
    FUN_00407A30(object);
    assert(object.state == 0xFFFFFFFFU);
    assert(noop_state.marker_calls == 0U);
    assert(noop_state.convert_calls == 0U);
    assert(noop_state.manager_calls == 0U);

    FUN_00407A30_SetServices(nullptr);
}
