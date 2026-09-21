#pragma once

#include <cstddef>
#include <cstdint>

namespace re5::recovered {

struct FUN_00405DF0_Services {
    void* context;
    const char* global_root;
    bool global_special_enabled;
    bool registry_suppressed_a;
    bool registry_suppressed_b;
    bool* global_loading;
    std::uint32_t (*get_active_selector)(void* context) noexcept;
    void (*set_active_selector)(void* context, std::uint32_t selector) noexcept;
    const char* (*resolve_path)(void* context, const char* path) noexcept;
    void (*format_path)(
        void* context,
        const char* format_marker,
        const char* first,
        const char* second,
        char* out,
        std::size_t out_size) noexcept;
    std::uintptr_t (*register_resource)(
        void* context,
        std::uintptr_t destination,
        const char* path,
        bool enabled) noexcept;
    void (*finalize_registered)(void* context, std::uintptr_t registered_value) noexcept;
    void (*prepare_archive)(void* context, const char* archive_path) noexcept;
    bool (*check_cancel)(void* context, const char* archive_path) noexcept;
    void (*begin_batch)(void* context, const char* primary_path) noexcept;
    void (*end_batch)(void* context) noexcept;
    void (*bind_registered)(void* context, std::uintptr_t* registered_value) noexcept;
    void (*finish_trace)(void* context) noexcept;
};

void FUN_00405DF0_SetServices(const FUN_00405DF0_Services* services) noexcept;
void FUN_00405DF0(std::uint32_t selector) noexcept;

} // namespace re5::recovered
