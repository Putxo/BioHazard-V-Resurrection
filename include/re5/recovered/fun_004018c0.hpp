#pragma once

namespace re5::recovered {

struct FUN_004018C0_Services {
    void* context;
    void* monitored_object;
    bool* cached_state;
    bool (*probe_object)(void* context, void* object) noexcept;
};

void FUN_004018C0_SetServices(const FUN_004018C0_Services* services) noexcept;
bool FUN_004018C0() noexcept;

} // namespace re5::recovered
