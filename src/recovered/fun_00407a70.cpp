#include "re5/recovered/fun_00407a70.hpp"

#include <bit>

namespace re5::recovered {
namespace {

std::int32_t arithmetic_shift_right_two(std::uint32_t value) noexcept {
    std::uint32_t shifted = value >> 2U;
    if ((value & 0x80000000U) != 0U) {
        shifted |= 0xC0000000U;
    }
    return std::bit_cast<std::int32_t>(shifted);
}

} // namespace

std::int32_t FUN_00407A70(const FUN_00407A70_Object& object) noexcept {
    if (object.begin == 0U) {
        return 0;
    }

    const std::uint32_t byte_delta = object.end - object.begin;
    return arithmetic_shift_right_two(byte_delta);
}

} // namespace re5::recovered
