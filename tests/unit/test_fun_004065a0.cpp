#include "re5/recovered/fun_004065a0.hpp"

#include <cassert>
#include <cstdint>
#include <cstring>

namespace {
struct State {
    unsigned trace_calls = 0U;
    unsigned dispatch_calls = 0U;
    unsigned sequence = 0U;
    std::uintptr_t logger_token = 0U;
    std::uintptr_t format_token = 0U;
    std::int32_t message_index = 0;
    const char* archive_name = nullptr;
    std::uintptr_t direct_table = 0U;
    std::int32_t direct_count = 0;
    std::uintptr_t resolved_table = 1U;
    std::int32_t resolved_count = 1;
};

void trace_marker(
    void* context,
    std::uintptr_t logger_token,
    std::uintptr_t format_token) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.trace_calls;
    state.logger_token = logger_token;
    state.format_token = format_token;
    state.sequence = 1U;
}

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
    assert(state.sequence == 1U || state.trace_calls == 0U);
    state.sequence = 2U;
    state.message_index = message_index;
    state.archive_name = archive_name;
    state.direct_table = direct_table;
    state.direct_count = direct_count;
    state.resolved_table = resolved_table;
    state.resolved_count = resolved_count;
}
} // namespace

void test_fun_004065a0() {
    using namespace re5::recovered;

    // Portable-host guard only. The original 42-byte wrapper has no branch.
    FUN_004065A0_SetServices(nullptr);
    FUN_004065A0();

    // Missing opaque dispatch is a host-only early exit after preserving the trace call.
    State no_dispatch_state{};
    FUN_004065A0_Services no_dispatch_services{&no_dispatch_state, trace_marker, nullptr};
    FUN_004065A0_SetServices(&no_dispatch_services);
    FUN_004065A0();
    assert(no_dispatch_state.trace_calls == 1U);
    assert(no_dispatch_state.dispatch_calls == 0U);
    assert(no_dispatch_state.logger_token == 0x01652E00U);
    assert(no_dispatch_state.format_token == 0x0137AFECU);

    // A missing host trace hook must not suppress the original resource dispatch semantics.
    State no_trace_state{};
    FUN_004065A0_Services no_trace_services{&no_trace_state, nullptr, dispatch_resource_group};
    FUN_004065A0_SetServices(&no_trace_services);
    FUN_004065A0();
    assert(no_trace_state.trace_calls == 0U);
    assert(no_trace_state.dispatch_calls == 1U);
    assert(no_trace_state.message_index == -1);

    State state{};
    FUN_004065A0_Services services{&state, trace_marker, dispatch_resource_group};
    FUN_004065A0_SetServices(&services);
    FUN_004065A0();

    assert(state.trace_calls == 1U);
    assert(state.dispatch_calls == 1U);
    assert(state.sequence == 2U);
    assert(state.logger_token == 0x01652E00U);
    assert(state.format_token == 0x0137AFECU);
    assert(state.message_index == -1);
    assert(state.archive_name != nullptr);
    assert(std::strcmp(state.archive_name, "Image\\Archive\\Title2Resource") == 0);
    assert(state.direct_table == 0x015677C8U);
    assert(state.direct_count == 2);
    assert(state.resolved_table == 0U);
    assert(state.resolved_count == 0);

    FUN_004065A0_SetServices(nullptr);
}
