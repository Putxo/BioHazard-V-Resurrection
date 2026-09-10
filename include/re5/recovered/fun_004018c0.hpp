#pragma once

namespace re5::recovered {

struct FUN_004018C0_Services {
    void* context;
    void* (*get_object)(void* context) noexcept;
    bool (*query_flag)(void* context, void* object) noexcept;
    void (*set_cached_flag)(void* context, bool value) noexcept;
};

void FUN_004018C0_SetServices(const FUN_004018C0_Services* services) noexcept;
bool FUN_004018C0() noexcept;

} // namespace re5::recovered
