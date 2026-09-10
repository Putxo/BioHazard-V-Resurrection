#pragma once

#include "re5/recovered/fun_00401b50.hpp"

#include <cstdint>

namespace re5::recovered {

struct FUN_00401D10_Services {
    void* context;
    std::uint32_t (*enumerate_awards)(
        void* context,
        FUN_00401B50_AwardRecord const** out_records,
        std::uint32_t requested_count,
        std::uint32_t unused) noexcept;
    void (*refresh_user_stats)(void* context, int force) noexcept;
};

void FUN_00401D10_SetServices(const FUN_00401D10_Services* services) noexcept;
std::uint32_t FUN_00401D10(FUN_00401B50_State& state) noexcept;

} // namespace re5::recovered
