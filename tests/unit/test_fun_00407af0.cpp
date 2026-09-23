#include "re5/recovered/fun_00407af0.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>

namespace {
struct State {
    std::size_t calls = 0U;
    bool saw_destination_before_override = false;
    const re5::recovered::FUN_00407AF0_Object* source = nullptr;
};

void copy_exception_base(
    void* context,
    re5::recovered::FUN_00407AF0_Object* destination,
    const re5::recovered::FUN_00407AF0_Object* source) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.calls;
    state.saw_destination_before_override =
        destination != nullptr && destination->vtable == 0x11111111U;
    state.source = source;

    assert(destination != nullptr);
    assert(source != nullptr);

    // Model the native base copy constructor at 0x01251B8B:
    // it installs the std::exception vtable and copies the two base fields.
    destination->vtable = 0x01541FACU;
    destination->message = source->message;
    destination->owns_message = source->owns_message;
}
} // namespace

void test_fun_00407af0() {
    using namespace re5::recovered;

    const FUN_00407AF0_Object source{
        0x01541F48U,
        0x12345678U,
        1U,
    };

    // Portable-host guard only: the native body always calls 0x01251B8B.
    FUN_00407AF0_Object no_services{
        0x11111111U,
        0xAAAAAAAAU,
        0U,
    };
    FUN_00407AF0_SetServices(nullptr);
    assert(FUN_00407AF0(no_services, source) == &no_services);
    assert(no_services.vtable == 0x11111111U);
    assert(no_services.message == 0xAAAAAAAAU);
    assert(no_services.owns_message == 0U);

    // Portable-host guard only: missing base-copy callback.
    State missing_state{};
    FUN_00407AF0_Services missing_services{&missing_state, nullptr};
    FUN_00407AF0_Object missing{
        0x11111111U,
        0xBBBBBBBBU,
        0U,
    };
    FUN_00407AF0_SetServices(&missing_services);
    assert(FUN_00407AF0(missing, source) == &missing);
    assert(missing_state.calls == 0U);
    assert(missing.vtable == 0x11111111U);
    assert(missing.message == 0xBBBBBBBBU);

    // Native order: invoke the std::exception copy constructor first, then
    // overwrite slot +0 with the std::bad_alloc vtable and return this.
    State state{};
    FUN_00407AF0_Services services{&state, copy_exception_base};
    FUN_00407AF0_Object destination{
        0x11111111U,
        0U,
        0U,
    };
    FUN_00407AF0_SetServices(&services);

    assert(FUN_00407AF0(destination, source) == &destination);
    assert(state.calls == 1U);
    assert(state.saw_destination_before_override);
    assert(state.source == &source);
    assert(destination.vtable == 0x01541F48U);
    assert(destination.message == source.message);
    assert(destination.owns_message == source.owns_message);

    FUN_00407AF0_SetServices(nullptr);
}
