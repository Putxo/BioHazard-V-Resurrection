#include "re5/recovered/fun_00405df0.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace {
struct State {
    std::uint32_t selector = 7U;
    bool loading = true;
    bool cancel = false;
    unsigned selector_sets = 0U;
    unsigned resolved = 0U;
    unsigned formatted = 0U;
    unsigned registered = 0U;
    unsigned finalized = 0U;
    unsigned prepared = 0U;
    unsigned cancel_checks = 0U;
    unsigned begin_batches = 0U;
    unsigned end_batches = 0U;
    unsigned binds = 0U;
    unsigned finishes = 0U;
    char first_registered_path[128]{};
    char last_registered_path[128]{};
};

std::uint32_t get_selector(void* context) noexcept {
    return static_cast<State*>(context)->selector;
}

void set_selector(void* context, std::uint32_t selector) noexcept {
    auto& state = *static_cast<State*>(context);
    state.selector = selector;
    ++state.selector_sets;
}

const char* resolve_path(void* context, const char* path) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.resolved;
    return path;
}

void format_path(
    void* context,
    const char* marker,
    const char* first,
    const char* second,
    char* out,
    std::size_t out_size) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.formatted;
    assert(marker != nullptr);
    assert(first != nullptr);
    assert(out != nullptr);
    assert(out_size == 0x104U);
    const char* text = second == nullptr ? first : "archive";
    std::size_t i = 0U;
    while (text[i] != '\0' && i + 1U < out_size) {
        out[i] = text[i];
        ++i;
    }
    out[i] = '\0';
}

std::uintptr_t register_resource(
    void* context,
    std::uintptr_t destination,
    const char* path,
    bool enabled) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(enabled);
    assert(path != nullptr);
    assert(destination == 0x016E216CU || destination == 0x01692FC8U);
    auto copy_path = [](char* out, std::size_t out_size, const char* text) noexcept {
        std::size_t i = 0U;
        while (text[i] != '\0' && i + 1U < out_size) {
            out[i] = text[i];
            ++i;
        }
        out[i] = '\0';
    };
    if (state.registered == 0U) {
        copy_path(state.first_registered_path, sizeof(state.first_registered_path), path);
    }
    copy_path(state.last_registered_path, sizeof(state.last_registered_path), path);
    ++state.registered;
    return 0x1000U + state.registered;
}

void finalize_registered(void* context, std::uintptr_t value) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(value != 0U);
    ++state.finalized;
}

void prepare_archive(void* context, const char* path) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(std::strcmp(path, "archive") == 0);
    ++state.prepared;
}

bool check_cancel(void* context, const char* path) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(std::strcmp(path, "archive") == 0);
    ++state.cancel_checks;
    return state.cancel;
}

void begin_batch(void* context, const char* path) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(std::strcmp(path, "Image\\Archive\\Msg2Resource") == 0);
    ++state.begin_batches;
}

void end_batch(void* context) noexcept {
    ++static_cast<State*>(context)->end_batches;
}

void bind_registered(void* context, std::uintptr_t* value) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(value != nullptr);
    assert(*value != 0U);
    ++state.binds;
}

void finish_trace(void* context) noexcept {
    ++static_cast<State*>(context)->finishes;
}

re5::recovered::FUN_00405DF0_Services make_services(State& state) noexcept {
    return {
        &state,
        "root",
        true,
        false,
        false,
        &state.loading,
        get_selector,
        set_selector,
        resolve_path,
        format_path,
        register_resource,
        finalize_registered,
        prepare_archive,
        check_cancel,
        begin_batch,
        end_batch,
        bind_registered,
        finish_trace,
    };
}
} // namespace

void test_fun_00405df0() {
    using namespace re5::recovered;

    State state{};
    auto services = make_services(state);
    FUN_00405DF0_SetServices(&services);
    FUN_00405DF0(3U);

    assert(state.selector == 7U);
    assert(state.loading);
    assert(state.selector_sets == 2U);
    assert(state.resolved == 10U);
    assert(state.formatted == 2U);
    assert(state.registered == 10U);
    assert(state.finalized == 1U);
    assert(state.prepared == 1U);
    assert(state.cancel_checks == 1U);
    assert(state.begin_batches == 1U);
    assert(state.end_batches == 1U);
    assert(state.binds == 9U);
    assert(state.finishes == 1U);
    assert(std::strcmp(state.first_registered_path, "Image\\Archive\\Msg2Resource") == 0);
    assert(std::strcmp(state.last_registered_path, "etc\\message\\mes_stage7") == 0);

    State cancelled{};
    cancelled.selector = 5U;
    cancelled.loading = false;
    cancelled.cancel = true;
    auto cancelled_services = make_services(cancelled);
    FUN_00405DF0_SetServices(&cancelled_services);
    FUN_00405DF0(2U);

    assert(cancelled.selector == 5U);
    assert(!cancelled.loading);
    assert(cancelled.selector_sets == 2U);
    assert(cancelled.registered == 1U);
    assert(cancelled.finalized == 1U);
    assert(cancelled.cancel_checks == 1U);
    assert(cancelled.begin_batches == 0U);
    assert(cancelled.end_batches == 0U);
    assert(cancelled.binds == 0U);
    assert(cancelled.finishes == 1U);

    FUN_00405DF0_SetServices(nullptr);
    FUN_00405DF0(9U);
}
