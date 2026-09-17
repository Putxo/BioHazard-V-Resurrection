#include "re5/recovered/fun_00403640.hpp"

#include <array>
#include <cassert>
#include <cstring>

namespace {
struct Root {
    std::array<re5::recovered::FUN_00403640_Entry, 4> entries{};
    unsigned count = 0;
    unsigned index = 0;
};

struct State {
    Root root{};
    int value = 0x3640;
    char bound_path[0x104]{};
    unsigned opens = 0;
    unsigned closes = 0;
    unsigned ensured = 0;
    unsigned bound = 0;
};

bool validate_handle(void*, const void* handle) noexcept {
    return handle != nullptr;
}

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

bool is_registry_ready(void*, std::uintptr_t registry) noexcept {
    assert(registry == 0x016E20DCU);
    return false;
}

void ensure_registry(void* context, std::uintptr_t) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.ensured;
}

bool next_entry(void*, void* root, re5::recovered::FUN_00403640_Entry& entry) noexcept {
    auto& scan = *static_cast<Root*>(root);
    if (scan.index >= scan.count) {
        return false;
    }
    entry = scan.entries[scan.index++];
    return true;
}

bool known_leaf(void*, const char* leaf) noexcept {
    return std::strcmp(leaf, "known") == 0;
}

const void* lookup_leaf(void* context, const char* leaf) noexcept {
    auto& state = *static_cast<State*>(context);
    return std::strcmp(leaf, "known") == 0 ? &state.value : nullptr;
}

void bind_leaf(void* context, const void* value, const char* path) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(value == &state.value);
    std::strncpy(state.bound_path, path, sizeof(state.bound_path) - 1U);
    ++state.bound;
}
} // namespace

void test_fun_00403640() {
    using namespace re5::recovered;

    State state{};
    state.root.entries[0] = FUN_00403640_Entry{".", false};
    state.root.entries[1] = FUN_00403640_Entry{"known.arc", false};
    state.root.entries[2] = FUN_00403640_Entry{"unknown.arc", false};
    state.root.count = 3;

    FUN_00403640_Services services{
        "active",
        "",
        "root",
        0x01652EF0U,
        0x016E20DCU,
        &state,
        validate_handle,
        open_root,
        close_root,
        is_registry_ready,
        ensure_registry,
        next_entry,
        known_leaf,
        lookup_leaf,
        bind_leaf,
    };
    FUN_00403640_SetServices(&services);

    int object = 0;
    int handle = 1;
    FUN_00403640(&object, &handle, "base", "stage");

    assert(state.opens == 1U);
    assert(state.closes == 1U);
    assert(state.ensured == 1U);
    assert(state.bound == 1U);
    assert(std::strcmp(state.bound_path, "base/known.arc") == 0);

    FUN_00403640_SetServices(nullptr);
}
