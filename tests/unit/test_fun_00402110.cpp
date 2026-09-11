#include "re5/recovered/fun_00402110.hpp"

#include <cassert>

void test_fun_00402110() {
    using namespace re5::recovered;

    FUN_00402110_Dword destination{0xAAAAAAAAU};
    const FUN_00402110_Dword source{0x13572468U};

    assert(FUN_00402110(destination, source) == &destination);
    assert(destination.value == 0x13572468U);
}
