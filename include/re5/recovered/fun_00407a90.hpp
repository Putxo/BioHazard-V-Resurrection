#pragma once

#include <cstdint>

namespace re5::recovered {

struct FUN_00407A90_Services {
    void* context;
    void* (*allocate_aligned)(
        void* context,
        std::uint32_t requested_size,
        std::uint32_t alignment);
};

void FUN_00407A90_SetServices(const FUN_00407A90_Services* services) noexcept;

void* FUN_00407A90(
    std::uint32_t requested_size,
    const void* unused_type_tag);

} // namespace re5::recovered
