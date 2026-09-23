#pragma once

#include <cstdint>

namespace re5::recovered {

struct FUN_004068A0_Services {
    void* context;
    void (*dispatch_resource_group)(
        void* context,
        std::int32_t message_index,
        const char* archive_name,
        std::uintptr_t direct_table,
        std::int32_t direct_count,
        std::uintptr_t resolved_table,
        std::int32_t resolved_count) noexcept;
};

void FUN_004068A0_SetServices(const FUN_004068A0_Services* services) noexcept;
void FUN_004068A0(std::uint32_t result_index) noexcept;

} // namespace re5::recovered
