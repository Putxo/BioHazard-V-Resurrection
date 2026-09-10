#include "re5/recovered/fun_00401730.hpp"

#include <array>
#include <cassert>
#include <cstdint>

namespace {
struct State {
    int seed_reads = 0;
    std::array<std::uint8_t, 8> seed{{1,2,3,4,5,6,7,8}};
};
void get_seed(void* ctx, std::uint8_t* out) noexcept {
    auto* state = static_cast<State*>(ctx);
    ++state->seed_reads;
    for (std::size_t i = 0; i < state->seed.size(); ++i) out[i] = state->seed[i];
}
}

void test_fun_00401730() {
    using namespace re5::recovered;
    State state{};
    FUN_00401730_Services services{&state, get_seed};
    FUN_00401730_SetServices(&services);

    std::array<std::uint8_t, 10> source{};
    source.fill(0x10U);
    std::array<std::uint8_t, 10> dest{};
    assert(FUN_00401730(dest.data(), 10U, source.data(), 10U) == 10U);
    const std::array<std::uint8_t, 10> expected{{0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x01,0x02}};
    assert(dest == expected);
    assert(state.seed_reads == 1);

    state.seed_reads = 0;
    dest.fill(0);
    assert(FUN_00401730(dest.data(), 9U, source.data(), 10U) == 0U);
    assert(state.seed_reads == 0);

    state.seed_reads = 0;
    assert(FUN_00401730(dest.data(), 0U, source.data(), 0U) == 0U);
    assert(state.seed_reads == 1);

    FUN_00401730_SetServices(nullptr);
    assert(FUN_00401730(dest.data(), 10U, source.data(), 10U) == 0U);
}
