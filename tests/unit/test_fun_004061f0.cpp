#include "re5/recovered/fun_004061f0.hpp"

#include <cassert>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace {
using re5::recovered::FUN_004061F0_Entry;
using re5::recovered::FUN_004061F0_Services;

struct RegisterCall {
    std::string path;
    std::uint32_t key_address{};
    bool enabled{};
};

struct TestState {
    std::int32_t message_index{9};
    bool loading{};
    bool cancel{};
    int indexed_resolves{};
    int table_resolves{};
    int prepare_calls{};
    int finalize_calls{};
    int cancel_checks{};
    int begin_calls{};
    int start_calls{};
    int end_calls{};
    int bind_calls{};
    std::string prepared_path;
    std::string batch_path;
    std::string checked_path;
    std::string resolved_table_storage;
    std::vector<std::int32_t> index_writes;
    std::vector<bool> loading_writes;
    std::vector<std::string> markers;
    std::vector<RegisterCall> registrations;
};

std::int32_t read_message_index(void* context) noexcept {
    return static_cast<TestState*>(context)->message_index;
}

void write_message_index(void* context, std::int32_t value) noexcept {
    auto& state = *static_cast<TestState*>(context);
    state.message_index = value;
    state.index_writes.push_back(value);
}

bool exchange_loading(void* context, bool enabled) noexcept {
    auto& state = *static_cast<TestState*>(context);
    const bool previous = state.loading;
    state.loading = enabled;
    state.loading_writes.push_back(enabled);
    return previous;
}

const char* resolve_indexed_path(void* context, const char*) noexcept {
    auto& state = *static_cast<TestState*>(context);
    ++state.indexed_resolves;
    return "resolved\\archive";
}

const char* resolve_table_path(void* context, const char* path) noexcept {
    auto& state = *static_cast<TestState*>(context);
    ++state.table_resolves;
    state.resolved_table_storage = "resolved:";
    state.resolved_table_storage += path != nullptr ? path : "";
    return state.resolved_table_storage.c_str();
}

void prepare_archive(void* context, const char* path) noexcept {
    auto& state = *static_cast<TestState*>(context);
    ++state.prepare_calls;
    state.prepared_path = path != nullptr ? path : "";
}

void* register_named(
    void* context,
    const char* path,
    std::uint32_t key_address,
    bool enabled) noexcept {
    auto& state = *static_cast<TestState*>(context);
    state.registrations.push_back({path != nullptr ? path : "", key_address, enabled});
    return reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(0x1000U + state.registrations.size()));
}

void finalize_registered(void* context, void* registered) noexcept {
    auto& state = *static_cast<TestState*>(context);
    assert(registered != nullptr);
    ++state.finalize_calls;
}

bool check_cancel(void* context, const char* path) noexcept {
    auto& state = *static_cast<TestState*>(context);
    ++state.cancel_checks;
    state.checked_path = path != nullptr ? path : "";
    return state.cancel;
}

void begin_batch(void* context, const char* path) noexcept {
    auto& state = *static_cast<TestState*>(context);
    ++state.begin_calls;
    state.batch_path = path != nullptr ? path : "";
}

void start_batch(void* context) noexcept {
    ++static_cast<TestState*>(context)->start_calls;
}

void end_batch(void* context, bool value) noexcept {
    auto& state = *static_cast<TestState*>(context);
    assert(!value);
    ++state.end_calls;
}

void bind_registered(void* context, void** registered) noexcept {
    auto& state = *static_cast<TestState*>(context);
    assert(registered != nullptr);
    assert(*registered != nullptr);
    ++state.bind_calls;
}

void log_marker(void* context, const char* marker) noexcept {
    static_cast<TestState*>(context)->markers.emplace_back(marker != nullptr ? marker : "");
}

