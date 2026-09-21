#include "re5/recovered/region_00409420.hpp"

namespace re5::recovered {

std::uintptr_t FUN_00409420() noexcept { return 0x01657DD0U; }

bool FUN_00409480(const FUN_00409480_Object* self) noexcept {
    return self->state_58 == 2U || self->state_58 == 5U;
}

bool FUN_004094A0(const FUN_00409480_Object* self) noexcept {
    return self->state_58 == 3U || self->state_58 == 4U;
}

bool FUN_004094C0(const FUN_00409480_Object* self) noexcept {
    return self->state_58 == 0U || self->state_58 == 3U || self->state_58 == 4U;
}

bool FUN_004094E0(const FUN_00409480_Object* self) noexcept {
    return self->state_58 == 1U || self->state_58 == 2U || self->state_58 == 5U;
}

bool FUN_00409500(const FUN_00409480_Object* self) noexcept {
    return self->value_60 == 0U && self->byte_64 != 0U;
}

std::uint32_t FUN_00409520(const FUN_00409480_Object* self) noexcept {
    return self->value_08;
}

bool FUN_00409530(const FUN_00409480_Object* self) noexcept {
    const std::uint32_t value = self->flags_04;
    return (value & 0x7U) == 2U && (value & 0x400U) != 0U;
}

bool FUN_00409550(const FUN_00409480_Object* self) noexcept {
    const std::uint32_t value = self->flags_04;
    const std::uint8_t masked_low = static_cast<std::uint8_t>(value) & 0x7U;
    const std::uint8_t shifted = static_cast<std::uint8_t>(value >> 10U);
    return masked_low == 2U && (masked_low & shifted) != 0U && (shifted & 0x10U) != 0U;
}

float* FUN_004095D0(FUN_00409480_Object* self) noexcept {
    return self->vector_30;
}

std::uint8_t FUN_004095E0(const FUN_00409480_Object* self) noexcept {
    return self->byte_21;
}

} // namespace re5::recovered
