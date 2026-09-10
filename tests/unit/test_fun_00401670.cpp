#include "re5/recovered/fun_00401670.hpp"

#include <cassert>
#include <cstdint>
#include <cstring>

namespace {
struct State {
    int touch_calls = 0;
    int stats_calls = 0;
    int name_calls = 0;
    int clear_calls = 0;
    int store_calls = 0;
    std::uint32_t last_index = 0;
    const char* last_clear_name = nullptr;
    bool clear_result = true;
    bool store_result = true;
    bool null_stats = false;
};

void touch(void* ctx) noexcept { ++static_cast<State*>(ctx)->touch_calls; }
void* get_stats(void* ctx) noexcept {
    auto* state = static_cast<State*>(ctx);
    ++state->stats_calls;
    return state->null_stats ? nullptr : state;
}
const char* get_name(void* stats, std::uint32_t index) noexcept {
    auto* state = static_cast<State*>(stats);
    ++state->name_calls;
    state->last_index = index;
    return "ignored-by-target";
}
bool clear(void* stats, const char* api_name) noexcept {
    auto* state = static_cast<State*>(stats);
    ++state->clear_calls;
    state->last_clear_name = api_name;
    return state->clear_result;
}
bool store(void* stats) noexcept {
    auto* state = static_cast<State*>(stats);
    ++state->store_calls;
    return state->store_result;
}
}

void test_fun_00401670() {
    using namespace re5::recovered;

    assert(std::strcmp(FUN_00401670_AchievementApiName(0), "Nothing. GFWL starts counting at 1") == 0);
    assert(std::strcmp(FUN_00401670_AchievementApiName(1), "ACH_CHAPTER11_CLEAR") == 0);
    assert(std::strcmp(FUN_00401670_AchievementApiName(50), "ACH_TRUE_TRUST") == 0);
    assert(std::strcmp(FUN_00401670_AchievementApiName(70), "ACH_HEAD_HUNTING") == 0);
    assert(FUN_00401670_AchievementApiName(71) == nullptr);

    State state{};
    FUN_00401670_Services services{&state, touch, get_stats, get_name, clear, store};
    FUN_00401670_SetServices(&services);
    assert(FUN_00401670(1));
    assert(state.touch_calls == 1 && state.stats_calls == 1 && state.name_calls == 1);
    assert(state.clear_calls == 1 && state.store_calls == 1 && state.last_index == 1);
    assert(std::strcmp(state.last_clear_name, "ACH_CHAPTER11_CLEAR") == 0);

    state = {};
    state.null_stats = true;
    assert(!FUN_00401670(1));
    assert(state.touch_calls == 1 && state.stats_calls == 1);
    assert(state.name_calls == 0 && state.clear_calls == 0 && state.store_calls == 0);

    state = {};
    state.clear_result = false;
    assert(!FUN_00401670(70));
    assert(state.name_calls == 1 && state.clear_calls == 1 && state.store_calls == 0);

    state = {};
    state.store_result = false;
    assert(!FUN_00401670(50));
    assert(state.name_calls == 1 && state.clear_calls == 1 && state.store_calls == 1);

    FUN_00401670_SetServices(nullptr);
    assert(!FUN_00401670(1));
}
