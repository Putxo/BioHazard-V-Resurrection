#pragma once

#include <cstdint>

namespace re5::recovered {

struct FUN_00405BD0_Services {
    void* context;
    const char* global_root;
    bool archive_special_enabled;
    bool registry_suppressed_a;
    bool registry_suppressed_b;
    std::uint32_t (*exchange_message_index)(void* context, std::uint32_t value) noexcept;
    bool (*exchange_global_loading)(void* context, bool enabled) noexcept;
    const char* (*resolve_path)(void* context, const char* path) noexcept;
    void (*prepare_archive)(void* context, const char* archive_path) noexcept;
    bool (*register_named)(void* context, const char* path, const char* key, bool enabled) noexcept;
    void (*finalize_registered)(void* context, bool* registered_value) noexcept;
    bool (*check_cancel)(void* context, const char* archive_path) noexcept;
    void (*begin_batch)(void* context, const char* base_path) noexcept;
    void (*start_batch)(void* context) noexcept;
    void (*end_batch)(void* context, bool value) noexcept;
    void (*bind_registered)(void* context, bool* registered_value) noexcept;
    void (*log_marker)(void* context, const char* marker) noexcept;
};

void FUN_00405BD0_SetServices(const FUN_00405BD0_Services* services) noexcept;
void FUN_00405BD0(std::uint32_t message_index) noexcept;

} // namespace re5::recovered
