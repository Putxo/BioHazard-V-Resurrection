#pragma once

#include "re5/recovered/fun_00401d50.hpp"

namespace re5::recovered {

struct FUN_00401E30_Services {
    void* context;
    FUN_00401D50_Object* (*get_global_instance)(void* context) noexcept;
    FUN_00401D50_Object* (*allocate_instance)(void* context, unsigned size, unsigned alignment) noexcept;
};

void FUN_00401E30_SetServices(const FUN_00401E30_Services* services) noexcept;
FUN_00401D50_Object* FUN_00401E30() noexcept;

} // namespace re5::recovered
