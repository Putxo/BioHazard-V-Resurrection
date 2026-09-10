#pragma once

namespace re5::recovered {

struct FUN_00401860_Services {
    bool (*invoke_slot_04)(void* object) noexcept;
};

void FUN_00401860_SetServices(const FUN_00401860_Services* services) noexcept;
bool FUN_00401860(void* object) noexcept;

} // namespace re5::recovered
