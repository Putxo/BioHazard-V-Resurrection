#include "re5/recovered/fun_00406870.hpp"

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

void test_fun_00406870() {
    using namespace re5::recovered;

    // Portable-host guard only. The original 41-byte wrapper has no branch.
    FUN_00406870_SetServices(nullptr);
    FUN_00406870(0U);

    // Missing host dispatch is another harness-only early exit.
    State no_dispatch_state{};
    FUN_00406870_Services no_dispatch_services{&no_dispatch_state, nullptr};
    FUN_00406870_SetServices(&no_dispatch_services);
    FUN_00406870(7U);
    assert(no_dispatch_state.dispatch_calls == 0U);

    static constexpr const char* k_expected_names[] = {
        "Image\\Archive\\VersusResult01",
        "Image\\Archive\\VersusResult02",
        "Image\\Archive\\VersusResult03",
        "Image\\Archive\\VersusResult04",
        "Image\\Archive\\VersusResult05",
        "Image\\Archive\\VersusResult06",
        "Image\\Archive\\VersusResult07",
        "Image\\Archive\\VersusResult08",
    };

    State state{};
    FUN_00406870_Services services{&state, dispatch_resource_group};
    FUN_00406870_SetServices(&services);

    for (std::uint32_t variant = 0U; variant < 8U; ++variant) {
        state = State{};
        FUN_00406870(variant);

        assert(state.dispatch_calls == 1U);
        assert(state.message_index == -1);
        assert(state.archive_name != nullptr);
        assert(std::strcmp(state.archive_name, k_expected_names[variant]) == 0);
        assert(state.direct_table == 0x01567D50U + static_cast<std::uintptr_t>(variant) * 0x10U);
        assert(state.direct_count == 2);
        assert(state.resolved_table == 0U);
        assert(state.resolved_count == 0);
    }

    FUN_00406870_SetServices(nullptr);
}
