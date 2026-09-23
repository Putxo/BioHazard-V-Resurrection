#include "re5/recovered/fun_00407b10.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>

namespace {
struct State {
    std::uint8_t global_force_flag = 0U;
    unsigned global_reads = 0U;
    unsigned enter_calls = 0U;
    void* critical_section = nullptr;
};

std::uint8_t read_global_force_flag(void* context) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.global_reads;
    return state.global_force_flag;
}

void enter_critical_section(void* context, void* critical_section) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.enter_calls;
    state.critical_section = critical_section;
}
} // namespace

void test_fun_00407b10() {
    using namespace re5::recovered;

    FUN_00407B10_Object object{};

    // Harness-only guard: the native thiscall has no services pointer.
    FUN_00407B10_SetServices(nullptr);
    FUN_00407B10(object);

    // Harness-only guard for the injected global-byte reader.
    State no_reader_state{};
    FUN_00407B10_Services no_reader{
        &no_reader_state, nullptr, enter_critical_section};
    FUN_00407B10_SetServices(&no_reader);
    FUN_00407B10(object);
    assert(no_reader_state.enter_calls == 0U);

    // Native early exit: object flag == 0 and global 0x016E2090 == 0.
    State inactive_state{};
    FUN_00407B10_Services services{
        &inactive_state, read_global_force_flag, enter_critical_section};
    FUN_00407B10_SetServices(&services);
    object.active_flag = 0U;
    FUN_00407B10(object);
    assert(inactive_state.global_reads == 1U);
    assert(inactive_state.enter_calls == 0U);

    // Native active-object path branches before reading the global flag.
    State object_active_state{};
    services.context = &object_active_state;
    FUN_00407B10_SetServices(&services);
    object.active_flag = 1U;
    FUN_00407B10(object);
    assert(object_active_state.global_reads == 0U);
    assert(object_active_state.enter_calls == 1U);
    assert(object_active_state.critical_section ==
           static_cast<void*>(object.critical_section));

    // Native global-force path enters when object flag is clear.
    State global_active_state{};
    global_active_state.global_force_flag = 1U;
    services.context = &global_active_state;
    FUN_00407B10_SetServices(&services);
    object.active_flag = 0U;
    FUN_00407B10(object);
    assert(global_active_state.global_reads == 1U);
    assert(global_active_state.enter_calls == 1U);
    assert(global_active_state.critical_section ==
           static_cast<void*>(object.critical_section));

    // Harness-only callback guard: avoid calling a missing host import shim.
    State no_enter_state{};
    no_enter_state.global_force_flag = 1U;
    FUN_00407B10_Services no_enter{
        &no_enter_state, read_global_force_flag, nullptr};
    FUN_00407B10_SetServices(&no_enter);
    object.active_flag = 0U;
    FUN_00407B10(object);
    assert(no_enter_state.global_reads == 1U);
    assert(no_enter_state.enter_calls == 0U);

    FUN_00407B10_SetServices(nullptr);
}
