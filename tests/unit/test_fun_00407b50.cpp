#include "re5/recovered/fun_00407b50.hpp"

#include <cassert>
#include <cstdint>

void test_fun_00407b50() {
    using namespace re5::recovered;

    FUN_00407B50_Object object{};
    object.string_object = 0U;
    assert(FUN_00407B50(object) == 0x0137AA5CU);
    object.string_object = 0x00123400U;
    assert(FUN_00407B50(object) == 0x00123408U);
    object.string_object = 0xFFFFFFFCU;
    assert(FUN_00407B50(object) == 0x00000004U);
}
