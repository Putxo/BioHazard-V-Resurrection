#pragma once

#include <cstdint>

namespace re5::recovered {

struct FUN_00407AF0_Object {
    std::uintptr_t vtable;
    std::uintptr_t message;
    std::uint32_t owns_message;
};

struct FUN_00407AF0_Services {
    void* context;
    void (*copy_exception_base)(
        void* context,
        FUN_00407AF0_Object* destination,
        const FUN_00407AF0_Object* source) noexcept;
};

void FUN_00407AF0_SetServices(const FUN_00407AF0_Services* services) noexcept;

FUN_00407AF0_Object* FUN_00407AF0(
    FUN_00407AF0_Object& destination,
    const FUN_00407AF0_Object& source) noexcept;

} // namespace re5::recovered
