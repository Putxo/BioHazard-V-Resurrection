#pragma once

#include <cstdint>

namespace re5::recovered {

struct FUN_00408640_Object {
    std::uint32_t vtable;
    std::uint32_t state;
};

[[nodiscard]] FUN_00408640_Object* FUN_00408640(
    FUN_00408640_Object* object) noexcept;

} // namespace re5::recovered
