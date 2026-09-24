#pragma once

#include "re5/recovered/fun_00408490.hpp"

#include <cstdint>

namespace re5::recovered {

using FUN_00408550_Object = FUN_00408490_Object;

inline constexpr std::uint32_t
    FUN_00408550_EXCEPTION_VTABLE = 0x01541FACU;
inline constexpr std::uint32_t
    FUN_00408550_LOGIC_ERROR_VTABLE = 0x01541F54U;

struct FUN_00408550_Services {
    void* context;
    std::uint32_t (*duplicate_base_message)(
        void* context,
        const FUN_00408550_Object& destination,
        std::uint32_t source_message) noexcept;
};

void FUN_00408550_SetServices(
    const FUN_00408550_Services* services) noexcept;

[[nodiscard]] FUN_00408550_Object* FUN_00408550(
    FUN_00408550_Object& object,
    const FUN_00408550_Object& source) noexcept;

} // namespace re5::recovered
