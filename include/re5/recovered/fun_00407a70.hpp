#pragma once

#include <cstddef>
#include <cstdint>

namespace re5::recovered {

struct FUN_00407A70_Object {
    std::uint32_t reserved_00;
    std::uint32_t begin;
    std::uint32_t end;
};

static_assert(offsetof(FUN_00407A70_Object, begin) == 0x04);
static_assert(offsetof(FUN_00407A70_Object, end) == 0x08);

std::int32_t FUN_00407A70(const FUN_00407A70_Object& object) noexcept;

} // namespace re5::recovered
