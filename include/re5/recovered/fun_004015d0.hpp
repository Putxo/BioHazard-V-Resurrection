#pragma once

namespace re5::recovered {

struct FUN_004015D0_Services {
    void* context;
    const char* (*get_ip_country)(void* context) noexcept;
};

void FUN_004015D0_SetServices(const FUN_004015D0_Services* services) noexcept;
[[nodiscard]] int FUN_004015D0() noexcept;

} // namespace re5::recovered
