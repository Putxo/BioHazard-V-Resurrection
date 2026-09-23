#include "re5/recovered/fun_004068a0.hpp"

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>

namespace {
struct State {
    unsigned dispatch_calls = 0U;
    std::int32_t message_index = 0;
    const char* archive_name = nullptr;
    std::uintptr_t direct_table = 0U;
    std::int32_t direct_count = 0;
    std::uintptr_t resolved_table = 1U;
    std::int32_t resolved_count = 1;
};

void dispatch_resource_group(
    void* context,
    std::int32_t message_index,
    const char* archive_name,
    std::uintptr_t direct_table,
    std::int32_t direct_count,
    std::uintptr_t resolved_table,
    std::int32_t resolved_count) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.dispatch_calls;
    state.message_index = message_index;
    state.archive_name = archive_name;
    state.direct_table = direct_table;
    state.direct_count = direct_count;
    state.resolved_table = resolved_table;
    state.resolved_count = resolved_count;
}
} // namespace

void test_fun_004068a0() {
    using namespace re5::recovered;

    // Host-only safety exit: the original 41-byte wrapper has no such branch.
    FUN_004068A0_SetServices(nullptr);
    FUN_004068A0(0U);

    // Host-only safety exit for an absent injectable dispatcher.
    State no_dispatch_state{};
    FUN_004068A0_Services no_dispatch_services{&no_dispatch_state, nullptr};
    FUN_004068A0_SetServices(&no_dispatch_services);
    FUN_004068A0(0U);
    assert(no_dispatch_state.dispatch_calls == 0U);

    constexpr std::array<const char*, 8> k_expected_names{{
        "Image\\Archive\\MerceResult01",
        "Image\\Archive\\MerceResult02",
        "Image\\Archive\\MerceResult03",
        "Image\\Archive\\MerceResult04",
        "Image\\Archive\\MerceResult05",
        "Image\\Archive\\MerceResult06",
        "Image\\Archive\\MerceResult07",
        "Image\\Archive\\MerceResult08",
    }};

    State state{};
    FUN_004068A0_Services services{&state, dispatch_resource_group};
    FUN_004068A0_SetServices(&services);

    for (std::uint32_t index = 0; index < k_expected_names.size(); ++index) {
        state = {};
        FUN_004068A0(index);

        assert(state.dispatch_calls == 1U);
        assert(state.message_index == -1);
        assert(state.archive_name != nullptr);
        assert(std::strcmp(state.archive_name, k_expected_names[index]) == 0);
        assert(state.direct_table == 0x01567DF0U + static_cast<std::uintptr_t>(index) * 0x10U);
        assert(state.direct_count == 2);
        assert(state.resolved_table == 0U);
        assert(state.resolved_count == 0);
    }

    // No index guard is added: every native direct caller supplies exactly 0..7.
    FUN_004068A0_SetServices(nullptr);
}
