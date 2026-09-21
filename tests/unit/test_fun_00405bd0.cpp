#include "re5/recovered/fun_00405bd0.hpp"

#include <cassert>
#include <cstdint>
#include <cstring>

namespace {
struct State {
    std::uint32_t message_index = 7U;
    bool loading = false;
    bool cancel = false;
    unsigned index_exchanges = 0U;
    unsigned loading_exchanges = 0U;
    unsigned resolves = 0U;
    unsigned prepares = 0U;
    unsigned registers = 0U;
    unsigned finalizes = 0U;
    unsigned cancel_checks = 0U;
    unsigned begin_batches = 0U;
    unsigned start_batches = 0U;
    unsigned end_batches = 0U;
    unsigned binds = 0U;
    unsigned logs = 0U;
};

std::uint32_t exchange_message_index(void* context, std::uint32_t value) noexcept {
    auto& state = *static_cast<State*>(context);
    const std::uint32_t previous = state.message_index;
    state.message_index = value;
    ++state.index_exchanges;
    return previous;
}

bool exchange_global_loading(void* context, bool enabled) noexcept {
    auto& state = *static_cast<State*>(context);
    const bool previous = state.loading;
    state.loading = enabled;
    ++state.loading_exchanges;
    return previous;
}

const char* resolve_path(void* context, const char* path) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(path != nullptr);
    ++state.resolves;
    return path;
}

void prepare_archive(void* context, const char* archive_path) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(std::strcmp(archive_path, "global\\Image\\Archive\\MsgResource.arc") == 0);
    ++state.prepares;
}

bool register_named(void* context, const char* path, const char* key, bool enabled) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(enabled);
    assert(path != nullptr);
    assert(key != nullptr);
    if (state.registers == 0U) {
        assert(std::strcmp(path, "Image\\Archive\\MsgResource") == 0);
        assert(std::strcmp(key, "0x16E216C") == 0);
    } else {
        assert(std::strncmp(path, "etc\\message\\mes_", 16U) == 0);
        assert(std::strcmp(key, "0x1692FC8") == 0);
    }
    ++state.registers;
    return true;
}

void finalize_registered(void* context, bool* registered_value) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(registered_value != nullptr);
    assert(*registered_value);
    ++state.finalizes;
}

bool check_cancel(void* context, const char* archive_path) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(std::strcmp(archive_path, "global\\Image\\Archive\\MsgResource.arc") == 0);
    ++state.cancel_checks;
    return state.cancel;
}

void begin_batch(void* context, const char* base_path) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(std::strcmp(base_path, "Image\\Archive\\MsgResource") == 0);
    ++state.begin_batches;
}

void start_batch(void* context) noexcept {
    ++static_cast<State*>(context)->start_batches;
}

void end_batch(void* context, bool value) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(!value);
    ++state.end_batches;
}

void bind_registered(void* context, bool* registered_value) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(registered_value != nullptr);
    assert(*registered_value);
    ++state.binds;
}

void log_marker(void* context, const char* marker) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(std::strcmp(marker, "0x137AF90") == 0);
    ++state.logs;
}

re5::recovered::FUN_00405BD0_Services make_services(State& state) noexcept {
    return {
        &state,
        "global",
        true,
        false,
        false,
        exchange_message_index,
        exchange_global_loading,
        resolve_path,
        prepare_archive,
        register_named,
        finalize_registered,
        check_cancel,
        begin_batch,
        start_batch,
        end_batch,
        bind_registered,
        log_marker,
    };
}
} // namespace

void test_fun_00405bd0() {
    using namespace re5::recovered;

    State normal{};
    auto normal_services = make_services(normal);
    FUN_00405BD0_SetServices(&normal_services);
    FUN_00405BD0(3U);

    assert(normal.message_index == 7U);
    assert(!normal.loading);
    assert(normal.index_exchanges == 2U);
    assert(normal.loading_exchanges == 2U);
    assert(normal.resolves == 10U);
    assert(normal.prepares == 1U);
    assert(normal.registers == 10U);
    assert(normal.finalizes == 1U);
    assert(normal.cancel_checks == 1U);
    assert(normal.begin_batches == 1U);
    assert(normal.start_batches == 1U);
    assert(normal.end_batches == 1U);
    assert(normal.binds == 9U);
    assert(normal.logs == 1U);

    State early{};
    early.message_index = 9U;
    early.loading = true;
    early.cancel = true;
    auto early_services = make_services(early);
    FUN_00405BD0_SetServices(&early_services);
    FUN_00405BD0(4U);

    assert(early.message_index == 9U);
    assert(early.loading);
    assert(early.index_exchanges == 2U);
    assert(early.loading_exchanges == 2U);
    assert(early.resolves == 1U);
    assert(early.prepares == 1U);
    assert(early.registers == 1U);
    assert(early.finalizes == 1U);
    assert(early.cancel_checks == 1U);
    assert(early.begin_batches == 0U);
    assert(early.start_batches == 0U);
    assert(early.end_batches == 0U);
    assert(early.binds == 0U);
    assert(early.logs == 1U);

    FUN_00405BD0_SetServices(nullptr);
    FUN_00405BD0(0U);
}
