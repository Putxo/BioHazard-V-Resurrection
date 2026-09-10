#pragma once

#include <cstdint>

namespace re5::recovered {

struct FUN_00401880_Services {
    void* context;
    void* (*get_friends)(void* context) noexcept;
    const char* (*invoke_slot_00)(void* context, void* friends) noexcept;
};

void FUN_00401880_SetServices(const FUN_00401880_Services* services) noexcept;
char* FUN_00401880(char* destination, std::uint32_t capacity) noexcept;

} // namespace re5::recovered
