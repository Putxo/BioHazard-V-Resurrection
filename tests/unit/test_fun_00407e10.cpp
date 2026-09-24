#include "re5/recovered/fun_00407e10.hpp"

#include <cassert>
#include <cstdint>
#include <cstring>

namespace {
struct State {
    std::uint8_t global_force_flag = 0U;
    unsigned global_reads = 0U;
    unsigned enter_calls = 0U;
    unsigned leave_calls = 0U;
    void* critical_section = nullptr;
    const std::uint32_t* observed_slot = nullptr;
    std::uint32_t expected_value = 0U;
};

std::uint8_t read_global_force_flag(void* context) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.global_reads;
    return state.global_force_flag;
}

void enter_critical_section(
    void* context,
    void* critical_section) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.enter_calls;
    state.critical_section = critical_section;
    if (state.observed_slot != nullptr) {
        assert(*state.observed_slot != state.expected_value);
    }
}

void leave_critical_section(
    void* context,
    void* critical_section) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.leave_calls;
    state.critical_section = critical_section;
    if (state.observed_slot != nullptr) {
        assert(*state.observed_slot == state.expected_value);
    }
}

std::uint32_t read_slot(
    const re5::recovered::FUN_00407E10_Object& object,
    std::uint32_t index) noexcept {
    std::uint32_t value = 0U;
    const std::uint32_t offset =
        re5::recovered::FUN_00407E10_ENTRY_BASE +
        index * re5::recovered::FUN_00407E10_ENTRY_STRIDE;
    std::memcpy(
        &value,
        reinterpret_cast<const std::uint8_t*>(&object) + offset,
        sizeof(value));
    return value;
}
} // namespace

void test_fun_00407e10() {
    using namespace re5::recovered;

    FUN_00407E10_Object object{};

    // Harness-only service guard: native imports/globals always exist. Missing
    // host services must never suppress the unconditional native store.
    FUN_00407E10_SetServices(nullptr);
    object.active_flag = 0U;
    object.current_index = 0U;
    FUN_00407E10(object, 0x11223344U);
    assert(read_slot(object, 0U) == 0x11223344U);

    // Harness-only missing-global-reader guard. Treat the unavailable host
    // force flag as inactive while preserving the store at the selected slot.
    State no_reader_state{};
    FUN_00407E10_Services no_reader{
        &no_reader_state,
        nullptr,
        enter_critical_section,
        leave_critical_section,
    };
    FUN_00407E10_SetServices(&no_reader);
    object.active_flag = 0U;
    object.current_index = 1U;
    FUN_00407E10(object, 0x55667788U);
    assert(no_reader_state.enter_calls == 0U);
    assert(no_reader_state.leave_calls == 0U);
    assert(read_slot(object, 1U) == 0x55667788U);

    // Native synchronization-bypass path: both gates are zero. The global is
    // read once, no lock calls occur, and this+0x70+this[0x20]*0x8C74 is
    // still written.
    State inactive_state{};
    FUN_00407E10_Services services{
        &inactive_state,
        read_global_force_flag,
        enter_critical_section,
        leave_critical_section,
    };
    FUN_00407E10_SetServices(&services);
    object.active_flag = 0U;
    object.current_index = 1U;
    FUN_00407E10(object, 0x01020304U);
    assert(inactive_state.global_reads == 1U);
    assert(inactive_state.enter_calls == 0U);
    assert(inactive_state.leave_calls == 0U);
    assert(read_slot(object, 1U) == 0x01020304U);

    // Object-active path short-circuits the global read. Enter must observe
    // the old slot and Leave the new slot, using exactly this+4.
    State object_active_state{};
    object_active_state.observed_slot =
        reinterpret_cast<const std::uint32_t*>(object.entry_storage);
    object_active_state.expected_value = 0xAABBCCDDU;
    services.context = &object_active_state;
    FUN_00407E10_SetServices(&services);
    object.active_flag = 1U;
    object.current_index = 0U;
    FUN_00407E10(object, object_active_state.expected_value);
    assert(object_active_state.global_reads == 0U);
    assert(object_active_state.enter_calls == 1U);
    assert(object_active_state.leave_calls == 1U);
    assert(object_active_state.critical_section ==
           static_cast<void*>(object.critical_section));
    assert(read_slot(object, 0U) == object_active_state.expected_value);

    // Global-force path locks when the object byte is clear and current_index
    // selects the second modeled stride.
    State global_active_state{};
    global_active_state.global_force_flag = 1U;
    global_active_state.observed_slot =
        reinterpret_cast<const std::uint32_t*>(
            object.entry_storage + FUN_00407E10_ENTRY_STRIDE);
    global_active_state.expected_value = 0xCAFEBABEU;
    services.context = &global_active_state;
    FUN_00407E10_SetServices(&services);
    object.active_flag = 0U;
    object.current_index = 1U;
    FUN_00407E10(object, global_active_state.expected_value);
    assert(global_active_state.global_reads == 1U);
    assert(global_active_state.enter_calls == 1U);
    assert(global_active_state.leave_calls == 1U);
    assert(global_active_state.critical_section ==
           static_cast<void*>(object.critical_section));
    assert(read_slot(object, 1U) == global_active_state.expected_value);
    assert(object.current_index == 1U);

    // Harness-only incomplete callback pair: suppress both callbacks rather
    // than enter without a matching leave; the native store remains.
    State incomplete_state{};
    incomplete_state.global_force_flag = 1U;
    FUN_00407E10_Services incomplete{
        &incomplete_state,
        read_global_force_flag,
        nullptr,
        leave_critical_section,
    };
    FUN_00407E10_SetServices(&incomplete);
    object.active_flag = 0U;
    object.current_index = 0U;
    FUN_00407E10(object, 0xDEADBEEFU);
    assert(incomplete_state.global_reads == 1U);
    assert(incomplete_state.enter_calls == 0U);
    assert(incomplete_state.leave_calls == 0U);
    assert(read_slot(object, 0U) == 0xDEADBEEFU);

    FUN_00407E10_SetServices(nullptr);
}
