#include "re5/recovered/region_00409420.hpp"

#include <cassert>

void test_region_00409420() {
    using namespace re5::recovered;
    assert(FUN_00409420() == 0x01657DD0U);

    FUN_00409480_Object object{};
    object.state_58 = 2U;
    assert(FUN_00409480(&object));
    assert(!FUN_004094A0(&object));
    assert(!FUN_004094C0(&object));
    assert(FUN_004094E0(&object));

    object.state_58 = 3U;
    assert(!FUN_00409480(&object));
    assert(FUN_004094A0(&object));
    assert(FUN_004094C0(&object));
    assert(!FUN_004094E0(&object));

    object.value_60 = 0U;
    object.byte_64 = 1U;
    assert(FUN_00409500(&object));
    object.value_60 = 1U;
    assert(!FUN_00409500(&object));

    object.value_08 = 0x12345678U;
    assert(FUN_00409520(&object) == 0x12345678U);

    object.flags_04 = 0x402U;
    assert(FUN_00409530(&object));
    object.flags_04 = 0x2U;
    assert(!FUN_00409530(&object));

    // 00409550 requires low three bits == 2, shifted byte bit 1 set,
    // and shifted byte bit 4 set. 0x4802 >> 10 == 0x12.
    object.flags_04 = 0x4802U;
    assert(FUN_00409550(&object));
    object.flags_04 = 0x4002U;
    assert(!FUN_00409550(&object));

    assert(FUN_004095D0(&object) == &object.vector_30[0]);
    object.byte_21 = 0x7AU;
    assert(FUN_004095E0(&object) == 0x7AU);
}
