#include "re5/recovered/pilot_bb9070.hpp"
#include <cassert>

void test_pilot_bb9070_accessors() {
    const re5::recovered::UnknownBB90Object tail{0x33333333u, nullptr};
    const re5::recovered::UnknownBB90Object middle{0x22222222u, &tail};
    const re5::recovered::UnknownBB90Object head{0x11111111u, &middle};
    const re5::recovered::UnknownBB90Object unrelated{0x44444444u, nullptr};

    assert(re5::recovered::FUN_00BB9070(&head) == 0x11111111u);
    assert(re5::recovered::FUN_00BB9080(&head) == &middle);
    assert(re5::recovered::FUN_00BB9090(&head) == &middle);

    bool found = false;
    assert(re5::recovered::FUN_00BB90C0(&middle, &found, &head) == &found);
    assert(found);

    found = false;
    assert(re5::recovered::FUN_00BB90C0(&head, &found, &head) == &found);
    assert(found);

    found = true;
    assert(re5::recovered::FUN_00BB90C0(&unrelated, &found, &head) == &found);
    assert(!found);

    found = true;
    assert(re5::recovered::FUN_00BB90C0(&tail, &found, nullptr) == &found);
    assert(!found);
}
