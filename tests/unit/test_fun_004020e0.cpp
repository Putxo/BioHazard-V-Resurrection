#include "re5/recovered/fun_004020e0.hpp"

#include <cassert>

void test_fun_004020e0() {
    using namespace re5::recovered;

    FUN_004020E0_SetGlobals(nullptr);
    assert(FUN_004020E0() == 0U);
    assert(FUN_004020F0() == 0U);

    const FUN_004020E0_Globals globals{0x12345678U, 0x87654321U};
    FUN_004020E0_SetGlobals(&globals);
    assert(FUN_004020E0() == 0x12345678U);
    assert(FUN_004020F0() == 0x87654321U);
    assert(FUN_00402100() == 0x016E6FF0U);
    FUN_004020E0_SetGlobals(nullptr);
}
