#include "re5/recovered/fun_00403150.hpp"

#include <array>
#include <cassert>
#include <cstring>

namespace {
struct ScanRoot {
    std::array<re5::recovered::FUN_00403150_Entry, 4> entries{};
    unsigned count = 0;
    unsigned index = 0;
};

struct State {
    ScanRoot root{};
    std::array<char, 4 * 0x104> leaves{};
    std::array<char, 4 * 0x104> errors{};
    unsigned opens = 0;
    unsigned closes = 0;
    unsigned scans = 0;
    unsigned ends = 0;
    unsigned resolved = 0;
    unsigned activated = 0;
};

void* open_root(void* context, const char*, std::uintptr_t table) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(table == 0x01652EF0U);
    state.root.index = 0;
    ++state.opens;
    return &state.root;
}

void close_root(void* context, void*) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.closes;
}

bool begin_scan(void* context, void*) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.scans;
    return true;
}

bool next_entry(void*, void* root, re5::recovered::FUN_00403150_Entry& entry) noexcept {
    auto& scan = *static_cast<ScanRoot*>(root);
    if (scan.index >= scan.count) {
        return false;
    }
    entry = scan.entries[scan.index++];
    return true;
}

bool end_scan(void* context, void*) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.ends;
    return true;
}

void report_error(void* context, const char* message) noexcept {
    auto& state = *static_cast<State*>(context);
    std::strncpy(&state.errors[state.resolved * 0x104], message, 0x103);
}

void* resolve_leaf(void* context, void*, const char* leaf, bool create) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(create);
    std::strncpy(&state.leaves[state.resolved * 0x104], leaf, 0x103);
    ++state.resolved;
    return &state;
}

void activate(void* context, void*) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.activated;
}
} // namespace

void test_fun_00403150() {
    using namespace re5::recovered;

    State state{};
    state.root.entries[0] = FUN_00403150_Entry{".", false};
    state.root.entries[1] = FUN_00403150_Entry{"file.arc", false};
    state.root.count = 2;

    FUN_00403150_Services services{
        "active",
        "",
        "%s:%s",
        "%s:%s",
        "%s/%s",
        "%s",
        0x01652EF0U,
        &state,
        open_root,
        close_root,
        begin_scan,
        next_entry,
        end_scan,
        report_error,
        resolve_leaf,
        activate,
    };
    FUN_00403150_SetServices(&services);

    int registry = 0;
    FUN_00403150("root", "stage", &registry);

    assert(state.opens == 1U);
    assert(state.closes == 1U);
    assert(state.scans == 1U);
    assert(state.resolved == 1U);
    assert(state.activated == 1U);
    assert(std::strcmp(&state.leaves[0], "root/file") == 0);

    FUN_00403150_SetServices(nullptr);
}
