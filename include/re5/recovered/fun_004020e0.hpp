#pragma once

#include <cstdint>

namespace re5::recovered {

struct FUN_004020E0_Globals {
    std::uintptr_t value_016e1660;
    std::uintptr_t value_016e1690;
};

void FUN_004020E0_SetGlobals(const FUN_004020E0_Globals* globals) noexcept;
std::uintptr_t FUN_004020E0() noexcept;
std::uintptr_t FUN_004020F0() noexcept;
std::uintptr_t FUN_00402100() noexcept;

} // namespace re5::recovered
