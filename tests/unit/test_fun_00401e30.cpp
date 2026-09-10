#include "re5/recovered/fun_00401d50.hpp"
#include "re5/recovered/fun_00401e30.hpp"

#include <cassert>

namespace {
struct Probe {
    re5::recovered::FUN_00401D50_Object storage{};
    re5::recovered::FUN_00401D50_Object* global = nullptr;
    unsigned size = 0;
    unsigned alignment = 0;
    int allocations = 0;
    int publishes = 0;
};

re5::recovered::FUN_00401D50_Object* get_global_instance(void* context) noexcept {
    return static_cast<Probe*>(context)->global;
}

re5::recovered::FUN_00401D50_Object* allocate_instance(void* context, unsigned size, unsigned alignment) noexcept {
    auto& probe = *static_cast<Probe*>(context);
    ++probe.allocations;
    probe.size = size;
    probe.alignment = alignment;
    return &probe.storage;
}

void publish_global_instance(void* context, re5::recovered::FUN_00401D50_Object* object) noexcept {
    auto& probe = *static_cast<Probe*>(context);
    ++probe.publishes;
    probe.global = object;
}
} // namespace

void test_fun_00401e30() {
    using namespace re5::recovered;

    Probe probe{};
    const FUN_00401D50_Services constructor_services{
        &probe,
        &publish_global_instance,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
    };
    const FUN_00401E30_Services services{
        &probe,
        &get_global_instance,
        &allocate_instance,
    };

    FUN_00401D50_SetServices(&constructor_services);
    FUN_00401E30_SetServices(&services);
    assert(FUN_00401E30() == &probe.storage);
    assert(FUN_00401E30() == &probe.storage);
    FUN_00401E30_SetServices(nullptr);
    FUN_00401D50_SetServices(nullptr);

    assert(probe.allocations == 1);
    assert(probe.publishes == 1);
    assert(probe.size == 0x4CU);
    assert(probe.alignment == 0x10U);
}
