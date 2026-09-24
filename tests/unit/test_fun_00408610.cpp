#include "re5/recovered/fun_00408610.hpp"

#include <cassert>
#include <cstdint>

void test_fun_00408610() {
    using namespace re5::recovered;

    FUN_00408610_Object first_object{};
    FUN_00408610_Object second_object{};

    // Host-only missing-global-state guard. Native code has no branch here
    // because the four fixed state dwords are assumed to exist.
    FUN_00408610_SetServices(nullptr);
    assert(FUN_00408610(first_object) == 0U);

    const FUN_00408610_Services missing_state{
        nullptr,
    };
    FUN_00408610_SetServices(&missing_state);
    assert(FUN_00408610(first_object) == 0U);

    // Exact first step from a small state vector.
    FUN_00408610_State state{
        1U,
        2U,
        3U,
        4U,
    };
    FUN_00408610_Services services{
        &state,
    };
    FUN_00408610_SetServices(&services);

    assert(FUN_00408610(first_object) == 0x00008805U);
    assert(state.word0 == 2U);
    assert(state.word1 == 3U);
    assert(state.word2 == 4U);
    assert(state.word3 == 0x00008805U);

    // A second call proves exact four-word rotation and that the observed
    // this pointer is not part of the native PRNG state.
    assert(FUN_00408610(second_object) == 0x00019807U);
    assert(state.word0 == 3U);
    assert(state.word1 == 4U);
    assert(state.word2 == 0x00008805U);
    assert(state.word3 == 0x00019807U);

    // Unsigned wrap/shift behavior at representative high-bit boundaries.
    FUN_00408610_State edge_state{
        0xFFFFFFFFU,
        0x80000000U,
        0x12345678U,
        0x9ABCDEF0U,
    };
    services.state = &edge_state;

    assert(FUN_00408610(first_object) == 0x9ABCA225U);
    assert(edge_state.word0 == 0x80000000U);
    assert(edge_state.word1 == 0x12345678U);
    assert(edge_state.word2 == 0x9ABCDEF0U);
    assert(edge_state.word3 == 0x9ABCA225U);

    // Native all-zero state has no reseed/special-case branch.
    FUN_00408610_State zero_state{};
    services.state = &zero_state;

    assert(FUN_00408610(first_object) == 0U);
    assert(zero_state.word0 == 0U);
    assert(zero_state.word1 == 0U);
    assert(zero_state.word2 == 0U);
    assert(zero_state.word3 == 0U);

    FUN_00408610_SetServices(nullptr);
}
