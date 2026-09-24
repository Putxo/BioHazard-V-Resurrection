#pragma once

namespace re5::recovered {

struct FUN_00408630_Services {
    void* global_value;
};

void FUN_00408630_SetServices(
    const FUN_00408630_Services* services) noexcept;

[[nodiscard]] void* FUN_00408630() noexcept;

} // namespace re5::recovered
