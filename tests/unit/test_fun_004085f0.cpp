#include "re5/recovered/fun_004085f0.hpp"

#include <cassert>
#include <cstdint>

namespace {
bool fix_enabled(
    const re5::recovered::FUN_004085F0_Object& object) noexcept {
    return
        ((object.flags >> 13U) & 1U) != 0U;
}
} // namespace

void test_fun_004085f0() {
    using namespace re5::recovered;

    // Native nonzero branch and its early ret 4: set only bit 13 and leave
    // every unrelated bit plus the preceding dword untouched.
    FUN_004085F0_Object set_object{
        0xAABBCCDDU,
        0x40000005U,
    };

    FUN_004085F0(set_object, 1U);

    assert(set_object.reserved_00 == 0xAABBCCDDU);
    assert(set_object.flags == 0x40002005U);
    assert(fix_enabled(set_object));

    // Native code compares a byte, not a canonicalized C++ bool. Any nonzero
    // low byte takes the same set path.
    FUN_004085F0_Object noncanonical{
        0x11223344U,
        0x00000010U,
    };

    FUN_004085F0(noncanonical, 0x80U);

    assert(noncanonical.reserved_00 == 0x11223344U);
    assert(noncanonical.flags == 0x00002010U);
    assert(fix_enabled(noncanonical));

    // Native zero branch and its own ret 4: clear exactly bit 13 while
    // preserving all remaining flag bits.
    FUN_004085F0_Object clear_object{
        0x55667788U,
        0xFFFFFFFFU,
    };

    FUN_004085F0(clear_object, 0U);

    assert(clear_object.reserved_00 == 0x55667788U);
    assert(clear_object.flags == 0xFFFFDFFFU);
    assert(!fix_enabled(clear_object));

    // Both operations are idempotent and never touch any field other than
    // the 0x2000 bit at this+4.
    const std::uint32_t set_once = set_object.flags;
    FUN_004085F0(set_object, 0x7FU);
    assert(set_object.flags == set_once);

    const std::uint32_t clear_once = clear_object.flags;
    FUN_004085F0(clear_object, 0U);
    assert(clear_object.flags == clear_once);
}
