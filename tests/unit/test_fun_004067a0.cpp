#include "re5/recovered/fun_004067a0.hpp"

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

void test_fun_004067a0() {
    using namespace re5::recovered;

    FUN_004067A0_SetServices(nullptr);
    FUN_004067A0();

    State no_dispatch_state{};
    FUN_004067A0_Services no_dispatch_services{&no_dispatch_state, nullptr};
    FUN_004067A0_SetServices(&no_dispatch_services);
    FUN_004067A0();
    assert(no_dispatch_state.dispatch_calls == 0U);

    State state{};
    FUN_004067A0_Services services{&state, dispatch_resource_group};
    FUN_004067A0_SetServices(&services);
    FUN_004067A0();

    assert(state.dispatch_calls == 1U);
    assert(state.message_index == -1);
    assert(state.archive_name != nullptr);
    assert(std::strcmp(state.archive_name, "Image\\Archive\\RecordResource") == 0);
    assert(state.direct_table == 0x01567B98U);
    assert(state.direct_count == 1);
    assert(state.resolved_table == 0U);
    assert(state.resolved_count == 0);

    FUN_004067A0_SetServices(nullptr);
}
