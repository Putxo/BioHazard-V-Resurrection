#include "re5/recovered/fun_00406010.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

namespace {
using re5::recovered::FUN_00406010_Entry;
using re5::recovered::FUN_00406010_Services;

struct Scan {
    std::vector<FUN_00406010_Entry> entries;
    std::size_t index = 0U;
};

struct State {
    unsigned opens = 0U;
    unsigned begins = 0U;
    unsigned ends = 0U;
    unsigned errors = 0U;
    unsigned resolves = 0U;
    unsigned registers = 0U;
    bool fail_open = false;
    std::vector<std::string> wildcards;
    std::vector<std::string> resolved_paths;
    std::vector<void*> registered;
    Scan top{{
        {".", true},
        {"..", true},
        {"sub", true},
        {"hero.lot", false},
        {"ignore.sce", false},
        {"nested.lot.backup", false},
    }};
    Scan sub{{
        {"child.lot", false},
    }};
};

void* open_type(void* context, const char* key, std::uintptr_t table) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.opens;
    assert(key != nullptr && std::strcmp(key, "rLayout") == 0);
    assert(table == 0x01652EF0U);
    if (state.fail_open) {
        return nullptr;
    }
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(0x6000U + state.opens));
}

void* begin_scan(void* context, const char* wildcard) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.begins;
    assert(wildcard != nullptr);
    state.wildcards.emplace_back(wildcard);
    if (std::strcmp(wildcard, "root\\stage\\*") == 0) {
        state.top.index = 0U;
        return &state.top;
    }
    if (std::strcmp(wildcard, "root\\stage\\sub\\*") == 0) {
        state.sub.index = 0U;
        return &state.sub;
    }
    return nullptr;
}

bool next_entry(void*, void* scan_ptr, FUN_00406010_Entry& entry) noexcept {
    auto& scan = *static_cast<Scan*>(scan_ptr);
    if (scan.index >= scan.entries.size()) {
        return false;
    }
    entry = scan.entries[scan.index++];
    return true;
}

void end_scan(void* context, void* scan) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(scan == &state.top || scan == &state.sub);
    ++state.ends;
}

void report_path_error(void* context, const char*, const char*) noexcept {
    ++static_cast<State*>(context)->errors;
}

void* resolve_leaf(void* context, void* type_root, const char* path, bool create) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(type_root != nullptr);
    assert(create);
    assert(path != nullptr);
    ++state.resolves;
    state.resolved_paths.emplace_back(path);
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(0x7000U + state.resolves));
}

void register_resolved(void* context, void* resolved) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(resolved != nullptr);
    ++state.registers;
    state.registered.push_back(resolved);
}

FUN_00406010_Services make_services(State& state) {
    return {
        &state,
        "root",
        "",
        0x01652EF0U,
        false,
        false,
        open_type,
        begin_scan,
        next_entry,
        end_scan,
        report_path_error,
        resolve_leaf,
        register_resolved,
    };
}
} // namespace

void test_fun_00406010() {
    using namespace re5::recovered;

    FUN_00406010_SetServices(nullptr);
    FUN_00406010("stage", "rLayout", ".lot");

    State missing{};
    missing.fail_open = true;
    auto missing_services = make_services(missing);
    FUN_00406010_SetServices(&missing_services);
    FUN_00406010("stage", "rLayout", ".lot");
    assert(missing.opens == 1U);
    assert(missing.begins == 0U);
    assert(missing.ends == 0U);
    assert(missing.resolves == 0U);
    assert(missing.registers == 0U);

    State state{};
    auto services = make_services(state);
    FUN_00406010_SetServices(&services);
    FUN_00406010("stage", "rLayout", ".lot");
    assert(state.opens == 2U);
    assert(state.begins == 2U);
    assert(state.ends == 2U);
    assert(state.errors == 0U);
    assert(state.wildcards.size() == 2U);
    assert(state.resolves == 3U);
    assert(state.registers == 3U);
    assert(state.resolved_paths.size() == 3U);
    assert(state.resolved_paths[0] == "stage\\sub\\child");
    assert(state.resolved_paths[1] == "stage\\hero");
    assert(state.resolved_paths[2] == "stage\\nested");

    State suppressed{};
    auto suppressed_services = make_services(suppressed);
    suppressed_services.registry_suppressed_b = true;
    FUN_00406010_SetServices(&suppressed_services);
    FUN_00406010("stage", "rLayout", ".lot");
    assert(suppressed.resolves == 3U);
    assert(suppressed.registers == 0U);

    FUN_00406010_SetServices(nullptr);
}
