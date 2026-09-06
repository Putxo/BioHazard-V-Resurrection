#include "re5/recovered/pilot_bb9070.hpp"
#include <cassert>

void test_pilot_bb9070_accessors() {
    const re5::recovered::UnknownBB90Object object{0x12345678u, 0x89ABCDEFu};
    assert(re5::recovered::FUN_00BB9070(&object) == 0x12345678u);
    assert(re5::recovered::FUN_00BB9080(&object) == 0x89ABCDEFu);
    assert(re5::recovered::FUN_00BB9090(&object) == 0x89ABCDEFu);
}
