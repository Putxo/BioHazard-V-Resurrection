#pragma once

#include <cstdint>

namespace re5::recovered {

struct FUN_004061F0_Entry {
    const char* path;
    std::uint32_t key_address;
};

struct FUN_004061F0_Services {
    void* context;
    const char* global_root;
    bool archive_special_enabled;
    bool registry_suppressed_a;
    bool registry_suppressed_b;
    std::int32_t (*read_message_index)(void* context) noexcept;
    void (*write_message_index)(void* context, std::int32_t value) noexcept;
    bool (*exchange_global_loading)(void* context, bool enabled) noexcept;
    const char* (*resolve_indexed_path)(void* context, const char* path) noexcept;
    const char* (*resolve_table_path)(void* context, const char* path) noexcept;
    void (*prepare_archive)(void* context, const char* archive_path) noexcept;
    void* (*register_named)(
        void* context,
        const char* path,
        std::uint32_t key_address,
        bool enabled) noexcept;
    void (*finalize_registered)(void* context, void* registered) noexcept;
    bool (*check_cancel)(void* context, const char* archive_path) noexcept;
    void (*begin_batch)(void* context, const char* base_path) noexcept;
    void (*start_batch)(void* context) noexcept;
    void (*end_batch)(void* context, bool value) noexcept;
    void (*bind_registered)(void* context, void** registered) noexcept;
    void (*log_marker)(void* context, const char* marker) noexcept;
};

void FUN_004061F0_SetServices(const FUN_004061F0_Services* services) noexcept;
void FUN_004061F0(
    std::int32_t message_index,
    const char* archive_resource,
    const FUN_004061F0_Entry* direct_entries,
    std::int32_t direct_count,
    const FUN_004061F0_Entry* resolved_entries,
    std::int32_t resolved_count) noexcept;

} // namespace re5::recovered
