#pragma once

namespace re5::recovered {

struct FUN_00401940_Services {
    bool* positive_return_state;
};

void FUN_00401940_SetServices(const FUN_00401940_Services* services) noexcept;
void FUN_00401940(bool value) noexcept;

} // namespace re5::recovered
