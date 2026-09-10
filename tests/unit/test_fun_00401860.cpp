#include "re5/recovered/fun_00401860.hpp"

#include <cassert>

namespace {
int g_calls = 0;
void* g_last_object = nullptr;
bool g_result = false;

bool invoke_slot_04(void* object) noexcept {
    ++g_calls;
    g_last_object = object;
    return g_result;
}
}

void test_fun_00401860() {
    using namespace re5::recovered;

    const FUN_00401860_Services services{&invoke_slot_04};
    FUN_00401860_SetServices(&services);

    g_calls = 0;
    g_last_object = nullptr;
    g_result = true;
    int object = 123;
    assert(FUN_00401860(&object));
    assert(g_calls == 1);
    assert(g_last_object == &object);

    g_result = false;
    assert(!FUN_00401860(&object));
    assert(g_calls == 2);

    assert(!FUN_00401860(nullptr));
    assert(g_calls == 2);

    FUN_00401860_SetServices(nullptr);
    assert(!FUN_00401860(&object));
}
