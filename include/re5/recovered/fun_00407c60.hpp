#pragma once

#include "re5/recovered/fun_004022f0.hpp"

#include <cstddef>

namespace re5::recovered {

struct FUN_00407C60_Services {
    void* context;
    void* (*allocate_aligned)(
        void* context,
        std::size_t size,
        std::size_t alignment) noexcept;
    void (*free_aligned)(
        void* context,
        void* pointer) noexcept;
};

void FUN_00407C60_SetServices(
    const FUN_00407C60_Services* services) noexcept;

FUN_00402360_String* FUN_00407C60(
    FUN_00402360_String& destination,
    const char* format,
    ...) noexcept;

} // namespace re5::recovered
