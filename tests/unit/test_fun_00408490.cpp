#include "re5/recovered/fun_00408490.hpp"

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>

namespace {

char* inline_data(
    re5::recovered::FUN_004082E0_String& value) noexcept {
    return reinterpret_cast<char*>(value.storage.data());
}

const char* inline_data(
    const re5::recovered::FUN_004082E0_String& value) noexcept {
    return reinterpret_cast<const char*>(value.storage.data());
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

    set_heap_pointer(value, state.storage.data());
    value.capacity = 0x40U;
    return true;
}

} // namespace

void test_fun_00408490() {
    using namespace re5::recovered;

    FUN_004082E0_SetServices(nullptr);

    // Normal inline logic_error construction.
    FUN_004082E0_String source{};
    source.capacity = 0x0FU;
    source.length = 5U;
    std::strcpy(inline_data(source), "hello");

    FUN_00408490_Object object{};
    object.vtable = 1U;
    object.base_message = 2U;
    object.base_owns_message = 3U;
    object.message.allocator_state = 0xAABBCCDDU;

    assert(FUN_00408490(object, source) == &object);
    assert(object.vtable == 0x01541F54U);
    assert(object.base_message == 0U);
    assert(object.base_owns_message == 0U);
    assert(object.message.allocator_state == 0xAABBCCDDU);
    assert(object.message.length == 5U);
    assert(object.message.capacity == 0x0FU);
    assert(std::strcmp(inline_data(object.message), "hello") == 0);
    assert(source.length == 5U);
    assert(std::strcmp(inline_data(source), "hello") == 0);

    // Delegated 82E0 zero-length early exit does not resolve source storage.
    FUN_004082E0_String empty_source{};
    empty_source.capacity = 0x10U;
    empty_source.length = 0U;
    char* invalid_pointer =
        reinterpret_cast<char*>(static_cast<std::uintptr_t>(1U));
    set_heap_pointer(empty_source, invalid_pointer);

    FUN_00408490_Object empty_object{};
    empty_object.message.allocator_state = 0x11223344U;
    assert(FUN_00408490(empty_object, empty_source) == &empty_object);
    assert(empty_object.vtable == 0x01541F54U);
    assert(empty_object.base_message == 0U);
    assert(empty_object.base_owns_message == 0U);
    assert(empty_object.message.allocator_state == 0x11223344U);
    assert(empty_object.message.length == 0U);
    assert(empty_object.message.capacity == 0x0FU);
    assert(inline_data(empty_object.message)[0] == '\0');

    // Host-only inherited growth guard: native code has 8100 linked, whereas
    // the portable 82E0 seam suppresses the copy if no growth service exists.
    char heap_source[64] = "0123456789abcdefghij";
    FUN_004082E0_String large_source{};
    large_source.capacity = 0x40U;
    large_source.length = 20U;
    set_heap_pointer(large_source, heap_source);

    FUN_00408490_Object guarded{};
    assert(FUN_00408490(guarded, large_source) == &guarded);
    assert(guarded.vtable == 0x01541F54U);
    assert(guarded.message.length == 0U);
    assert(guarded.message.capacity == 0x0FU);
    assert(inline_data(guarded.message)[0] == '\0');

    // Normal growth-required path through the already-recovered 82E0 seam.
    GrowState grow_state{};
    FUN_004082E0_Services services{
        &grow_state,
        grow,
    };
    FUN_004082E0_SetServices(&services);

    FUN_00408490_Object grown{};
    assert(FUN_00408490(grown, large_source) == &grown);
    assert(grow_state.calls == 1U);
    assert(grow_state.required_length == 20U);
    assert(grow_state.preserved_length == 0U);
    assert(grown.vtable == 0x01541F54U);
    assert(grown.base_message == 0U);
    assert(grown.base_owns_message == 0U);
    assert(grown.message.length == 20U);
    assert(grown.message.capacity == 0x40U);
    assert(std::strcmp(
               grow_state.storage.data(),
               "0123456789abcdefghij") == 0);

    FUN_004082E0_SetServices(nullptr);
}
