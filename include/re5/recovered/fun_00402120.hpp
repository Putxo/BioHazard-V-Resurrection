#pragma once

#include <cstddef>

namespace re5::recovered {

struct FUN_00402120_Services {
    void* allocator_context;
    void* (*allocate_aligned)(void* allocator_context, std::size_t size, std::size_t alignment) noexcept;
};

void FUN_00402120_SetServices(const FUN_00402120_Services* services) noexcept;
void* FUN_00402120(std::size_t size) noexcept;

} // namespace re5::recovered
