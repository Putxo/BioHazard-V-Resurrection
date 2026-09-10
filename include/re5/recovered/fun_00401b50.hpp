#pragma once

#include <cstdint>

namespace re5::recovered {

struct FUN_00401B50_AwardRecord {
    std::uint32_t achievement_id;
    const char* label;
    std::uint32_t flags;
};

struct FUN_00401B50_State {
    const FUN_00401B50_AwardRecord* award_records;
    std::uint32_t awarded_count;
    std::uint32_t saved_count;
};

struct FUN_00401B50_Services {
    void* context;
    void (*log_header)(void* context, const char* user_name) noexcept;
    void (*log_award)(void* context, std::uint32_t achievement_id, const char* state, const char* label) noexcept;
    void (*log_summary)(void* context, std::uint32_t saved_count, std::uint32_t awarded_count) noexcept;
    bool (*store_award)(void* context, std::uint32_t achievement_id) noexcept;
};

void FUN_00401B50_SetServices(const FUN_00401B50_Services* services) noexcept;
std::uint32_t FUN_00401B50(
    FUN_00401B50_State& state,
    const FUN_00401B50_AwardRecord* records,
    std::uint32_t record_count) noexcept;

} // namespace re5::recovered
