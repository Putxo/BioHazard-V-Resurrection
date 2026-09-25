#include "re5/recovered/fun_004086a0.hpp"

#include <cassert>
#include <cstdint>

void test_fun_004086a0() {
    using namespace re5::recovered;

    // Portable guard only: native code assumes a valid this pointer.
    assert(FUN_004086A0(nullptr) == nullptr);

    FUN_004086A0_Object object{
        0xAAAAAAAAU,
        0xBBBBBBBBU,
        0xCCCCCCCCU,
        0xDDDDDDDDU,
        0xEEEEEEEEU,
        0x12345678U,
        -1.0F,
    };

    assert(FUN_004086A0(&object) == &object);
    assert(object.vtable == 0x0137E458U);
    assert(object.state == 0U);
    assert(object.object_a == 0U);
    assert(object.object_b == 0U);
    assert(object.sentinel == 0xFFFFFFFFU);
    assert(object.preserved == 0x12345678U);
    assert(object.scalar == 1000.0F);

    // Native path has no conditional early exit. Re-running it must restore
    // every written field while still preserving the untouched +0x14 dword.
    object.vtable = 0U;
    object.state = 0xFFFFFFFFU;
    object.object_a = 1U;
    object.object_b = 2U;
    object.sentinel = 0U;
    object.preserved = 0xCAFEBABEU;
    object.scalar = 0.0F;

    assert(FUN_004086A0(&object) == &object);
    assert(object.vtable == 0x0137E458U);
    assert(object.state == 0U);
    assert(object.object_a == 0U);
    assert(object.object_b == 0U);
    assert(object.sentinel == 0xFFFFFFFFU);
    assert(object.preserved == 0xCAFEBABEU);
    assert(object.scalar == 1000.0F);
}
