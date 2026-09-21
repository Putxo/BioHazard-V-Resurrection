#pragma once

#include <cstdint>

namespace re5::recovered {

struct FUN_004065D0_Services {
    void* context;
    void (*trace_marker)(
        void* context,
        std::uintptr_t logger_token,
        std::uintptr_t format_token) noexcept;
    void (*dispatch_resource_group)(
        void* context,
        std::int32_t message_index,
        const char* archive_name,
        std::uintptr_t direct_table,
        std::int32_t direct_count,
        std::uintptr_t resolved_table,
        std::int32_t resolved_count) noexcept;
};

void FUN_004065D0_SetServices(const FUN_004065D0_Services* services) noexcept;
void FUN_004065D0() noexcept;

} // namespace re5::recovered
