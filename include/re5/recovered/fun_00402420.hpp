#pragma once

#include "re5/recovered/fun_004022f0.hpp"

#include <cstddef>
#include <cstdint>

namespace re5::recovered {

struct FUN_00402480_Services {
    void* context;
    void* (*allocate_aligned)(void* context, std::size_t size, std::size_t alignment) noexcept;
    std::uintptr_t value_016e201c;
};

using FUN_004024B0_Callback = void (*)(void* element) noexcept;

void FUN_00402480_SetServices(const FUN_00402480_Services* services) noexcept;
const char* FUN_00402420(const FUN_00402360_String& value) noexcept;
FUN_00402360_String* FUN_00402430(FUN_00402360_String& destination, const FUN_00402360_String& source) noexcept;
void* FUN_00402480(std::size_t size) noexcept;
std::uintptr_t FUN_004024A0() noexcept;
void FUN_004024B0(void* base, std::size_t stride, std::int32_t count, FUN_004024B0_Callback callback) noexcept;

} // namespace re5::recovered
