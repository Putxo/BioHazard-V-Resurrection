#include "re5/recovered/fun_00405710.hpp"

#include <cassert>
#include <cstring>

namespace {
struct State {
    re5::recovered::FUN_00405710_Child child{};
    unsigned formatted = 0;
    unsigned registered = 0;
    unsigned opened = 0;
    unsigned ready_checks = 0;
    unsigned releases = 0;
    unsigned loading_sets = 0;
    unsigned begin_batches = 0;
    unsigned end_batches = 0;
    unsigned binds = 0;
    bool cancel = false;
    bool loading = false;
};

void format_path(void* context, const char* marker, const char* value, char* out, std::size_t out_size) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.formatted;
    assert(marker != nullptr);
    assert(value != nullptr);
    if (out_size != 0U) {
        out[0] = marker[0];
        out[1] = '\0';
    }
}

bool register_named(void* context, const char* path, const char* key, bool enabled) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(enabled);
    assert(path != nullptr);
    assert(key != nullptr);
    ++state.registered;
    return true;
}

bool check_cancel(void* context, const char*) noexcept {
    return static_cast<State*>(context)->cancel;
}

re5::recovered::FUN_00405710_Child* open_child(
    void* context,
    re5::recovered::FUN_00405710_Request& request) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(std::strcmp(request.name, "request") == 0);
    ++state.opened;
    return &state.child;
}

bool child_ready(void* context, re5::recovered::FUN_00405710_Child&) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.ready_checks;
    return true;
}

void release_child(void* context, re5::recovered::FUN_00405710_Child&, bool immediate) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(immediate);
    ++state.releases;
}

void set_global_loading(void* context, bool enabled) noexcept {
    auto& state = *static_cast<State*>(context);
    state.loading = enabled;
    ++state.loading_sets;
}

void begin_batch(void* context, char* out, std::size_t out_size) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.begin_batches;
    if (out_size != 0U) {
        out[0] = 'b';
        out[1] = '\0';
    }
}

void end_batch(void* context) noexcept {
    ++static_cast<State*>(context)->end_batches;
}

void bind_registered(void* context, bool* registered_value) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(registered_value != nullptr);
    assert(*registered_value);
    ++state.binds;
}
} // namespace

void test_fun_00405710() {
    using namespace re5::recovered;

    FUN_00405710_Entry entries[] = {
        {"entry_a", "path_a"},
        {"entry_b", "path_b"},
    };
    State state{};
    state.child = {entries, 2U};

    FUN_00405710_Services services{
        &state,
        "global",
        true,
        false,
        false,
        format_path,
        register_named,
        check_cancel,
        open_child,
        child_ready,
        release_child,
        set_global_loading,
        begin_batch,
        end_batch,
        bind_registered,
    };

    FUN_00405710_SetServices(&services);
    FUN_00405710_Request skipped{"request", 1U};
    FUN_00405710(skipped);
    assert(state.opened == 0U);

    FUN_00405710_Request request{"request", 0U};
    FUN_00405710(request);

    assert(state.formatted == 2U);
    assert(state.opened == 1U);
    assert(state.ready_checks == 1U);
    assert(state.begin_batches == 2U);
    assert(state.end_batches == 2U);
    assert(state.registered == 5U);
    assert(state.binds == 4U);
    assert(state.releases == 1U);
    assert(state.loading_sets == 2U);
    assert(!state.loading);

    FUN_00405710_SetServices(nullptr);
}
