#include "re5/recovered/fun_00408070.hpp"

#include <cassert>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>

namespace {

char* inline_data(re5::recovered::FUN_00408070_String& value) noexcept {
    return reinterpret_cast<char*>(value.storage.data());
}

void set_heap_pointer(
    re5::recovered::FUN_00408070_String& value,
    char* pointer) noexcept {
    std::memset(value.storage.data(), 0, value.storage.size());
    std::memcpy(value.storage.data(), &pointer, sizeof(pointer));
}

} // namespace

void test_fun_00408070() {
    using namespace re5::recovered;

    // Inline/SSO path: remove two bytes from the middle and shift the tail.
    FUN_00408070_String inline_value{};
    inline_value.capacity = 0x0FU;
    inline_value.length = 6U;
    std::strcpy(inline_data(inline_value), "abcdef");

    assert(FUN_00408070(inline_value, 2U, 2U) == &inline_value);
    assert(inline_value.length == 4U);
    assert(std::strcmp(inline_data(inline_value), "abef") == 0);

    // Native zero-count early return. The payload and length stay untouched.
    const auto inline_before = inline_value;
    assert(FUN_00408070(inline_value, 4U, 0U) == &inline_value);
    assert(inline_value.length == inline_before.length);
    assert(std::memcmp(
               inline_value.storage.data(),
               inline_before.storage.data(),
               inline_value.storage.size()) == 0);

    // Count is clamped to the available suffix, including UINT32_MAX.
    assert(FUN_00408070(
               inline_value,
               2U,
               0xFFFFFFFFU) == &inline_value);
    assert(inline_value.length == 2U);
    assert(std::strcmp(inline_data(inline_value), "ab") == 0);

    // Native invalid-position guard calls the exact std::out_of_range path.
    // The object is untouched when the exception is raised.
    bool threw = false;
    try {
        (void)FUN_00408070(inline_value, 3U, 1U);
    } catch (const std::out_of_range& exception) {
        threw = true;
        assert(std::string(exception.what()) == "invalid string position");
    }
    assert(threw);
    assert(inline_value.length == 2U);
    assert(std::strcmp(inline_data(inline_value), "ab") == 0);

    // capacity == 0x10 selects the heap-pointer path, matching the native
    // unsigned threshold exactly.
    char heap_storage[64] = "0123456789abcdefXYZ";
    FUN_00408070_String heap_value{};
    heap_value.capacity = 0x20U;
    heap_value.length = 19U;
    set_heap_pointer(heap_value, heap_storage);

    assert(FUN_00408070(heap_value, 16U, 2U) == &heap_value);
    assert(heap_value.length == 17U);
    assert(std::strcmp(heap_storage, "0123456789abcdefZ") == 0);

    // Erasing exactly at end is a native early return. A deliberately invalid
    // heap pointer proves that no data pointer is resolved on this branch.
    FUN_00408070_String no_touch{};
    no_touch.capacity = 0x10U;
    no_touch.length = 0U;
    char* invalid_pointer =
        reinterpret_cast<char*>(static_cast<std::uintptr_t>(1U));
    set_heap_pointer(no_touch, invalid_pointer);

    assert(FUN_00408070(
               no_touch,
               0U,
               0xFFFFFFFFU) == &no_touch);
    assert(no_touch.length == 0U);
}
