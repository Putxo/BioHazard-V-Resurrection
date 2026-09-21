#pragma once

#include <cstddef>
#include <cstdint>

namespace re5::recovered {

struct FUN_00405710_Request {
    const char* name;
    std::uint8_t flags_1f = 0;
};

struct FUN_00405710_Entry {
    const char* name;
    const char* path;
};

struct FUN_00405710_Child {
    const FUN_00405710_Entry* entries = nullptr;
    std::size_t entry_count = 0;
};

struct FUN_00405710_Services {
    void* context;
    const char* global_root;
    bool global_special_enabled;
    bool registry_suppressed_a;
    bool registry_suppressed_b;
    void (*format_path)(void* context, const char* format_marker, const char* value, char* out, std::size_t out_size) noexcept;
    bool (*register_named)(void* context, const char* path, const char* key, bool enabled) noexcept;
    bool (*check_cancel)(void* context, const char* path) noexcept;
    FUN_00405710_Child* (*open_child)(void* context, FUN_00405710_Request& request) noexcept;
    bool (*child_ready)(void* context, FUN_00405710_Child& child) noexcept;
    void (*release_child)(void* context, FUN_00405710_Child& child, bool immediate) noexcept;
    void (*set_global_loading)(void* context, bool enabled) noexcept;
    void (*begin_batch)(void* context, char* out, std::size_t out_size) noexcept;
    void (*end_batch)(void* context) noexcept;
    void (*bind_registered)(void* context, bool* registered_value) noexcept;
};

void FUN_00405710_SetServices(const FUN_00405710_Services* services) noexcept;
void FUN_00405710(FUN_00405710_Request& request) noexcept;

} // namespace re5::recovered
