#pragma once

#include <cstddef>

namespace re5::recovered {

struct FUN_00404F90_TableEntry {
    const char* name;
    const char* path;
};

struct FUN_00404F90_Services {
    void* context;
    const FUN_00404F90_TableEntry* table;
    std::size_t table_count;
    void (*log_marker)(void* context, const char* marker) noexcept;
    void (*register_entry)(void* context, const FUN_00404F90_TableEntry& entry, bool enabled) noexcept;
    void (*register_group)(void* context, const char* first, const char* second, const char* third) noexcept;
};

void FUN_00404F90_SetServices(const FUN_00404F90_Services* services) noexcept;
void FUN_00404F90() noexcept;

} // namespace re5::recovered
