#include "re5/recovered/fun_00407b30.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>

namespace {
struct State {
    std::uint8_t global_force_flag = 0U;
    unsigned global_reads = 0U;
    unsigned leave_calls = 0U;
    void* critical_section = nullptr;
};

std::uint8_t read_global_force_flag(void* context) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.global_reads;
    return state.global_force_flag;
}

void leave_critical_section(void* context, void* critical_section) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.leave_calls;
    state.critical_section = critical_section;
}
} // namespace

void test_fun_00407b30() {
    using namespace re5::recovered;

    FUN_00407B30_Object object{};

    // Harness-only guard: native code has no injected service pointer.
    FUN_00407B30_SetServices(nullptr);
    FUN_00407B30(object);

    // Harness-only guard for the injected global-byte reader.
    State no_reader_state{};
    FUN_00407B30_Services no_reader{
        &no_reader_state, nullptr, leave_critical_section};
    FUN_00407B30_SetServices(&no_reader);
    FUN_00407B30(object);
    assert(no_reader_state.leave_calls == 0U);

    // Native early exit: object flag == 0 and global 0x016E2090 == 0.
    State inactive_state{};
    FUN_00407B30_Services services{
        &inactive_state, read_global_force_flag, leave_critical_section};
    FUN_00407B30_SetServices(&services);
    object.active_flag = 0U;
    FUN_00407B30(object);
    assert(inactive_state.global_reads == 1U);
    assert(inactive_state.leave_calls == 0U);

    // Native active-object path branches before reading the global flag.
    State object_active_state{};
    services.context = &object_active_state;
    FUN_00407B30_SetServices(&services);
    object.active_flag = 1U;
    FUN_00407B30(object);
    assert(object_active_state.global_reads == 0U);
    assert(object_active_state.leave_calls == 1U);
    assert(object_active_state.critical_section ==
           static_cast<void*>(object.critical_section));

    // Native global-force path leaves when object flag is clear.
    State global_active_state{};
    global_active_state.global_force_flag = 1U;
    services.context = &global_active_state;
    FUN_00407B30_SetServices(&services);
    object.active_flag = 0U;
    FUN_00407B30(object);
    assert(global_active_state.global_reads == 1U);
    assert(global_active_state.leave_calls == 1U);
    assert(global_active_state.critical_section ==
           static_cast<void*>(object.critical_section));

    // Harness-only callback guard.
    State no_leave_state{};
    no_leave_state.global_force_flag = 1U;
    FUN_00407B30_Services no_leave{
        &no_leave_state, read_global_force_flag, nullptr};
    FUN_00407B30_SetServices(&no_leave);
    object.active_flag = 0U;
    FUN_00407B30(object);
    assert(no_leave_state.global_reads == 1U);
    assert(no_leave_state.leave_calls == 0U);

    FUN_00407B30_SetServices(nullptr);
}
