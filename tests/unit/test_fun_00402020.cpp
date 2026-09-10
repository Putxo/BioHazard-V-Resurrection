#include "re5/recovered/fun_00402020.hpp"

#include <cassert>

namespace {
struct Probe {
    int unregister_calls = 0;
    int register_calls = 0;
    unsigned callback_id = 0;
};

void unregister_callback(void* context, re5::recovered::FUN_00402020_CallbackSlot*) noexcept {
    ++static_cast<Probe*>(context)->unregister_calls;
}

void register_callback(void* context, re5::recovered::FUN_00402020_CallbackSlot*, std::uint32_t callback_id) noexcept {
    auto& probe = *static_cast<Probe*>(context);
    ++probe.register_calls;
    probe.callback_id = callback_id;
}
} // namespace

void test_fun_00402020() {
    using namespace re5::recovered;

    Probe probe{};
    const FUN_00402020_Services services{
        &probe,
        &unregister_callback,
        &register_callback,
    };
    FUN_00402020_CallbackSlot slot{"", true, 0, nullptr, 0};
    int owner = 0;

    FUN_00402020_SetServices(&services);
    FUN_00402020(slot, &owner, 0x00401A70U);
    FUN_00402020_SetServices(nullptr);

    assert(probe.unregister_calls == 1);
    assert(probe.register_calls == 1);
    assert(probe.callback_id == 0x44DU);
    assert(slot.owner == &owner);
    assert(slot.callback_va == 0x00401A70U);
}
