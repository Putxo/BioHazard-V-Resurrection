#include "re5/recovered/fun_00405a60.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace {
constexpr std::array<const char*, 6> k_expected_paths{{
    "id\\jpn\\menu",
    "etc\\message\\mes_tex",
    "etc\\message\\mes_tex",
    "etc\\message\\cursor_ps3_dc",
    "etc\\message\\cursor_ps3_dc",
    "etc\\vibration_dc",
}};
constexpr std::array<std::uintptr_t, 6> k_expected_keys{{
    0x0165BB68U,
    0x016E21B4U,
    0x01692E3CU,
    0x016E21B4U,
    0x01692E3CU,
    0x016E22B0U,
}};

struct State {
    unsigned traces = 0U;
    unsigned formatted = 0U;
    unsigned prepared = 0U;
    unsigned registered = 0U;
    unsigned finalized = 0U;
    unsigned cancel_checks = 0U;
    unsigned loading_gets = 0U;
    unsigned loading_sets = 0U;
    unsigned begin_batches = 0U;
    unsigned end_batches = 0U;
    unsigned binds = 0U;
    unsigned static_index = 0U;
    bool cancel = false;
    std::uint8_t loading = 0U;
    bool saw_end_trace = false;
    bool saw_primary_batch = false;
};

std::uint8_t get_global_loading(void* context) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.loading_gets;
    return state.loading;
}

void set_global_loading(void* context, std::uint8_t value) noexcept {
    auto& state = *static_cast<State*>(context);
    state.loading = value;
    ++state.loading_sets;
}

void trace_marker(void* context, const char* marker) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(marker != nullptr);
    ++state.traces;
    if (std::strcmp(marker, "0x137AF90") == 0) {
        state.saw_end_trace = true;
    }
}

void format_path(
    void* context,
    const char* marker,
    const char* first,
    const char* second,
    char* out,
    std::size_t out_size) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(marker != nullptr);
    assert(first != nullptr);
    assert(out != nullptr);
    assert(out_size == 0x104U);
    ++state.formatted;

    const char* replacement = nullptr;
    if (std::strcmp(marker, "0x137B518") == 0) {
        assert(second == nullptr);
        assert(std::strcmp(first, "Image\\Archive\\Core2Resource") == 0);
        replacement = "primary";
    } else {
        assert(std::strcmp(marker, "0x137AAD4") == 0);
        assert(second != nullptr);
        assert(std::strcmp(first, "root") == 0);
        assert(std::strcmp(second, "primary") == 0);
        replacement = "secondary";
    }
    std::strncpy(out, replacement, out_size - 1U);
    out[out_size - 1U] = '\0';
}

void prepare_secondary(void* context, const char* path) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(std::strcmp(path, "secondary") == 0);
    ++state.prepared;
}

void* register_named(
    void* context,
    const char* path,
    std::uintptr_t key_token,
    bool enabled) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(enabled);
    ++state.registered;

    if (key_token == 0x016E216CU) {
        assert(std::strcmp(path, "primary") == 0);
        return reinterpret_cast<void*>(static_cast<std::uintptr_t>(0x1000U));
    }

    assert(state.static_index < k_expected_paths.size());
    assert(std::strcmp(path, k_expected_paths[state.static_index]) == 0);
    assert(key_token == k_expected_keys[state.static_index]);
    ++state.static_index;
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(0x2000U + state.static_index));
}

void finalize_registered(void* context, void* registered) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(registered != nullptr);
    ++state.finalized;
}

bool check_cancel(void* context, const char* path) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(std::strcmp(path, "secondary") == 0);
    ++state.cancel_checks;
    return state.cancel;
}

void begin_batch(void* context, const char* primary_path) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(std::strcmp(primary_path, "primary") == 0);
    state.saw_primary_batch = true;
    ++state.begin_batches;
}

void end_batch(void* context, bool enabled) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(!enabled);
    ++state.end_batches;
}

void bind_registered(void* context, void** registered_slot) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(registered_slot != nullptr);
    assert(*registered_slot != nullptr);
    ++state.binds;
}

re5::recovered::FUN_00405A60_Services make_services(State& state) {
    return {
        &state,
        "root",
        false,
        false,
        false,
        get_global_loading,
        set_global_loading,
        trace_marker,
        format_path,
        prepare_secondary,
        register_named,
        finalize_registered,
        check_cancel,
        begin_batch,
        end_batch,
        bind_registered,
    };
}
} // namespace

void test_fun_00405a60() {
    using namespace re5::recovered;

    FUN_00405A60_SetServices(nullptr);
    FUN_00405A60();

    State cancel_state{};
    cancel_state.cancel = true;
    auto cancel_services = make_services(cancel_state);
    cancel_services.global_special_enabled = true;
    FUN_00405A60_SetServices(&cancel_services);
    FUN_00405A60();
    assert(cancel_state.traces == 1U);
    assert(cancel_state.formatted == 2U);
    assert(cancel_state.prepared == 1U);
    assert(cancel_state.registered == 1U);
    assert(cancel_state.finalized == 1U);
    assert(cancel_state.cancel_checks == 1U);
    assert(cancel_state.loading_gets == 0U);
    assert(cancel_state.loading_sets == 0U);
    assert(cancel_state.begin_batches == 0U);
    assert(cancel_state.end_batches == 0U);
    assert(cancel_state.binds == 0U);
    assert(!cancel_state.saw_end_trace);

    State state{};
    state.loading = 0x7FU;
    auto services = make_services(state);
    FUN_00405A60_SetServices(&services);
    FUN_00405A60();
    assert(state.traces == 2U);
    assert(state.saw_end_trace);
    assert(state.formatted == 2U);
    assert(state.registered == 6U);
    assert(state.static_index == 6U);
    assert(state.loading_gets == 1U);
    assert(state.loading_sets == 2U);
    assert(state.loading == 0x7FU);
    assert(state.begin_batches == 1U);
    assert(state.end_batches == 1U);
    assert(state.saw_primary_batch);
    assert(state.binds == 6U);

    State suppressed_state{};
    auto suppressed_services = make_services(suppressed_state);
    suppressed_services.registry_suppressed_a = true;
    FUN_00405A60_SetServices(&suppressed_services);
    FUN_00405A60();
    assert(suppressed_state.registered == 6U);
    assert(suppressed_state.binds == 0U);

    FUN_00405A60_SetServices(nullptr);
}
