#include "re5/recovered/fun_00407b50.hpp"

#include <cassert>
#include <cstdint>

void test_fun_00407b50() {
    using namespace re5::recovered;

    FUN_00407B50_Object object{};

    // Native null branch: [this+0x1028] == 0 returns the fixed fallback token.
    object.string_object = 0U;
    assert(FUN_00407B50(object) == 0x0137AA5CU);

    // Native non-null path: return the payload address at object+8.
    object.string_object = 0x00123400U;
    assert(FUN_00407B50(object) == 0x00123408U);

    // The original x86 ADD is 32-bit; unsigned wrap preserves EAX semantics.
    object.string_object = 0xFFFFFFFCU;
    assert(FUN_00407B50(object) == 0x00000004U);
}
