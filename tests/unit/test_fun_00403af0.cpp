#include "re5/recovered/fun_00403af0.hpp"

#include <array>
#include <cassert>
#include <cstring>

namespace {
struct State {
    std::array<re5::recovered::FUN_00403AF0_Entry, 4> entries{};
    re5::recovered::FUN_00403AF0_TableRecord record{"known", &record, "suffix"};
    unsigned count = 0;
    unsigned index = 0;
    unsigned ensured = 0;
    unsigned scans = 0;
    unsigned ends = 0;
    unsigned binds = 0;
    unsigned missing = 0;
    char bound_path[0x104]{};
};

bool validate_name(void*, const char* name) noexcept {
    return name != nullptr && name[0] != '\0';
}

bool registry_ready(void*) noexcept {
    return false;
}

void ensure_registry(void* context, std::uintptr_t registry) noexcept {
    assert(registry == 0x016E20DCU);
    auto& state = *static_cast<State*>(context);
    ++state.ensured;
}

bool begin_scan(void* context, const char*) noexcept {
    auto& state = *static_cast<State*>(context);
    state.index = 0;
    ++state.scans;
    return true;
}

bool next_entry(void* context, re5::recovered::FUN_00403AF0_Entry& entry) noexcept {
    auto& state = *static_cast<State*>(context);
    if (state.index >= state.count) {
        return false;
    }
    entry = state.entries[state.index++];
    return true;
}

bool end_scan(void* context) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.ends;
    return true;
}

const re5::recovered::FUN_00403AF0_TableRecord* find_record(void* context, const char* leaf) noexcept {
    auto& state = *static_cast<State*>(context);
    return std::strcmp(leaf, "known") == 0 ? &state.record : nullptr;
}

void bind_record(
    void* context,
    const re5::recovered::FUN_00403AF0_TableRecord& record,
    const char* path) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(record.object == &state.record);
    std::strncpy(state.bound_path, path, sizeof(state.bound_path) - 1U);
    ++state.binds;
}

void report_missing(void* context, const char*) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.missing;
}
} // namespace

void test_fun_00403af0() {
    using namespace re5::recovered;

    State state{};
    state.entries[0] = FUN_00403AF0_Entry{".", false};
    state.entries[1] = FUN_00403AF0_Entry{"known.arc", false};
    state.entries[2] = FUN_00403AF0_Entry{"missing.arc", false};
    state.count = 3;

    FUN_00403AF0_Services services{
        "",
        0x016E20DCU,
        &state,
        validate_name,
        registry_ready,
        ensure_registry,
        begin_scan,
        next_entry,
        end_scan,
        find_record,
        bind_record,
        report_missing,
    };
    FUN_00403AF0_SetServices(&services);

    FUN_00403AF0_NameRef root{"root"};
    FUN_00403AF0(&root);

    assert(state.ensured == 1U);
    assert(state.scans == 1U);
    assert(state.ends == 1U);
    assert(state.binds == 1U);
    assert(state.missing == 1U);
    assert(std::strcmp(state.bound_path, "root/known.arc") == 0);

    FUN_00403AF0_SetServices(nullptr);
}
