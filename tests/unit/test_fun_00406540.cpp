#include "re5/recovered/fun_00406540.hpp"

#include <cassert>
#include <cstdint>
#include <cstring>

namespace {
struct State {
    unsigned calls = 0U;
    std::uint32_t selector = 0U;
    const char* archive_name = nullptr;
    std::uintptr_t primary_table = 0U;
    std::uint32_t primary_count = 0U;
    std::uintptr_t secondary_table = 0U;
    std::uint32_t secondary_count = 0U;
};

void dispatch_resource_group(
    void* context,
    std::uint32_t selector,
    const char* archive_name,
    std::uintptr_t primary_table,
    std::uint32_t primary_count,
    std::uintptr_t secondary_table,
    std::uint32_t secondary_count) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.calls;
    state.selector = selector;
    state.archive_name = archive_name;
    state.primary_table = primary_table;
    state.primary_count = primary_count;
    state.secondary_table = secondary_table;
    state.secondary_count = secondary_count;
}
} // namespace

void test_fun_00406540() {
    using namespace re5::recovered;

    // Portable-host early exits only. The original 33-byte wrapper has no branch.
    FUN_00406540_SetServices(nullptr);
    FUN_00406540(3U);

    State no_dispatch_state{};
    FUN_00406540_Services no_dispatch_services{&no_dispatch_state, nullptr};
    FUN_00406540_SetServices(&no_dispatch_services);
    FUN_00406540(5U);
    assert(no_dispatch_state.calls == 0U);

    State state{};
    FUN_00406540_Services services{&state, dispatch_resource_group};
    FUN_00406540_SetServices(&services);
    FUN_00406540(12U);

    assert(state.calls == 1U);
    assert(state.selector == 12U);
    assert(state.archive_name != nullptr);
    assert(std::strcmp(state.archive_name, "Image\\Archive\\Game2Resource") == 0);
    assert(state.primary_table == 0x015676D0U);
    assert(state.primary_count == 2U);
    assert(state.secondary_table == 0x015676E0U);
    assert(state.secondary_count == 5U);

    FUN_00406540_SetServices(nullptr);
}
