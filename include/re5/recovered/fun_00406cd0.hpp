#pragma once

#include <cstdint>

namespace re5::recovered {

struct FUN_00406CD0_Services {
    void* context;
    void (*log_marker)(void* context, const char* marker) noexcept;
    void (*dispatch_resource_group)(
        void* context,
        std::int32_t message_index,
        const char* archive_name,
        std::uintptr_t direct_table,
        std::uint32_t direct_count,
        std::uintptr_t resolved_table,
        std::uint32_t resolved_count) noexcept;
};

void FUN_00406CD0_SetServices(const FUN_00406CD0_Services* services) noexcept;
void FUN_00406CD0() noexcept;

} // namespace re5::recovered
