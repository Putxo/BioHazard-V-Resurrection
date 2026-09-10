#include "re5/recovered/fun_00401910.hpp"

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

void test_fun_00401910() {
    using namespace re5::recovered;

    bool cached_state = false;
    bool positive_return_state = false;
    void* object = reinterpret_cast<void*>(0x16E15C8);
    const FUN_00401910_Services services{
        nullptr,
        object,
        &cached_state,
        &positive_return_state,
        &probe_object,
    };

    g_probe_calls = 0;
    g_seen_object = nullptr;
    g_probe_result = true;
    positive_return_state = true;
    FUN_00401910_SetServices(&services);
    assert(FUN_00401910());
    FUN_00401910_SetServices(nullptr);
    assert(cached_state);
    assert(g_probe_calls == 1);
    assert(g_seen_object == object);

    cached_state = false;
    g_probe_calls = 0;
    g_probe_result = true;
    positive_return_state = false;
    FUN_00401910_SetServices(&services);
    assert(!FUN_00401910());
    FUN_00401910_SetServices(nullptr);
    assert(cached_state);
    assert(g_probe_calls == 1);

    cached_state = true;
    g_probe_calls = 0;
    g_probe_result = false;
    positive_return_state = true;
    FUN_00401910_SetServices(&services);
    assert(!FUN_00401910());
    FUN_00401910_SetServices(nullptr);
    assert(!cached_state);
    assert(g_probe_calls == 1);

    cached_state = true;
    const FUN_00401910_Services null_object_services{
        nullptr,
        nullptr,
        &cached_state,
        &positive_return_state,
        &probe_object,
    };
    g_probe_calls = 0;
    FUN_00401910_SetServices(&null_object_services);
    assert(!FUN_00401910());
    FUN_00401910_SetServices(nullptr);
    assert(!cached_state);
    assert(g_probe_calls == 0);
}
