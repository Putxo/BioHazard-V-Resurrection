#pragma once

#include <cstddef>
#include <cstdint>

namespace re5::recovered {

struct FUN_00403150_Entry {
    const char* name;
    bool descend;
};

struct FUN_00403150_Services {
    const char* active_name;
    const char* fallback_name;
    const char* header_format;
    const char* error_format;
    const char* child_format;
    const char* leaf_format;
    std::uintptr_t root_table_01652ef0;
    void* context;
    void* (*open_root)(void* context, const char* key, std::uintptr_t table) noexcept;
    void (*close_root)(void* context, void* root) noexcept;
    bool (*begin_scan)(void* context, void* root) noexcept;
    bool (*next_entry)(void* context, void* root, FUN_00403150_Entry& entry) noexcept;
    bool (*end_scan)(void* context, void* root) noexcept;
    void (*report_error)(void* context, const char* message) noexcept;
    void* (*resolve_leaf)(void* context, void* root, const char* leaf, bool create) noexcept;
    void (*activate)(void* context, void* resolved) noexcept;
};

void FUN_00403150_SetServices(const FUN_00403150_Services* services) noexcept;
void FUN_00403150(const char* base_path, const char* key, void* root_context) noexcept;

} // namespace re5::recovered
