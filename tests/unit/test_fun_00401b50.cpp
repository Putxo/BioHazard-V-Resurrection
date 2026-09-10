#include "re5/recovered/fun_00401b50.hpp"

#include <cassert>
#include <cstdint>
#include <vector>

namespace {
struct Probe {
    int header_calls = 0;
    int summary_calls = 0;
    const char* user_name = nullptr;
    std::uint32_t summary_saved = 0;
    std::uint32_t summary_awarded = 0;
    std::vector<std::uint32_t> stored_ids;
    std::vector<const char*> states;
};

void log_header(void* context, const char* user_name) noexcept {
    auto& probe = *static_cast<Probe*>(context);
    ++probe.header_calls;
    probe.user_name = user_name;
}

void log_award(void* context, std::uint32_t achievement_id, const char* state, const char*) noexcept {
    auto& probe = *static_cast<Probe*>(context);
    probe.stored_ids.push_back(achievement_id);
    probe.states.push_back(state);
}

void log_summary(void* context, std::uint32_t saved_count, std::uint32_t awarded_count) noexcept {
    auto& probe = *static_cast<Probe*>(context);
    ++probe.summary_calls;
    probe.summary_saved = saved_count;
    probe.summary_awarded = awarded_count;
}

bool store_award(void*, std::uint32_t achievement_id) noexcept {
    return (achievement_id & 1U) != 0U;
}
} // namespace

void test_fun_00401b50() {
    using namespace re5::recovered;

    Probe probe{};
    const FUN_00401B50_Services services{
        &probe,
        &log_header,
        &log_award,
        &log_summary,
        &store_award,
    };
    FUN_00401B50_SetServices(&services);

    const FUN_00401B50_AwardRecord records[]{
        {1, "chapter 1", 0x00020000U},
        {2, "ignored", 0x00000000U},
        {3, "chapter 3", 0x00010000U},
        {4, "chapter 4", 0x00030000U},
    };

    FUN_00401B50_State state{records, 9, 8};
    const std::uint32_t saved = FUN_00401B50(state, records, 4);
    FUN_00401B50_SetServices(nullptr);

    assert(saved == 2);
    assert(state.award_records == records);
    assert(state.awarded_count == 3);
    assert(state.saved_count == 2);
    assert(probe.header_calls == 1);
    assert(probe.summary_calls == 1);
    assert(probe.summary_saved == 2);
    assert(probe.summary_awarded == 3);
    assert(probe.stored_ids.size() == 3);
    assert(probe.stored_ids[0] == 1);
    assert(probe.stored_ids[1] == 3);
    assert(probe.stored_ids[2] == 4);
    assert(probe.states[0][0] == 'O');
    assert(probe.states[1][0] == 'O');
}
