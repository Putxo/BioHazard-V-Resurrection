#include "re5/recovered/fun_00401fe0.hpp"

#include <cassert>
#include <cstring>

void test_fun_00401fe0() {
    using namespace re5::recovered;

    char buffer[64]{};
    const int result = FUN_00401FE0(buffer, "Award %d %s: '%s'", 7, "Online", "Knife");
    assert(result > 0);
    assert(std::strcmp(buffer, "Award 7 Online: 'Knife'") == 0);

    char small[4]{};
    assert(FUN_00401FE0(small, "%s", "abc") == 3);
    assert(std::strcmp(small, "abc") == 0);
}
