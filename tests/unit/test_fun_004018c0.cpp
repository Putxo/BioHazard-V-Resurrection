#include "re5/recovered/fun_004018c0.hpp"

#include <cassert>

namespace {
int g_probe_calls = 0;
void* g_seen_object = nullptr;
bool g_probe_result = false;

bool probe_object(void*, void* object) noexcept {
    ++g_probe_calls;
    g_seen_object = object;
    return g_probe_result;
}
} // namespace

void test_fun_004018c0() {
    using namespace re5::recovered;

    bool cached_state = true;
    void* object = reinterpret_cast<void*>(0x16E15C8);
    const FUN_004018C0_Services services{
        nullptr,
        object,
        &cached_state,
        &probe_object,
    };

    g_probe_calls = 0;
    g_seen_object = nullptr;
    g_probe_result = true;
    FUN_004018C0_SetServices(&services);
    assert(FUN_004018C0());
    FUN_004018C0_SetServices(nullptr);
    assert(cached_state);
    assert(g_probe_calls == 1);
    assert(g_seen_object == object);

    cached_state = true;
    g_probe_calls = 0;
    g_probe_result = false;
    FUN_004018C0_SetServices(&services);
    assert(!FUN_004018C0());
    FUN_004018C0_SetServices(nullptr);
    assert(!cached_state);
    assert(g_probe_calls == 1);

    cached_state = true;
    const FUN_004018C0_Services null_object_services{
        nullptr,
        nullptr,
        &cached_state,
        &probe_object,
    };
    g_probe_calls = 0;
    FUN_004018C0_SetServices(&null_object_services);
    assert(!FUN_004018C0());
    FUN_004018C0_SetServices(nullptr);
    assert(!cached_state);
    assert(g_probe_calls == 0);
}
