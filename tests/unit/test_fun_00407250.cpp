#include "re5/recovered/fun_00407250.hpp"

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>

namespace {
struct Call {
    std::int32_t message_index = 0;
    const char* archive_name = nullptr;
    std::uintptr_t direct_table = 0U;
    std::int32_t direct_count = 0;
    std::uintptr_t resolved_table = 0U;
    std::int32_t resolved_count = 0;
};

struct State {
    std::array<Call, 44> calls{};
    unsigned count = 0U;
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
    auto& call = state.calls[state.count++];
    call = {message_index, archive_name, direct_table, direct_count, resolved_table, resolved_count};
}

constexpr std::array<const char*, 16> k_chapter_archives{{
    "Image\\Archive\\ChapterEnd11","Image\\Archive\\ChapterEnd12",
    "Image\\Archive\\ChapterEnd21","Image\\Archive\\ChapterEnd22",
    "Image\\Archive\\ChapterEnd23","Image\\Archive\\ChapterEnd31",
    "Image\\Archive\\ChapterEnd32","Image\\Archive\\ChapterEnd33",
    "Image\\Archive\\ChapterEnd41","Image\\Archive\\ChapterEnd42",
    "Image\\Archive\\ChapterEnd51","Image\\Archive\\ChapterEnd52",
    "Image\\Archive\\ChapterEnd53","Image\\Archive\\ChapterEnd61",
    "Image\\Archive\\ChapterEnd62","Image\\Archive\\ChapterEnd63",
}};
constexpr std::array<const char*, 8> k_versus_archives{{
    "Image\\Archive\\VersusResult01","Image\\Archive\\VersusResult02",
    "Image\\Archive\\VersusResult03","Image\\Archive\\VersusResult04",
    "Image\\Archive\\VersusResult05","Image\\Archive\\VersusResult06",
    "Image\\Archive\\VersusResult07","Image\\Archive\\VersusResult08",
}};
constexpr std::array<const char*, 8> k_merce_archives{{
    "Image\\Archive\\MerceResult01","Image\\Archive\\MerceResult02",
    "Image\\Archive\\MerceResult03","Image\\Archive\\MerceResult04",
    "Image\\Archive\\MerceResult05","Image\\Archive\\MerceResult06",
    "Image\\Archive\\MerceResult07","Image\\Archive\\MerceResult08",
}};
} // namespace

void test_fun_00407250() {
    using namespace re5::recovered;

    FUN_00407250_SetServices(nullptr);
    FUN_00407250();

    State no_dispatch_state{};
    FUN_00407250_Services no_dispatch_services{&no_dispatch_state, nullptr};
    FUN_00407250_SetServices(&no_dispatch_services);
    FUN_00407250();
    assert(no_dispatch_state.count == 0U);

    State state{};
    FUN_00407250_Services services{&state, dispatch_resource_group};
    FUN_00407250_SetServices(&services);
    FUN_00407250();

    assert(state.count == 44U);

    for (unsigned i = 0; i < 11U; ++i) {
        const auto& call = state.calls[i];
        assert(call.message_index == static_cast<std::int32_t>(i));
        assert(std::strcmp(call.archive_name, "Image\\Archive\\GameResource") == 0);
        assert(call.direct_table == 0x01567590U);
        assert(call.direct_count == 15);
        assert(call.resolved_table == 0x01567608U);
        assert(call.resolved_count == 7);
    }

    unsigned cursor = 11U;
    for (unsigned i = 0; i < k_chapter_archives.size(); ++i, ++cursor) {
        const auto& call = state.calls[cursor];
        assert(call.message_index == -1);
        assert(std::strcmp(call.archive_name, k_chapter_archives[i]) == 0);
        assert(call.direct_table == 0x01567C10U + static_cast<std::uintptr_t>(i) * 0x10U);
        assert(call.direct_count == 2);
        assert(call.resolved_table == 0U);
        assert(call.resolved_count == 0);
    }
    for (unsigned i = 0; i < k_versus_archives.size(); ++i, ++cursor) {
        const auto& call = state.calls[cursor];
        assert(call.message_index == -1);
        assert(std::strcmp(call.archive_name, k_versus_archives[i]) == 0);
        assert(call.direct_table == 0x01567D50U + static_cast<std::uintptr_t>(i) * 0x10U);
        assert(call.direct_count == 2);
        assert(call.resolved_table == 0U);
        assert(call.resolved_count == 0);
    }
    for (unsigned i = 0; i < k_merce_archives.size(); ++i, ++cursor) {
        const auto& call = state.calls[cursor];
        assert(call.message_index == -1);
        assert(std::strcmp(call.archive_name, k_merce_archives[i]) == 0);
        assert(call.direct_table == 0x01567DF0U + static_cast<std::uintptr_t>(i) * 0x10U);
        assert(call.direct_count == 2);
        assert(call.resolved_table == 0U);
        assert(call.resolved_count == 0);
    }

    const auto& bench = state.calls[cursor];
    assert(bench.message_index == -1);
    assert(std::strcmp(bench.archive_name, "Image\\Archive\\BenchResource") == 0);
    assert(bench.direct_table == 0x01567E90U);
    assert(bench.direct_count == 2);
    assert(bench.resolved_table == 0U);
    assert(bench.resolved_count == 0);

    FUN_00407250_SetServices(nullptr);
}
