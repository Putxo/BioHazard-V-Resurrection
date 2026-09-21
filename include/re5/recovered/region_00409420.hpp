#pragma once

#include <cstddef>
#include <cstdint>

namespace re5::recovered {

struct FUN_00409480_Object {
    std::uint8_t pad_00[0x04];
    std::uint32_t flags_04;
    std::uint32_t value_08;
    std::uint8_t pad_0C[0x15];
    std::uint8_t byte_21;
    std::uint8_t pad_22[0x0E];
    float vector_30[3];
    std::uint8_t pad_3C[0x1C];
    std::uint32_t state_58;
    std::uint8_t pad_5C[0x04];
    std::uint32_t value_60;
    std::uint8_t byte_64;
};

static_assert(offsetof(FUN_00409480_Object, flags_04) == 0x04U);
static_assert(offsetof(FUN_00409480_Object, value_08) == 0x08U);
static_assert(offsetof(FUN_00409480_Object, byte_21) == 0x21U);
static_assert(offsetof(FUN_00409480_Object, vector_30) == 0x30U);
static_assert(offsetof(FUN_00409480_Object, state_58) == 0x58U);
static_assert(offsetof(FUN_00409480_Object, value_60) == 0x60U);
static_assert(offsetof(FUN_00409480_Object, byte_64) == 0x64U);

[[nodiscard]] std::uintptr_t FUN_00409420() noexcept;
[[nodiscard]] bool FUN_00409480(const FUN_00409480_Object* self) noexcept;
[[nodiscard]] bool FUN_004094A0(const FUN_00409480_Object* self) noexcept;
[[nodiscard]] bool FUN_004094C0(const FUN_00409480_Object* self) noexcept;
[[nodiscard]] bool FUN_004094E0(const FUN_00409480_Object* self) noexcept;
[[nodiscard]] bool FUN_00409500(const FUN_00409480_Object* self) noexcept;
[[nodiscard]] std::uint32_t FUN_00409520(const FUN_00409480_Object* self) noexcept;
[[nodiscard]] bool FUN_00409530(const FUN_00409480_Object* self) noexcept;
[[nodiscard]] bool FUN_00409550(const FUN_00409480_Object* self) noexcept;
[[nodiscard]] float* FUN_004095D0(FUN_00409480_Object* self) noexcept;
[[nodiscard]] std::uint8_t FUN_004095E0(const FUN_00409480_Object* self) noexcept;

} // namespace re5::recovered
