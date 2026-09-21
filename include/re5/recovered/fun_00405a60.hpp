#pragma once

#include <cstddef>
#include <cstdint>

namespace re5::recovered {

struct FUN_00405A60_Services {
    void* context;
    const char* global_root;
    bool global_special_enabled;
    bool registry_suppressed_a;
    bool registry_suppressed_b;
    std::uint8_t (*get_global_loading)(void* context) noexcept;
    void (*set_global_loading)(void* context, std::uint8_t value) noexcept;
    void (*trace_marker)(void* context, const char* marker) noexcept;
    void (*format_path)(
        void* context,
        const char* format_marker,
        const char* first,
        const char* second,
        char* out,
        std::size_t out_size) noexcept;
    void (*prepare_secondary)(void* context, const char* path) noexcept;
    void* (*register_named)(
        void* context,
        const char* path,
        std::uintptr_t key_token,
        bool enabled) noexcept;
    void (*finalize_registered)(void* context, void* registered) noexcept;
    bool (*check_cancel)(void* context, const char* path) noexcept;
    void (*begin_batch)(void* context, const char* primary_path) noexcept;
    void (*end_batch)(void* context, bool enabled) noexcept;
    void (*bind_registered)(void* context, void** registered_slot) noexcept;
};

void FUN_00405A60_SetServices(const FUN_00405A60_Services* services) noexcept;
void FUN_00405A60() noexcept;

} // namespace re5::recovered
