#pragma once

#include <cstdint>

namespace re5::recovered {

struct FUN_004086A0_Object {
    std::uint32_t vtable;
    std::uint32_t state;
    std::uint32_t object_a;
    std::uint32_t object_b;
    std::uint32_t sentinel;
    std::uint32_t preserved;
    float scalar;
};

static_assert(sizeof(FUN_004086A0_Object) == 0x1CU);

[[nodiscard]] FUN_004086A0_Object* FUN_004086A0(
    FUN_004086A0_Object* object) noexcept;

} // namespace re5::recovered
