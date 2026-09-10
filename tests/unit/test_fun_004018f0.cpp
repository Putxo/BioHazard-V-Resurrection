#include "re5/recovered/fun_004018f0.hpp"

#include <cassert>

namespace {
struct Probe {
    int false_calls{};
    int true_calls{};
    int true_argument{};
    void* seen_false_object{};
    void* seen_true_object{};
};

void dispatch_false_path(void* context, void* object) noexcept {
    auto& probe = *static_cast<Probe*>(context);
    ++probe.false_calls;
    probe.seen_false_object = object;
}

void dispatch_true_path(void* context, void* object, int argument) noexcept {
    auto& probe = *static_cast<Probe*>(context);
    ++probe.true_calls;
    probe.seen_true_object = object;
    probe.true_argument = argument;
}
} // namespace

void test_fun_004018f0() {
    using namespace re5::recovered;

    Probe probe{};
    void* object = reinterpret_cast<void*>(0x16E15C8);
    const FUN_004018F0_Services services{
        &probe,
        object,
        &dispatch_false_path,
        &dispatch_true_path,
    };

    FUN_004018F0_SetServices(&services);
    FUN_004018F0(false);
    FUN_004018F0(true);
    FUN_004018F0_SetServices(nullptr);

    assert(probe.false_calls == 1);
    assert(probe.true_calls == 1);
    assert(probe.seen_false_object == object);
    assert(probe.seen_true_object == object);
    assert(probe.true_argument == 1);

    Probe null_dispatch_probe{};
    const FUN_004018F0_Services null_dispatch_services{
        &null_dispatch_probe,
        object,
        nullptr,
        nullptr,
    };
    FUN_004018F0_SetServices(&null_dispatch_services);
    FUN_004018F0(false);
    FUN_004018F0(true);
    FUN_004018F0_SetServices(nullptr);
    assert(null_dispatch_probe.false_calls == 0);
    assert(null_dispatch_probe.true_calls == 0);
}
