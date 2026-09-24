#include "re5/recovered/fun_004083c0.hpp"

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
    bool report_success_without_capacity = false;
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

    if (state.report_success_without_capacity) {
        return true;
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

void test_fun_004083c0() {
    using namespace re5::recovered;

    FUN_004083C0_SetServices(nullptr);

    // Native internal-alias branch delegates to 82E0 using the exact offset.
    FUN_004082E0_String self{};
    self.capacity = 0x0FU;
    self.length = 6U;
    std::strcpy(inline_data(self), "abcdef");
    assert(FUN_004083C0(
               self,
               inline_data(self) + 2,
               3U) == &self);
    assert(self.length == 3U);
    assert(std::strcmp(inline_data(self), "cde") == 0);

    // Alias detection precedes the length-error check. UINT32_MAX therefore
    // reaches 82E0 and is clamped to the available self substring.
    FUN_004082E0_String alias_max{};
    alias_max.capacity = 0x0FU;
    alias_max.length = 6U;
    std::strcpy(inline_data(alias_max), "abcdef");
    assert(FUN_004083C0(
               alias_max,
               inline_data(alias_max) + 2,
               0xFFFFFFFFU) == &alias_max);
    assert(alias_max.length == 4U);
    assert(std::strcmp(inline_data(alias_max), "cdef") == 0);

    // External UINT32_MAX takes the native 0x0125132D length-error path and
    // leaves the destination untouched.
    FUN_004082E0_String too_long{};
    too_long.capacity = 0x0FU;
    too_long.length = 3U;
    std::strcpy(inline_data(too_long), "old");
    bool length_threw = false;
    try {
        (void)FUN_004083C0(
            too_long,
            "x",
            0xFFFFFFFFU);
    } catch (const std::length_error& exception) {
        length_threw = true;
        assert(std::string(exception.what()) == "string too long");
    }
    assert(length_threw);
    assert(too_long.length == 3U);
    assert(std::strcmp(inline_data(too_long), "old") == 0);

    // Normal external inline assignment.
    FUN_004082E0_String inline_target{};
    inline_target.capacity = 0x0FU;
    inline_target.length = 3U;
    std::strcpy(inline_data(inline_target), "old");
    assert(FUN_004083C0(
               inline_target,
               "hello",
               5U) == &inline_target);
    assert(inline_target.length == 5U);
    assert(std::strcmp(inline_data(inline_target), "hello") == 0);

    // Native zero-length early exit does not dereference source. A null source
    // is therefore safe when count is zero.
    inline_target.length = 5U;
    std::strcpy(inline_data(inline_target), "hello");
    assert(FUN_004083C0(
               inline_target,
               nullptr,
               0U) == &inline_target);
    assert(inline_target.length == 0U);
    assert(inline_data(inline_target)[0] == '\0');

    // A pointer exactly at data+length is outside the native half-open alias
    // interval. With count zero it follows the external empty-assignment path.
    FUN_004082E0_String end_pointer{};
    end_pointer.capacity = 0x0FU;
    end_pointer.length = 4U;
    std::strcpy(inline_data(end_pointer), "test");
    assert(FUN_004083C0(
               end_pointer,
               inline_data(end_pointer) + 4,
               0U) == &end_pointer);
    assert(end_pointer.length == 0U);
    assert(inline_data(end_pointer)[0] == '\0');

    // Existing heap destination with enough capacity is used directly.
    std::array<char, 64> heap_storage{};
    std::strcpy(heap_storage.data(), "old");
    FUN_004082E0_String heap_target{};
    heap_target.capacity = 0x3FU;
    heap_target.length = 3U;
    set_heap_pointer(heap_target, heap_storage.data());
    assert(FUN_004083C0(
               heap_target,
               "heap-value",
               10U) == &heap_target);
    assert(heap_target.length == 10U);
    assert(std::strcmp(heap_storage.data(), "heap-value") == 0);

    // Growth-required path forwards exactly required=count and
    // preserved=current length to the native 8100-shaped service.
    GrowState grow_state{};
    FUN_004083C0_Services services{
        &grow_state,
        grow,
    };
    FUN_004083C0_SetServices(&services);

    FUN_004082E0_String grow_target{};
    grow_target.capacity = 0x0FU;
    grow_target.length = 4U;
    std::strcpy(inline_data(grow_target), "keep");
    constexpr char large_source[] = "0123456789abcdefghi";
    assert(FUN_004083C0(
               grow_target,
               large_source,
               19U) == &grow_target);
    assert(grow_state.calls == 1U);
    assert(grow_state.required_length == 19U);
    assert(grow_state.preserved_length == 4U);
    assert(grow_target.capacity == 0x40U);
    assert(grow_target.length == 19U);
    assert(std::strcmp(
               grow_state.storage.data(),
               large_source) == 0);

    // Host-only missing-growth guard: native 8100 is always linked, but the
    // portable harness preserves the destination if its dependency is absent.
    FUN_004083C0_SetServices(nullptr);
    FUN_004082E0_String missing_growth{};
    missing_growth.capacity = 0x0FU;
    missing_growth.length = 4U;
    std::strcpy(inline_data(missing_growth), "keep");
    assert(FUN_004083C0(
               missing_growth,
               large_source,
               19U) == &missing_growth);
    assert(missing_growth.length == 4U);
    assert(std::strcmp(inline_data(missing_growth), "keep") == 0);

    // Host-only explicit growth failure follows the same no-copy guard.
    GrowState failed_growth{};
    failed_growth.fail = true;
    services.context = &failed_growth;
    FUN_004083C0_SetServices(&services);
    FUN_004082E0_String failed_target{};
    failed_target.capacity = 0x0FU;
    failed_target.length = 4U;
    std::strcpy(inline_data(failed_target), "keep");
    assert(FUN_004083C0(
               failed_target,
               large_source,
               19U) == &failed_target);
    assert(failed_growth.calls == 1U);
    assert(failed_target.length == 4U);
    assert(std::strcmp(inline_data(failed_target), "keep") == 0);

    // Host-only malformed growth service guard: a callback that says success
    // without installing enough capacity must not permit an overflow.
    GrowState insufficient_growth{};
    insufficient_growth.report_success_without_capacity = true;
    services.context = &insufficient_growth;
    FUN_004083C0_SetServices(&services);
    FUN_004082E0_String insufficient_target{};
    insufficient_target.capacity = 0x0FU;
    insufficient_target.length = 4U;
    std::strcpy(inline_data(insufficient_target), "keep");
    assert(FUN_004083C0(
               insufficient_target,
               large_source,
               19U) == &insufficient_target);
    assert(insufficient_growth.calls == 1U);
    assert(insufficient_target.length == 4U);
    assert(std::strcmp(
               inline_data(insufficient_target),
               "keep") == 0);

    FUN_004083C0_SetServices(nullptr);
}
