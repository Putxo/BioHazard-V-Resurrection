#include "re5/recovered/fun_004014c0.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace {
struct Probe {
    std::vector<bool> primary_ready_results;
    std::size_t primary_ready_index{};
    bool primary_gate_result{true};
    std::uint32_t primary_gate_count{};
    std::uintptr_t primary_token{};
    std::vector<bool> flag_values;
    std::vector<std::uint32_t> sleeps;
    bool secondary_gate_result{true};
    std::uint32_t secondary_gate_count{};
    std::vector<void*> secondary_objects;
    std::size_t secondary_object_index{};
    std::vector<bool> secondary_ready_results;
    std::size_t secondary_ready_index{};
    std::vector<std::array<std::uintptr_t, 2>> primary_logs;
    std::vector<std::array<std::uintptr_t, 2>> secondary_logs;
};

Probe* g_secondary_probe = nullptr;

bool next_result(const std::vector<bool>& values, std::size_t& index, bool fallback) noexcept {
    if (index < values.size()) {
        return values[index++];
    }
    return fallback;
}

bool primary_ready(void* context) noexcept {
    auto& probe = *static_cast<Probe*>(context);
    return next_result(probe.primary_ready_results, probe.primary_ready_index, true);
}

bool primary_gate(void* context, std::uintptr_t token) noexcept {
    auto& probe = *static_cast<Probe*>(context);
    ++probe.primary_gate_count;
    probe.primary_token = token;
    return probe.primary_gate_result;
}

void primary_log(void* context, std::uintptr_t log_context, std::uintptr_t message) noexcept {
    static_cast<Probe*>(context)->primary_logs.push_back({log_context, message});
}

void set_wait_flag(void* context, bool enabled) noexcept {
    static_cast<Probe*>(context)->flag_values.push_back(enabled);
}

void sleep_ms(void* context, std::uint32_t milliseconds) noexcept {
    static_cast<Probe*>(context)->sleeps.push_back(milliseconds);
}

bool secondary_gate(void* context) noexcept {
    auto& probe = *static_cast<Probe*>(context);
    ++probe.secondary_gate_count;
    return probe.secondary_gate_result;
}

void* get_secondary_object(void* context) noexcept {
    auto& probe = *static_cast<Probe*>(context);
    g_secondary_probe = &probe;
    if (probe.secondary_object_index < probe.secondary_objects.size()) {
        return probe.secondary_objects[probe.secondary_object_index++];
    }
    return &probe;
}

bool secondary_ready(void* /*object*/) noexcept {
    assert(g_secondary_probe != nullptr);
    return next_result(
        g_secondary_probe->secondary_ready_results,
        g_secondary_probe->secondary_ready_index,
        true);
}

void secondary_log(void* context, std::uintptr_t log_context, std::uintptr_t message) noexcept {
    static_cast<Probe*>(context)->secondary_logs.push_back({log_context, message});
}

re5::recovered::FUN_004014C0_Services make_services(Probe& probe) noexcept {
    return {
        &probe,
        &primary_ready,
        &primary_gate,
        &primary_log,
        &set_wait_flag,
        &sleep_ms,
        &secondary_gate,
        &get_secondary_object,
        &secondary_ready,
        &secondary_log,
    };
}
} // namespace

void test_fun_004014c0() {
    using namespace re5::recovered;

    Probe primary_timeout{};
    primary_timeout.primary_ready_results = {false, false, false, false, false, false, false};
    primary_timeout.secondary_ready_results = {true, true};
    const auto primary_timeout_services = make_services(primary_timeout);
    FUN_004014C0_SetServices(&primary_timeout_services);
    assert(FUN_004014C0());
    FUN_004014C0_SetServices(nullptr);

    assert(primary_timeout.primary_ready_index == 7U);
    assert(primary_timeout.primary_gate_count == 1U);
    assert(primary_timeout.primary_token == 0x0004F8A8U);
    assert((primary_timeout.flag_values == std::vector<bool>{true, false}));
    assert(primary_timeout.primary_logs.size() == 1U);
    assert(primary_timeout.primary_logs[0][0] == 0x01652E00U);
    assert(primary_timeout.primary_logs[0][1] == 0x0137A744U);
    assert(primary_timeout.sleeps.size() == 5U);
    for (const auto delay : primary_timeout.sleeps) {
        assert(delay == 1000U);
    }

    Probe secondary_gate_failure{};
    secondary_gate_failure.primary_ready_results = {true};
    secondary_gate_failure.secondary_gate_result = false;
    const auto secondary_gate_failure_services = make_services(secondary_gate_failure);
    FUN_004014C0_SetServices(&secondary_gate_failure_services);
    assert(!FUN_004014C0());
    FUN_004014C0_SetServices(nullptr);

    assert(secondary_gate_failure.primary_gate_count == 0U);
    assert((secondary_gate_failure.flag_values == std::vector<bool>{false}));
    assert(secondary_gate_failure.secondary_logs.size() == 1U);
    assert(secondary_gate_failure.secondary_logs[0][0] == 0x01652E00U);
    assert(secondary_gate_failure.secondary_logs[0][1] == 0x0137A724U);

    Probe secondary_timeout_missing{};
    secondary_timeout_missing.primary_ready_results = {true};
    secondary_timeout_missing.secondary_ready_results = {false, false, false, false, false, false};
    secondary_timeout_missing.secondary_objects = {
        &secondary_timeout_missing,
        &secondary_timeout_missing,
        &secondary_timeout_missing,
        &secondary_timeout_missing,
        &secondary_timeout_missing,
        &secondary_timeout_missing,
        nullptr,
    };
    const auto secondary_timeout_missing_services = make_services(secondary_timeout_missing);
    FUN_004014C0_SetServices(&secondary_timeout_missing_services);
    assert(FUN_004014C0());
    FUN_004014C0_SetServices(nullptr);

    assert(secondary_timeout_missing.secondary_ready_index == 6U);
    assert(secondary_timeout_missing.secondary_object_index == 7U);
    assert(secondary_timeout_missing.sleeps.size() == 5U);
    assert(secondary_timeout_missing.secondary_logs.size() == 1U);
    assert(secondary_timeout_missing.secondary_logs[0][1] == 0x0137A6E0U);

    Probe final_not_ready{};
    final_not_ready.primary_ready_results = {true};
    final_not_ready.secondary_ready_results = {true, false};
    final_not_ready.secondary_objects = {
        &final_not_ready,
        &final_not_ready,
        &final_not_ready,
    };
    const auto final_not_ready_services = make_services(final_not_ready);
    FUN_004014C0_SetServices(&final_not_ready_services);
    assert(FUN_004014C0());
    FUN_004014C0_SetServices(nullptr);

    assert(final_not_ready.secondary_object_index == 3U);
    assert(final_not_ready.secondary_logs.size() == 1U);
    assert(final_not_ready.secondary_logs[0][1] == 0x0137A6FCU);
}
