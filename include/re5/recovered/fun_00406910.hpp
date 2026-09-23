#pragma once

#include <cstdint>

namespace re5::recovered {

struct FUN_00406910_Services {
    void* context;

    void (*trace_marker)(void* context, const char* marker) noexcept;

    std::uint8_t (*get_convert_mode)(void* context) noexcept;
    void (*set_convert_mode)(void* context, std::uint8_t value) noexcept;

    const char* (*resource_root)(void* context) noexcept;

    bool (*special_mode_enabled)(void* context) noexcept;
    void (*prepare_archive)(void* context, const char* archive_path) noexcept;
    void* (*resolve_resource)(
        void* context,
        std::uintptr_t table,
        const char* name,
        bool create_if_missing) noexcept;
    void (*activate_resource)(void* context, void* resource) noexcept;
    bool (*archive_ready)(void* context, const char* archive_path) noexcept;

    void (*convert_archive)(void* context, const char* archive_name) noexcept;
    void (*flush_archive_conversion)(void* context) noexcept;

    bool (*registry_suppressed_a)(void* context) noexcept;
    bool (*registry_suppressed_b)(void* context) noexcept;
    void (*register_resolved)(void* context, void* resource) noexcept;

    void (*convert_directory)(
        void* context,
        const char* base_path,
        const char* type_key,
        const char* extension) noexcept;

    void (*finish_conversion)(void* context, std::int32_t value) noexcept;
};

void FUN_00406910_SetServices(const FUN_00406910_Services* services) noexcept;
void FUN_00406910(std::int32_t stage_id) noexcept;

} // namespace re5::recovered
