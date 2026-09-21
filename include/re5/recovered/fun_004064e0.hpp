#pragma once

#include <cstdint>

namespace re5::recovered {

struct FUN_004064E0_Services {
    void* context;
    void (*dispatch_resource_group)(
        void* context,
        std::uint32_t selector,
        const char* archive_name,
        std::uintptr_t primary_table,
        std::uint32_t primary_count,
        std::uintptr_t secondary_table,
        std::uint32_t secondary_count) noexcept;
};

void FUN_004064E0_SetServices(const FUN_004064E0_Services* services) noexcept;
void FUN_004064E0(std::uint32_t selector) noexcept;

} // namespace re5::recovered
