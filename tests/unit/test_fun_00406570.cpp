#include "re5/recovered/fun_00406570.hpp"

#include <cassert>
#include <cstdint>
#include <cstring>

namespace {
struct State {
    unsigned log_calls = 0U;
    unsigned dispatch_calls = 0U;
    const char* marker = nullptr;
    std::int32_t message_index = 0;
    const char* archive_name = nullptr;
    std::uintptr_t direct_table = 0U;
    std::uint32_t direct_count = 0U;
    std::uintptr_t resolved_table = 1U;
    std::uint32_t resolved_count = 1U;
    char order[3]{};
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
    ++state.dispatch_calls;
    state.message_index = message_index;
    state.archive_name = archive_name;
    state.direct_table = direct_table;
    state.direct_count = direct_count;
    state.resolved_table = resolved_table;
    state.resolved_count = resolved_count;
    state.order[state.order_count++] = 'D';
}
} // namespace

void test_fun_00406570() {
    using namespace re5::recovered;

    // Portable-host early exit only. The native 42-byte wrapper has no branch.
    FUN_00406570_SetServices(nullptr);
    FUN_00406570();

    State log_only{};
    FUN_00406570_Services log_only_services{&log_only, log_marker, nullptr};
    FUN_00406570_SetServices(&log_only_services);
    FUN_00406570();
    assert(log_only.log_calls == 1U);
    assert(log_only.dispatch_calls == 0U);
    assert(log_only.marker != nullptr);
    assert(std::strcmp(log_only.marker, ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n") == 0);
    assert(log_only.order_count == 1U && log_only.order[0] == 'L');

    State dispatch_only{};
    FUN_00406570_Services dispatch_only_services{&dispatch_only, nullptr, dispatch_resource_group};
    FUN_00406570_SetServices(&dispatch_only_services);
    FUN_00406570();
    assert(dispatch_only.log_calls == 0U);
    assert(dispatch_only.dispatch_calls == 1U);
    assert(dispatch_only.message_index == -1);
    assert(dispatch_only.archive_name != nullptr);
    assert(std::strcmp(dispatch_only.archive_name, "Image\\Archive\\TitleResource") == 0);
    assert(dispatch_only.direct_table == 0x01567708U);
    assert(dispatch_only.direct_count == 24U);
    assert(dispatch_only.resolved_table == 0U);
    assert(dispatch_only.resolved_count == 0U);
    assert(dispatch_only.order_count == 1U && dispatch_only.order[0] == 'D');

    State state{};
    FUN_00406570_Services services{&state, log_marker, dispatch_resource_group};
    FUN_00406570_SetServices(&services);
    FUN_00406570();
    assert(state.log_calls == 1U);
    assert(state.dispatch_calls == 1U);
    assert(state.marker != nullptr);
    assert(std::strcmp(state.marker, ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n") == 0);
    assert(state.message_index == -1);
    assert(state.archive_name != nullptr);
    assert(std::strcmp(state.archive_name, "Image\\Archive\\TitleResource") == 0);
    assert(state.direct_table == 0x01567708U);
    assert(state.direct_count == 24U);
    assert(state.resolved_table == 0U);
    assert(state.resolved_count == 0U);
    assert(state.order_count == 2U);
    assert(state.order[0] == 'L');
    assert(state.order[1] == 'D');

    FUN_00406570_SetServices(nullptr);
}
