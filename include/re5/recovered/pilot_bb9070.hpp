#pragma once
#include <cstdint>

namespace re5::recovered {

// Provisional two-DWORD view used only for the BB9070 pilot cluster.
// No class name or wider layout is inferred from these accessors.
struct UnknownBB90Object {
    std::uint32_t field_00;
    std::uint32_t field_04;
};

[[nodiscard]] std::uint32_t FUN_00BB9070(const UnknownBB90Object* self) noexcept;
[[nodiscard]] std::uint32_t FUN_00BB9080(const UnknownBB90Object* self) noexcept;
[[nodiscard]] std::uint32_t FUN_00BB9090(const UnknownBB90Object* self) noexcept;

} // namespace re5::recovered
