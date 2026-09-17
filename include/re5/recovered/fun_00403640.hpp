#pragma once

#include <cstdint>

namespace re5::recovered {

struct FUN_00403640_Entry {
    const char* name;
    bool descend;
};

struct FUN_00403640_Services {
    const char* active_name;
    const char* fallback_name;
    const char* root_suffix;
    std::uintptr_t root_table_01652ef0;
    std::uintptr_t registry_root_016e20dc;
    void* context;
    bool (*validate_handle)(void* context, const void* handle) noexcept;
    void* (*open_root)(void* context, const char* key, std::uintptr_t table) noexcept;
    void (*close_root)(void* context, void* root) noexcept;
    bool (*is_registry_ready)(void* context, std::uintptr_t registry) noexcept;
    void (*ensure_registry)(void* context, std::uintptr_t registry) noexcept;
    bool (*next_entry)(void* context, void* root, FUN_00403640_Entry& entry) noexcept;
    bool (*known_leaf)(void* context, const char* leaf) noexcept;
    const void* (*lookup_leaf)(void* context, const char* leaf) noexcept;
    void (*bind_leaf)(void* context, const void* value, const char* path) noexcept;
};

void FUN_00403640_SetServices(const FUN_00403640_Services* services) noexcept;
void FUN_00403640(void* object, const void* handle, const char* base_path, const char* key) noexcept;

} // namespace re5::recovered
