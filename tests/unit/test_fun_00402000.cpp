#include "re5/recovered/fun_00402000.hpp"

#include <cassert>

void test_fun_00402000() {
    using namespace re5::recovered;

    assert(FUN_00402000() == 0x016E6EC0U);
    assert(FUN_00402010() == 0x98U);
}
