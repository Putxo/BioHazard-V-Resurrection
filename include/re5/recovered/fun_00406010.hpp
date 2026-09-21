#pragma once

#include <cstdint>

namespace re5::recovered {

struct FUN_00406010_Entry {
    const char* name;
    bool directory;
};

struct FUN_00406010_Services {
    void* context;
    const char* active_root;
    const char* fallback_name;
    std::uintptr_t root_table_01652ef0;
    bool registry_suppressed_a;
    bool registry_suppressed_b;
    void* (*open_type)(void* context, const char* key, std::uintptr_t table) noexcept;
    void* (*begin_scan)(void* context, const char* wildcard) noexcept;
    bool (*next_entry)(void* context, void* scan, FUN_00406010_Entry& entry) noexcept;
    void (*end_scan)(void* context, void* scan) noexcept;
    void (*report_path_error)(void* context, const char* first, const char* second) noexcept;
    void* (*resolve_leaf)(void* context, void* type_root, const char* path_without_extension, bool create) noexcept;
    void (*register_resolved)(void* context, void* resolved) noexcept;
};

void FUN_00406010_SetServices(const FUN_00406010_Services* services) noexcept;
void FUN_00406010(const char* base_path, const char* key, const char* extension) noexcept;

} // namespace re5::recovered
