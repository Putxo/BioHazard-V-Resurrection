#include "re5/recovered/fun_00406ca0.hpp"

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>

namespace {
struct Call {
    std::int32_t message_index = -1;
    const char* archive_name = nullptr;
    std::uintptr_t direct_table = 0U;
    std::int32_t direct_count = 0;
    std::uintptr_t resolved_table = 0U;
    std::int32_t resolved_count = 0;
};

struct State {
    std::array<Call, 11> calls{};
    std::size_t count = 0U;
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
    assert(state.count < state.calls.size());
    state.calls[state.count++] = Call{
        message_index,
        archive_name,
        direct_table,
        direct_count,
        resolved_table,
        resolved_count,
    };
}
} // namespace

void test_fun_00406ca0() {
    using namespace re5::recovered;

    // Portable-host guard only. The native 41-byte loop has no service-null branch.
    FUN_00406CA0_SetServices(nullptr);
    FUN_00406CA0();

    // Missing host callback is the second harness-only early exit.
    State no_dispatch_state{};
    FUN_00406CA0_Services no_dispatch_services{&no_dispatch_state, nullptr};
    FUN_00406CA0_SetServices(&no_dispatch_services);
    FUN_00406CA0();
    assert(no_dispatch_state.count == 0U);

    State state{};
    FUN_00406CA0_Services services{&state, dispatch_resource_group};
    FUN_00406CA0_SetServices(&services);
    FUN_00406CA0();

    assert(state.count == 11U);
    for (std::size_t i = 0; i < state.count; ++i) {
        const auto& call = state.calls[i];
        assert(call.message_index == static_cast<std::int32_t>(i));
        assert(call.archive_name != nullptr);
        assert(std::strcmp(call.archive_name, "Image\\Archive\\Game2Resource") == 0);
        assert(call.direct_table == 0x015676D0U);
        assert(call.direct_count == 2);
        assert(call.resolved_table == 0x015676E0U);
        assert(call.resolved_count == 5);
    }

    FUN_00406CA0_SetServices(nullptr);
}
