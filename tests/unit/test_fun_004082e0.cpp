#include "re5/recovered/fun_004082e0.hpp"

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>

namespace {
char* inline_data(
    re5::recovered::FUN_004082E0_String& value) noexcept {
    return reinterpret_cast<char*>(value.storage.data());
}

void set_heap_pointer(
    re5::recovered::FUN_004082E0_String& value,
    char* pointer) noexcept {
    std::memset(value.storage.data(), 0, value.storage.size());
    std::memcpy(value.storage.data(), &pointer, sizeof(pointer));
}

struct GrowState {
    std::array<char, 128> storage{};
    unsigned calls = 0U;
    std::uint32_t required_length = 0U;
    std::uint32_t preserved_length = 0U;
    bool fail = false;
};

bool grow(
    void* context,
    re5::recovered::FUN_004082E0_String& value,
    std::uint32_t required_length,
    std::uint32_t preserved_length) noexcept {
    auto& state = *static_cast<GrowState*>(context);
    ++state.calls;
    state.required_length = required_length;
    state.preserved_length = preserved_length;

    if (state.fail) {
        return false;
    }

    if (preserved_length != 0U && value.capacity < 0x10U) {
        std::memcpy(
            state.storage.data(),
            value.storage.data(),
            preserved_length);
    }
    state.storage[preserved_length] = '\0';
    set_heap_pointer(value, state.storage.data());
    value.capacity = 0x40U;
    return true;
}
} // namespace

void test_fun_004082e0() {
    using namespace re5::recovered;

    FUN_004082E0_SetServices(nullptr);

    // Normal inline source/destination assignment of a bounded substring.
    FUN_004082E0_String source{};
    source.capacity = 0x0FU;
    source.length = 6U;
    std::strcpy(inline_data(source), "abcdef");

    FUN_004082E0_String destination{};
    destination.capacity = 0x0FU;
    destination.length = 3U;
    std::strcpy(inline_data(destination), "old");

    assert(FUN_004082E0(
               destination,
               source,
               2U,
               3U) == &destination);
    assert(destination.length == 3U);
    assert(std::strcmp(inline_data(destination), "cde") == 0);

    // Native count clamp, including UINT32_MAX.
    assert(FUN_004082E0(
               destination,
               source,
               4U,
               0xFFFFFFFFU) == &destination);
    assert(destination.length == 2U);
    assert(std::strcmp(inline_data(destination), "ef") == 0);

    // Invalid-position path raises the exact MSVC message and mutates nothing.
    const FUN_004082E0_String before_invalid = destination;
    bool position_threw = false;
    try {
        (void)FUN_004082E0(destination, source, 7U, 1U);
    } catch (const std::out_of_range& exception) {
        position_threw = true;
        assert(
            std::string(exception.what()) ==
            "invalid string position");
    }
    assert(position_threw);
    assert(
        std::memcmp(
            &destination,
            &before_invalid,
            sizeof(destination)) == 0);

    // Native self-source branch: two erase operations retain only [2,5).
    FUN_004082E0_String self{};
    self.capacity = 0x0FU;
    self.length = 6U;
    std::strcpy(inline_data(self), "abcdef");
    assert(FUN_004082E0(self, self, 2U, 3U) == &self);
    assert(self.length == 3U);
    assert(std::strcmp(inline_data(self), "cde") == 0);

    // Zero-length distinct-source path does not resolve source storage.
    FUN_004082E0_String invalid_source{};
    invalid_source.capacity = 0x10U;
    invalid_source.length = 0U;
    set_heap_pointer(
        invalid_source,
        reinterpret_cast<char*>(
            static_cast<std::uintptr_t>(1U)));
    destination.length = 3U;
    std::strcpy(inline_data(destination), "old");
    assert(FUN_004082E0(
               destination,
               invalid_source,
               0U,
               0U) == &destination);
    assert(destination.length == 0U);
    assert(inline_data(destination)[0] == '\0');

    // Heap-source path plus opaque FUN_00408100 growth contract.
    char heap_source[64] = "0123456789abcdefghij";
    FUN_004082E0_String large_source{};
    large_source.capacity = 0x40U;
    large_source.length = 20U;
    set_heap_pointer(large_source, heap_source);

    GrowState grow_state{};
    FUN_004082E0_Services services{
        &grow_state,
        grow,
    };
    FUN_004082E0_SetServices(&services);

    FUN_004082E0_String small_destination{};
    small_destination.capacity = 0x0FU;
    small_destination.length = 3U;
    std::strcpy(inline_data(small_destination), "old");

    assert(FUN_004082E0(
               small_destination,
               large_source,
               1U,
               18U) == &small_destination);
    assert(grow_state.calls == 1U);
    assert(grow_state.required_length == 18U);
    assert(grow_state.preserved_length == 3U);
    assert(small_destination.length == 18U);
    assert(
        std::strcmp(
            grow_state.storage.data(),
            "123456789abcdefghi") == 0);

    // Host-only missing-grow guard: growth-required operation is suppressed
    // without touching the destination.
    FUN_004082E0_SetServices(nullptr);
    FUN_004082E0_String guarded{};
    guarded.capacity = 0x0FU;
    guarded.length = 4U;
    std::strcpy(inline_data(guarded), "keep");
    assert(FUN_004082E0(
               guarded,
               large_source,
               0U,
               20U) == &guarded);
    assert(guarded.length == 4U);
    assert(std::strcmp(inline_data(guarded), "keep") == 0);

    // Host-only grow-failure guard has the same no-mutation safety behavior.
    GrowState failing_grow{};
    failing_grow.fail = true;
    services.context = &failing_grow;
    FUN_004082E0_SetServices(&services);
    assert(FUN_004082E0(
               guarded,
               large_source,
               0U,
               20U) == &guarded);
    assert(failing_grow.calls == 1U);
    assert(guarded.length == 4U);
    assert(std::strcmp(inline_data(guarded), "keep") == 0);

    FUN_004082E0_SetServices(nullptr);
}
