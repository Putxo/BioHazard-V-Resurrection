#pragma once

#include <cstddef>
#include <cstdint>

namespace re5::recovered {

struct FUN_00407B50_Object {
    std::uint8_t reserved_0000_1027[0x1028];
    std::uint32_t string_object;
};

static_assert(offsetof(FUN_00407B50_Object, string_object) == 0x1028);
static_assert(sizeof(FUN_00407B50_Object) == 0x102C);

[[nodiscard]] std::uint32_t FUN_00407B50(
    const FUN_00407B50_Object& object) noexcept;

} // namespace re5::recovered
