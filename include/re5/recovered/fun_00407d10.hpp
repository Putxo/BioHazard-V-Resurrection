#pragma once

#include "re5/recovered/fun_004022f0.hpp"

namespace re5::recovered {

struct FUN_00407D10_Services {
    void* context;
    FUN_00402360_Block* (*concatenate)(
        void* context,
        const FUN_00402360_Block* left,
        const FUN_00402360_Block* right) noexcept;
};

void FUN_00407D10_SetServices(
    const FUN_00407D10_Services* services) noexcept;

void FUN_00407D10(
    FUN_00402360_String& destination,
    const char* suffix) noexcept;

} // namespace re5::recovered
