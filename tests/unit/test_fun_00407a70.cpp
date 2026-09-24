#include "re5/recovered/fun_00407a70.hpp"

#include <cassert>
#include <cstdint>

void test_fun_00407a70() {
    using namespace re5::recovered;

    // Native early exit: a null begin pointer returns zero without using end.
    FUN_00407A70_Object object{0xDEADBEEFU, 0U, 0x12345678U};
    assert(FUN_00407A70(object) == 0);

    // Three 4-byte elements.
    object = FUN_00407A70_Object{0U, 0x00001000U, 0x0000100CU};
    assert(FUN_00407A70(object) == 3);

    // A non-null empty range returns zero.
    object = FUN_00407A70_Object{0xFFFFFFFFU, 0x00002000U, 0x00002000U};
    assert(FUN_00407A70(object) == 0);

    // Preserve the target's signed arithmetic shift for a negative delta.
    object = FUN_00407A70_Object{0U, 0x00001000U, 0x00000FFCU};
    assert(FUN_00407A70(object) == -1);

    // The native SAR also rounds a negative non-multiple delta toward -infinity.
    object = FUN_00407A70_Object{0U, 0x00001000U, 0x00000FFFU};
    assert(FUN_00407A70(object) == -1);

    // Dword subtraction wraps exactly as on the 32-bit target.
    object = FUN_00407A70_Object{0U, 0xFFFFFFFCU, 0x00000004U};
    assert(FUN_00407A70(object) == 2);

    // The leading dword is not read by this helper.
    object = FUN_00407A70_Object{0x13579BDFU, 0x00003000U, 0x00003010U};
    assert(FUN_00407A70(object) == 4);
}
