#include "re5/recovered/fun_00408640.hpp"

#include <cassert>
#include <cstdint>
#include <type_traits>

void test_fun_00408640() {
    using namespace re5::recovered;

    static_assert(sizeof(FUN_00408640_Object) == 8U);
    static_assert(std::is_standard_layout_v<FUN_00408640_Object>);

    // Host-only guard. Native thiscall assumes ECX is a valid object.
    assert(FUN_00408640(nullptr) == nullptr);

    FUN_00408640_Object object{
        0xDEADBEEFU,
        0xA5A5A5A5U,
    };

    FUN_00408640_Object* result = FUN_00408640(&object);
    assert(result == &object);
    assert(object.vtable == 0x0137E440U);
    assert(object.state == 0U);

    object.vtable = 0xFFFFFFFFU;
    object.state = 0x12345678U;
    assert(FUN_00408640(&object) == &object);
    assert(object.vtable == 0x0137E440U);
    assert(object.state == 0U);
}
