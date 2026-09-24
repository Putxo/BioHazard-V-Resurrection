#pragma once

#include "re5/recovered/fun_004022f0.hpp"

namespace re5::recovered {

using FUN_00407CD0_Object = FUN_00402360_String;

struct FUN_00407CD0_Services {
    void* context;
    void (*free_aligned)(void* context, void* pointer) noexcept;
};

void FUN_00407CD0_SetServices(
    const FUN_00407CD0_Services* services) noexcept;

void FUN_00407CD0(FUN_00407CD0_Object& object) noexcept;

} // namespace re5::recovered
