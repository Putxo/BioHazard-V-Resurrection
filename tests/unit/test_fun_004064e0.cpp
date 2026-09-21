#include "re5/recovered/fun_004064e0.hpp"

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

void test_fun_004064e0() {
    using namespace re5::recovered;

    // Portable host guard: the original 33-byte wrapper itself has no branch.
    FUN_004064E0_SetServices(nullptr);
    FUN_004064E0(3U);

    State no_dispatch_state{};
    FUN_004064E0_Services no_dispatch_services{&no_dispatch_state, nullptr};
    FUN_004064E0_SetServices(&no_dispatch_services);
    FUN_004064E0(4U);
    assert(no_dispatch_state.calls == 0U);

    State state{};
    FUN_004064E0_Services services{&state, dispatch_resource_group};
    FUN_004064E0_SetServices(&services);
    FUN_004064E0(9U);

    assert(state.calls == 1U);
    assert(state.selector == 9U);
    assert(state.archive_name != nullptr);
    assert(std::strcmp(state.archive_name, "Image\\Archive\\GameResource") == 0);
    assert(state.primary_table == 0x01567590U);
    assert(state.primary_count == 15U);
    assert(state.secondary_table == 0x01567608U);
    assert(state.secondary_count == 7U);

    FUN_004064E0_SetServices(nullptr);
}
