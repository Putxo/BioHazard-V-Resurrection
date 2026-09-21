#include "re5/recovered/fun_00404f90.hpp"

#include <array>
#include <cassert>
#include <cstring>

namespace {
struct State {
    unsigned markers = 0;
    unsigned entries = 0;
    unsigned groups = 0;
    char first_marker[32]{};
    char last_marker[32]{};
};

void log_marker(void* context, const char* marker) noexcept {
    auto& state = *static_cast<State*>(context);
    if (state.markers == 0U) {
        std::strncpy(state.first_marker, marker, sizeof(state.first_marker) - 1U);
    }
    std::strncpy(state.last_marker, marker, sizeof(state.last_marker) - 1U);
    ++state.markers;
}

void register_entry(
    void* context,
    const re5::recovered::FUN_00404F90_TableEntry& entry,
    bool enabled) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(enabled);
    assert(entry.name != nullptr);
    assert(entry.path != nullptr);
    ++state.entries;
}

void register_group(void* context, const char* first, const char* second, const char* third) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(std::strcmp(first, "0x137AFC0") == 0);
    assert(std::strcmp(second, "0x137AFD0") == 0);
    assert(std::strcmp(third, "0x137AFE4") == 0);
    ++state.groups;
}
} // namespace

void test_fun_00404f90() {
    using namespace re5::recovered;

    State state{};
    std::array<FUN_00404F90_TableEntry, 48> table{};
    for (std::size_t i = 0; i < table.size(); ++i) {
        table[i] = {"name", "path"};
    }

    FUN_00404F90_Services services{
        &state,
        table.data(),
        table.size(),
        log_marker,
        register_entry,
        register_group,
    };
    FUN_00404F90_SetServices(&services);
    FUN_00404F90();

    assert(state.markers == 3U);
    assert(state.entries == 48U);
    assert(state.groups == 1U);
    assert(std::strcmp(state.first_marker, "0x137AFEC") == 0);
    assert(std::strcmp(state.last_marker, "0x137AF90") == 0);

    FUN_00404F90_SetServices(nullptr);
}
