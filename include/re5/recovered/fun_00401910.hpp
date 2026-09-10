#pragma once

namespace re5::recovered {

struct FUN_00401910_Services {
    void* context;
    void* monitored_object;
    bool* cached_state;
    const bool* positive_return_state;
    bool (*probe_object)(void* context, void* object) noexcept;
};

void FUN_00401910_SetServices(const FUN_00401910_Services* services) noexcept;
bool FUN_00401910() noexcept;

} // namespace re5::recovered
