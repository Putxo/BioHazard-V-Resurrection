#pragma once

#include "re5/recovered/fun_00408490.hpp"

#include <cstdint>

namespace re5::recovered {

inline constexpr std::uint32_t
    FUN_004084D0_LOGIC_ERROR_VTABLE = 0x01541F54U;
inline constexpr std::uint32_t
    FUN_004084D0_EXCEPTION_VTABLE = 0x01541FACU;

using FUN_004084D0_Object = FUN_00408490_Object;

struct FUN_004084D0_Services {
    void* context;
    void (*free_aligned)(
        void* context,
        void* pointer) noexcept;
    void (*release_base_message)(
        void* context,
        std::uint32_t message) noexcept;
};

void FUN_004084D0_SetServices(
    const FUN_004084D0_Services* services) noexcept;

void FUN_004084D0(
    FUN_004084D0_Object& object) noexcept;

} // namespace re5::recovered
