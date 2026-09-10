#pragma once

#include <cstdint>
#include <cstddef>

namespace re5::recovered {

struct FUN_004014C0_Services {
    void* context;
    bool (*primary_ready)(void* context) noexcept;
    bool (*primary_gate)(void* context, std::uintptr_t token) noexcept;
    void (*primary_log)(
        void* context,
        std::uintptr_t log_context,
        std::uintptr_t message) noexcept;
    void (*set_wait_flag)(void* context, bool enabled) noexcept;
    void (*sleep_ms)(void* context, std::uint32_t milliseconds) noexcept;
    bool (*secondary_gate)(void* context) noexcept;
    void* (*get_secondary_object)(void* context) noexcept;
    bool (*secondary_ready)(void* object) noexcept;
    void (*secondary_log)(
        void* context,
        std::uintptr_t log_context,
        std::uintptr_t message) noexcept;
};

void FUN_004014C0_SetServices(const FUN_004014C0_Services* services) noexcept;
[[nodiscard]] bool FUN_004014C0() noexcept;

} // namespace re5::recovered