FUN_004061F0_Services make_services(TestState& state) {
    return {
        &state,
        "root",
        true,
        false,
        false,
        read_message_index,
        write_message_index,
        exchange_loading,
        resolve_indexed_path,
        resolve_table_path,
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

void test_normal_path() {
    TestState state;
    auto services = make_services(state);
    re5::recovered::FUN_004061F0_SetServices(&services);

    const FUN_004061F0_Entry direct_entries[] = {
        {"direct-a", 0x11111111U},
        {"direct-b", 0x22222222U},
    };
    const FUN_004061F0_Entry resolved_entries[] = {
        {"resolve-a", 0x33333333U},
        {"resolve-b", 0x44444444U},
    };

    re5::recovered::FUN_004061F0(
        4,
        "Image\\Archive\\GameResource",
        direct_entries,
        2,
        resolved_entries,
        2);

    assert((state.index_writes == std::vector<std::int32_t>{4, 9}));
    assert((state.loading_writes == std::vector<bool>{true, false}));
    assert(state.message_index == 9);
    assert(!state.loading);
    assert(state.indexed_resolves == 1);
    assert(state.table_resolves == 2);
    assert(state.prepare_calls == 1);
    assert(state.prepared_path == "root\\resolved\\archive.arc");
    assert(state.cancel_checks == 1);
    assert(state.checked_path == state.prepared_path);
    assert(state.finalize_calls == 1);
    assert(state.begin_calls == 1);
    assert(state.start_calls == 1);
    assert(state.end_calls == 1);
    assert(state.batch_path == "resolved\\archive");
    assert(state.registrations.size() == 5U);
    assert(state.registrations[0].path == "resolved\\archive");
    assert(state.registrations[0].key_address == 0x016E216CU);
    assert(state.registrations[1].path == "direct-a");
    assert(state.registrations[1].key_address == 0x11111111U);
    assert(state.registrations[2].path == "direct-b");
    assert(state.registrations[3].path == "resolved:resolve-a");
    assert(state.registrations[3].key_address == 0x33333333U);
    assert(state.registrations[4].path == "resolved:resolve-b");
    assert(state.bind_calls == 4);
    assert((state.markers == std::vector<std::string>{"0x0137AFEC", "0x0137AF90"}));
}

void test_special_cancel_early_exit() {
    TestState state;
    state.cancel = true;
    auto services = make_services(state);
    re5::recovered::FUN_004061F0_SetServices(&services);

    const FUN_004061F0_Entry direct_entries[] = {{"must-not-run", 1U}};
    const FUN_004061F0_Entry resolved_entries[] = {{"must-not-resolve", 2U}};

    re5::recovered::FUN_004061F0(
        3,
        "archive",
        direct_entries,
        1,
        resolved_entries,
        1);

    assert(state.registrations.size() == 1U);
    assert(state.finalize_calls == 1);
    assert(state.cancel_checks == 1);
    assert(state.begin_calls == 0);
    assert(state.start_calls == 0);
    assert(state.end_calls == 0);
    assert(state.bind_calls == 0);
    assert(state.table_resolves == 0);
    assert((state.index_writes == std::vector<std::int32_t>{3, 9}));
    assert((state.loading_writes == std::vector<bool>{true, false}));
    assert(state.message_index == 9);
    assert(!state.loading);
    assert((state.markers == std::vector<std::string>{"0x0137AFEC", "0x0137AF90"}));
}

void test_negative_index_uses_raw_resource() {
    TestState state;
    auto services = make_services(state);
    services.archive_special_enabled = false;
    re5::recovered::FUN_004061F0_SetServices(&services);

    re5::recovered::FUN_004061F0(
        -1,
        "raw\\resource",
        nullptr,
        4,
        nullptr,
        3);

    assert(state.indexed_resolves == 0);
    assert((state.index_writes == std::vector<std::int32_t>{9}));
    assert(state.batch_path == "raw\\resource");
    assert(state.registrations.empty());
    assert(state.begin_calls == 1);
    assert(state.start_calls == 1);
    assert(state.end_calls == 1);
    assert(state.message_index == 9);
}

void test_nonpositive_counts_skip_tables() {
    TestState state;
    auto services = make_services(state);
    services.archive_special_enabled = false;
    re5::recovered::FUN_004061F0_SetServices(&services);

    const FUN_004061F0_Entry direct_entries[] = {{"direct", 1U}};
    const FUN_004061F0_Entry resolved_entries[] = {{"resolved", 2U}};

    re5::recovered::FUN_004061F0(
        0,
        "archive",
        direct_entries,
        0,
        resolved_entries,
        -2);

    assert(state.registrations.empty());
    assert(state.table_resolves == 0);
    assert(state.begin_calls == 1);
    assert(state.start_calls == 1);
    assert(state.end_calls == 1);
}
} // namespace

void test_fun_004061f0() {
    re5::recovered::FUN_004061F0_SetServices(nullptr);
    re5::recovered::FUN_004061F0(0, nullptr, nullptr, 0, nullptr, 0);

    test_normal_path();
    test_special_cancel_early_exit();
    test_negative_index_uses_raw_resource();
    test_nonpositive_counts_skip_tables();

    re5::recovered::FUN_004061F0_SetServices(nullptr);
}
