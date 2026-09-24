#pragma once

#include <cstddef>
#include <cstdint>

namespace re5::recovered {

struct FUN_00407B70_Object {
    std::uint8_t reserved_0000_102B[0x102C];
    std::uint32_t string_object;
};

static_assert(offsetof(FUN_00407B70_Object, string_object) == 0x102C);
static_assert(sizeof(FUN_00407B70_Object) == 0x1030);

[[nodiscard]] std::uint32_t FUN_00407B70(
    const FUN_00407B70_Object& object) noexcept;

} // namespace re5::recovered
