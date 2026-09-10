#include "re5/recovered/fun_00401b50.hpp"
#include "re5/recovered/fun_00401d10.hpp"

#include <cassert>
#include <cstdint>

namespace {
struct Probe {
    const re5::recovered::FUN_00401B50_AwardRecord* records = nullptr;
    std::uint32_t requested_count = 0;
    std::uint32_t unused = 9;
    int refresh_force = 0;
    int store_calls = 0;
};

std::uint32_t enumerate_awards(
    void* context,
    const re5::recovered::FUN_00401B50_AwardRecord** out_records,
    std::uint32_t requested_count,
    std::uint32_t unused) noexcept {
    auto& probe = *static_cast<Probe*>(context);
    probe.requested_count = requested_count;
    probe.unused = unused;
    *out_records = probe.records;
    return 2;
}

void refresh_user_stats(void* context, int force) noexcept {
    static_cast<Probe*>(context)->refresh_force = force;
}

bool store_award(void* context, std::uint32_t) noexcept {
    ++static_cast<Probe*>(context)->store_calls;
    return true;
}
} // namespace

void test_fun_00401d10() {
    using namespace re5::recovered;

    const FUN_00401B50_AwardRecord records[]{
        {10, "one", 0x00010000U},
        {11, "two", 0x00020000U},
    };
    Probe probe{};
    probe.records = records;

    const FUN_00401B50_Services summary_services{
        &probe,
        nullptr,
        nullptr,
        nullptr,
        &store_award,
    };
    const FUN_00401D10_Services services{
        &probe,
        &enumerate_awards,
        &refresh_user_stats,
    };

    FUN_00401B50_SetServices(&summary_services);
    FUN_00401D10_SetServices(&services);
    FUN_00401B50_State state{};
    const std::uint32_t enumerated = FUN_00401D10(state);
    FUN_00401D10_SetServices(nullptr);
    FUN_00401B50_SetServices(nullptr);

    assert(enumerated == 2);
    assert(probe.requested_count == 0x46U);
    assert(probe.unused == 0U);
    assert(probe.refresh_force == 1);
    assert(state.award_records == records);
    assert(state.awarded_count == 2);
    assert(state.saved_count == 2);
    assert(probe.store_calls == 2);
}
