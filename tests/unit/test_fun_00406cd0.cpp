#include "re5/recovered/fun_00406cd0.hpp"

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>

namespace {
struct DispatchRecord {
    std::int32_t message_index = 0;
    const char* archive_name = nullptr;
    std::uintptr_t direct_table = 0U;
    std::uint32_t direct_count = 0U;
    std::uintptr_t resolved_table = 1U;
    std::uint32_t resolved_count = 1U;
};

struct State {
    unsigned log_calls = 0U;
    unsigned dispatch_calls = 0U;
    const char* marker = nullptr;
    std::array<DispatchRecord, 8> dispatches{};
    char order[10]{};
    unsigned order_count = 0U;
};

void log_marker(void* context, const char* marker) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.log_calls;
    state.marker = marker;
    state.order[state.order_count++] = 'L';
}

void dispatch_resource_group(
    void* context,
    std::int32_t message_index,
    const char* archive_name,
    std::uintptr_t direct_table,
    std::uint32_t direct_count,
    std::uintptr_t resolved_table,
    std::uint32_t resolved_count) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(state.dispatch_calls < state.dispatches.size());
    auto& out = state.dispatches[state.dispatch_calls++];
    out.message_index = message_index;
    out.archive_name = archive_name;
    out.direct_table = direct_table;
    out.direct_count = direct_count;
    out.resolved_table = resolved_table;
    out.resolved_count = resolved_count;
    state.order[state.order_count++] = 'D';
}
} // namespace

void test_fun_00406cd0() {
    using namespace re5::recovered;

    // Host-only safety exit. The native 209-byte routine has no such branch.
    FUN_00406CD0_SetServices(nullptr);
    FUN_00406CD0();

    // Host-only missing-dispatch guard: marker still executes, resource loop is skipped.
    State no_dispatch{};
    FUN_00406CD0_Services no_dispatch_services{&no_dispatch, log_marker, nullptr};
    FUN_00406CD0_SetServices(&no_dispatch_services);
    FUN_00406CD0();
    assert(no_dispatch.log_calls == 1U);
    assert(no_dispatch.dispatch_calls == 0U);
    assert(no_dispatch.marker != nullptr);
    assert(std::strcmp(no_dispatch.marker, ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n") == 0);
    assert(no_dispatch.order_count == 1U && no_dispatch.order[0] == 'L');

    // Host-only missing-marker adapter: native resource sequence remains testable.
    State no_marker{};
    FUN_00406CD0_Services no_marker_services{&no_marker, nullptr, dispatch_resource_group};
    FUN_00406CD0_SetServices(&no_marker_services);
    FUN_00406CD0();
    assert(no_marker.log_calls == 0U);
    assert(no_marker.dispatch_calls == 8U);

    constexpr std::array<const char*, 8> k_names{{
        "Image\\Archive\\TitleResource",
        "Image\\Archive\\MenuResource",
        "Image\\Archive\\ShopResource",
        "Image\\Archive\\VersusResource",
        "Image\\Archive\\MerceResource",
        "Image\\Archive\\RecordResource",
        "Image\\Archive\\ViewerResource",
        "Image\\Archive\\CreditResource",
    }};
    constexpr std::array<std::uintptr_t, 8> k_tables{{
        0x01567708U, 0x01567800U, 0x01567928U, 0x01567958U,
        0x015679A0U, 0x01567B98U, 0x01567BA0U, 0x01567C00U,
    }};
    constexpr std::array<std::uint32_t, 8> k_counts{{24U, 22U, 3U, 9U, 8U, 1U, 5U, 1U}};

    State state{};
    FUN_00406CD0_Services services{&state, log_marker, dispatch_resource_group};
    FUN_00406CD0_SetServices(&services);
    FUN_00406CD0();

    assert(state.log_calls == 1U);
    assert(state.dispatch_calls == 8U);
    assert(state.marker != nullptr);
    assert(std::strcmp(state.marker, ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n") == 0);
    assert(state.order_count == 9U);
    assert(state.order[0] == 'L');

    for (std::size_t i = 0; i < state.dispatches.size(); ++i) {
        const auto& out = state.dispatches[i];
        assert(state.order[i + 1U] == 'D');
        assert(out.message_index == -1);
        assert(out.archive_name != nullptr);
        assert(std::strcmp(out.archive_name, k_names[i]) == 0);
        assert(out.direct_table == k_tables[i]);
        assert(out.direct_count == k_counts[i]);
        assert(out.resolved_table == 0U);
        assert(out.resolved_count == 0U);
    }

    FUN_00406CD0_SetServices(nullptr);
}
