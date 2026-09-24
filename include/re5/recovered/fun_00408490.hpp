#pragma once

#include "re5/recovered/fun_004082e0.hpp"

#include <cstddef>
#include <cstdint>

namespace re5::recovered {

struct FUN_00408490_Object {
    std::uint32_t vtable;
    std::uint32_t base_message;
    std::uint32_t base_owns_message;
    FUN_004082E0_String message;
};

static_assert(offsetof(FUN_00408490_Object, base_message) == 0x04);
static_assert(offsetof(FUN_00408490_Object, base_owns_message) == 0x08);
static_assert(offsetof(FUN_00408490_Object, message) == 0x0C);
static_assert(sizeof(FUN_00408490_Object) == 0x28);

[[nodiscard]] FUN_00408490_Object* FUN_00408490(
    FUN_00408490_Object& object,
    const FUN_004082E0_String& source);

} // namespace re5::recovered
