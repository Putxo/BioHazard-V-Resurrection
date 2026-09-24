#pragma once

#include <cstdint>

namespace re5::recovered {

struct FUN_00408610_Object {};

struct FUN_00408610_State {
    std::uint32_t word0;
    std::uint32_t word1;
    std::uint32_t word2;
    std::uint32_t word3;
};

struct FUN_00408610_Services {
    FUN_00408610_State* state;
};

void FUN_00408610_SetServices(
    const FUN_00408610_Services* services) noexcept;

[[nodiscard]] std::uint32_t FUN_00408610(
    FUN_00408610_Object& object) noexcept;

} // namespace re5::recovered
