#include "re5/recovered/fun_00401d50.hpp"

#include <cassert>
#include <cstdint>
#include <vector>

namespace {
struct Probe {
    re5::recovered::FUN_00401D50_Object* published = nullptr;
    std::vector<std::uintptr_t> registered_callbacks;
    bool has_stats = true;
    bool has_user = true;
    bool logged_on = true;
    int request_stats_calls = 0;
};

void publish_global_instance(void* context, re5::recovered::FUN_00401D50_Object* object) noexcept {
    static_cast<Probe*>(context)->published = object;
}

void register_callback(
    void* context,
    re5::recovered::FUN_00401D50_Object*,
    re5::recovered::FUN_00401D50_CallbackSlot*,
    std::uintptr_t callback_va) noexcept {
    static_cast<Probe*>(context)->registered_callbacks.push_back(callback_va);
}

bool has_user_stats(void* context) noexcept {
    return static_cast<Probe*>(context)->has_stats;
}

bool has_user(void* context) noexcept {
    return static_cast<Probe*>(context)->has_user;
}

bool is_user_logged_on(void* context) noexcept {
    return static_cast<Probe*>(context)->logged_on;
}

void request_current_stats(void* context) noexcept {
    ++static_cast<Probe*>(context)->request_stats_calls;
}
} // namespace

void test_fun_00401d50() {
    using namespace re5::recovered;

    Probe probe{};
    const FUN_00401D50_Services services{
        &probe,
        &publish_global_instance,
        &register_callback,
        &has_user_stats,
        &has_user,
        &is_user_logged_on,
        &request_current_stats,
    };

    FUN_00401D50_Object object{};
    object.summary.awarded_count = 77;
    object.summary.saved_count = 88;

    FUN_00401D50_SetServices(&services);
    assert(FUN_00401D50(object) == &object);
    FUN_00401D50_SetServices(nullptr);

    assert(probe.published == &object);
    assert(probe.registered_callbacks.size() == 3);
    assert(probe.registered_callbacks[0] == 0x00401A70U);
    assert(probe.registered_callbacks[1] == 0x00401230U);
    assert(probe.registered_callbacks[2] == 0x00401240U);
    assert(probe.request_stats_calls == 1);
    assert(object.slot_10.owner == &object);
    assert(object.slot_24.owner == &object);
    assert(object.slot_38.owner == &object);
    assert(object.summary.awarded_count == 0);
    assert(object.summary.saved_count == 0xFFFFFFFFU);
}
