#include "re5/recovered/fun_004016c0.hpp"

#include <cassert>
#include <cstdint>
#include <cstring>

namespace {
struct State {
    int log_calls = 0;
    int touch_calls = 0;
    int stats_calls = 0;
    int name_calls = 0;
    int set_calls = 0;
    int store_calls = 0;
    std::uint32_t last_index = 0;
    std::uint32_t last_limit = 0;
    const char* last_set_name = nullptr;
    bool null_stats = false;
    bool set_result = true;
    bool store_result = true;
};

void log_invalid(void* ctx, std::uint32_t index, std::uint32_t limit) noexcept {
    auto* state = static_cast<State*>(ctx);
    ++state->log_calls;
    state->last_index = index;
    state->last_limit = limit;
}
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
bool set_achievement(void* stats, const char* api_name) noexcept {
    auto* state = static_cast<State*>(stats);
    ++state->set_calls;
    state->last_set_name = api_name;
    return state->set_result;
}
bool store(void* stats) noexcept {
    auto* state = static_cast<State*>(stats);
    ++state->store_calls;
    return state->store_result;
}
}

void test_fun_004016c0() {
    using namespace re5::recovered;

    State state{};
    FUN_004016C0_Services services{&state, log_invalid, touch, get_stats, get_name, set_achievement, store};
    FUN_004016C0_SetServices(&services);

    assert(FUN_004016C0(1));
    assert(state.log_calls == 0 && state.touch_calls == 1 && state.stats_calls == 1);
    assert(state.name_calls == 1 && state.set_calls == 1 && state.store_calls == 1);
    assert(state.last_index == 1);
    assert(std::strcmp(state.last_set_name, "ACH_CHAPTER11_CLEAR") == 0);

    state = {};
    assert(!FUN_004016C0(71));
    assert(state.log_calls == 1 && state.last_index == 71 && state.last_limit == 71);
    assert(state.touch_calls == 0 && state.stats_calls == 0 && state.name_calls == 0);

    state = {};
    state.null_stats = true;
    assert(!FUN_004016C0(70));
    assert(state.touch_calls == 1 && state.stats_calls == 1);
    assert(state.name_calls == 0 && state.set_calls == 0 && state.store_calls == 0);

    state = {};
    state.set_result = false;
    assert(!FUN_004016C0(50));
    assert(state.name_calls == 1 && state.set_calls == 1 && state.store_calls == 0);
    assert(std::strcmp(state.last_set_name, "ACH_TRUE_TRUST") == 0);

    state = {};
    state.store_result = false;
    assert(!FUN_004016C0(70));
    assert(state.name_calls == 1 && state.set_calls == 1 && state.store_calls == 1);
    assert(std::strcmp(state.last_set_name, "ACH_HEAD_HUNTING") == 0);

    FUN_004016C0_SetServices(nullptr);
    assert(!FUN_004016C0(1));
}
