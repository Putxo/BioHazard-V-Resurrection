#include "re5/recovered/fun_00406840.hpp"

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

void test_fun_00406840() {
    using namespace re5::recovered;

    // Portable-host guard only. The native 41-byte body has no conditional branch.
    FUN_00406840_SetServices(nullptr);
    FUN_00406840(0U);

    // Missing host dispatch is another harness-only early exit.
    State no_dispatch_state{};
    FUN_00406840_Services no_dispatch_services{&no_dispatch_state, nullptr};
    FUN_00406840_SetServices(&no_dispatch_services);
    FUN_00406840(0U);
    assert(no_dispatch_state.dispatch_calls == 0U);

    constexpr std::array<const char*, 16> expected_names{{
        "Image\\Archive\\ChapterEnd11",
        "Image\\Archive\\ChapterEnd12",
        "Image\\Archive\\ChapterEnd21",
        "Image\\Archive\\ChapterEnd22",
        "Image\\Archive\\ChapterEnd23",
        "Image\\Archive\\ChapterEnd31",
        "Image\\Archive\\ChapterEnd32",
        "Image\\Archive\\ChapterEnd33",
        "Image\\Archive\\ChapterEnd41",
        "Image\\Archive\\ChapterEnd42",
        "Image\\Archive\\ChapterEnd51",
        "Image\\Archive\\ChapterEnd52",
        "Image\\Archive\\ChapterEnd53",
        "Image\\Archive\\ChapterEnd61",
        "Image\\Archive\\ChapterEnd62",
        "Image\\Archive\\ChapterEnd63",
    }};

    State state{};
    FUN_00406840_Services services{&state, dispatch_resource_group};
    FUN_00406840_SetServices(&services);

    for (std::uint32_t i = 0; i < expected_names.size(); ++i) {
        state = State{};
        FUN_00406840(i);

        assert(state.dispatch_calls == 1U);
        assert(state.message_index == -1);
        assert(state.archive_name != nullptr);
        assert(std::strcmp(state.archive_name, expected_names[i]) == 0);
        assert(state.direct_table == 0x01567C10U + static_cast<std::uintptr_t>(i) * 0x10U);
        assert(state.direct_count == 2);
        assert(state.resolved_table == 0U);
        assert(state.resolved_count == 0);
    }

    FUN_00406840_SetServices(nullptr);
}
