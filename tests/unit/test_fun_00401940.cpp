#include "re5/recovered/fun_00401940.hpp"

#include <cassert>

void test_fun_00401940() {
    using namespace re5::recovered;

    bool positive_return_state = false;
    const FUN_00401940_Services services{
        &positive_return_state,
    };

    FUN_00401940_SetServices(&services);
    FUN_00401940(true);
    assert(positive_return_state);

    FUN_00401940(false);
    assert(!positive_return_state);

    FUN_00401940_SetServices(nullptr);
    FUN_00401940(true);
    assert(!positive_return_state);
}
