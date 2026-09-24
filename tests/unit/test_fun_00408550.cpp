#include "re5/recovered/fun_00408550.hpp"

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

struct DuplicateState {
    unsigned calls = 0U;
    std::uint32_t source_message = 0U;
    std::uint32_t result = 0U;
    std::uint32_t vtable_seen = 0U;
    std::uint32_t owns_seen = 0U;
};

std::uint32_t duplicate_base_message(
    void* context,
    const re5::recovered::FUN_00408550_Object& destination,
    std::uint32_t source_message) noexcept {
    auto& state = *static_cast<DuplicateState*>(context);
    ++state.calls;
    state.source_message = source_message;
    state.vtable_seen = destination.vtable;
    state.owns_seen = destination.base_owns_message;
    return state.result;
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

void test_fun_00408550() {
    using namespace re5::recovered;

    FUN_00408550_SetServices(nullptr);
    FUN_004082E0_SetServices(nullptr);

    // Non-owning std::exception state is shallow-copied exactly, followed by
    // an inline full copy of the embedded logic_error message.
    FUN_00408550_Object source{};
    source.vtable = 0xDEADBEEFU;
    source.base_message = 0x12345678U;
    source.base_owns_message = 0U;
    source.message.allocator_state = 0xAABBCCDDU;
    source.message.capacity = 0x0FU;
    source.message.length = 5U;
    std::strcpy(inline_data(source.message), "hello");

    FUN_00408550_Object object{};
    object.message.allocator_state = 0x11223344U;
    assert(FUN_00408550(object, source) == &object);
    assert(object.vtable == FUN_00408550_LOGIC_ERROR_VTABLE);
    assert(object.base_message == 0x12345678U);
    assert(object.base_owns_message == 0U);
    assert(object.message.allocator_state == 0x11223344U);
    assert(object.message.capacity == 0x0FU);
    assert(object.message.length == 5U);
    assert(std::strcmp(inline_data(object.message), "hello") == 0);
    assert(source.vtable == 0xDEADBEEFU);
    assert(source.base_message == 0x12345678U);
    assert(source.base_owns_message == 0U);

    // Owned non-null base message is duplicated. The callback observes the
    // exact intermediate std::exception vtable and copied ownership flag,
    // proving native base-copy ordering before the logic_error overwrite.
    DuplicateState duplicate{};
    duplicate.result = 0xCAFEBABEU;
    FUN_00408550_Services services{
        &duplicate,
        duplicate_base_message,
    };
    FUN_00408550_SetServices(&services);

    source.base_message = 0x01020304U;
    source.base_owns_message = 1U;
    FUN_00408550_Object owned{};
    owned.message.allocator_state = 0x55667788U;
    assert(FUN_00408550(owned, source) == &owned);
    assert(duplicate.calls == 1U);
    assert(duplicate.source_message == 0x01020304U);
    assert(duplicate.vtable_seen == FUN_00408550_EXCEPTION_VTABLE);
    assert(duplicate.owns_seen == 1U);
    assert(owned.base_message == 0xCAFEBABEU);
    assert(owned.base_owns_message == 1U);
    assert(owned.vtable == FUN_00408550_LOGIC_ERROR_VTABLE);
    assert(owned.message.allocator_state == 0x55667788U);
    assert(std::strcmp(inline_data(owned.message), "hello") == 0);

    // Native base-copy null-message branch bypasses duplication even when the
    // ownership flag is set.
    duplicate.calls = 0U;
    source.base_message = 0U;
    source.base_owns_message = 1U;
    FUN_00408550_Object null_owned{};
    assert(FUN_00408550(null_owned, source) == &null_owned);
    assert(duplicate.calls == 0U);
    assert(null_owned.base_message == 0U);
    assert(null_owned.base_owns_message == 1U);
    assert(null_owned.vtable == FUN_00408550_LOGIC_ERROR_VTABLE);

    // Host-only missing-duplication seam. Native allocation/copy helpers are
    // always linked; the portable guard leaves the base pointer null but does
    // not invent an early return: the derived vtable and SSO copy still run.
    FUN_00408550_SetServices(nullptr);
    source.base_message = 0x0BADF00DU;
    source.base_owns_message = 1U;
    FUN_00408550_Object missing_duplicate{};
    assert(FUN_00408550(missing_duplicate, source) == &missing_duplicate);
    assert(missing_duplicate.base_message == 0U);
    assert(missing_duplicate.base_owns_message == 1U);
    assert(missing_duplicate.vtable == FUN_00408550_LOGIC_ERROR_VTABLE);
    assert(missing_duplicate.message.length == 5U);
    assert(std::strcmp(
               inline_data(missing_duplicate.message),
               "hello") == 0);

    // A native allocation failure has the same observable base-message result:
    // duplication returns null, construction continues, and the SSO message
    // is still copied.
    duplicate.calls = 0U;
    duplicate.result = 0U;
    services.context = &duplicate;
    FUN_00408550_SetServices(&services);
    FUN_00408550_Object duplicate_failure{};
    assert(FUN_00408550(duplicate_failure, source) == &duplicate_failure);
    assert(duplicate.calls == 1U);
    assert(duplicate_failure.base_message == 0U);
    assert(duplicate_failure.base_owns_message == 1U);
    assert(duplicate_failure.vtable == FUN_00408550_LOGIC_ERROR_VTABLE);
    assert(std::strcmp(
               inline_data(duplicate_failure.message),
               "hello") == 0);

    // Delegated 82E0 zero-length early exit must not resolve source heap
    // storage. An intentionally invalid pointer therefore remains safe.
    FUN_00408550_Object empty_source{};
    empty_source.base_message = 0x99U;
    empty_source.base_owns_message = 0U;
    empty_source.message.capacity = 0x10U;
    empty_source.message.length = 0U;
    set_heap_pointer(
        empty_source.message,
        reinterpret_cast<char*>(
            static_cast<std::uintptr_t>(1U)));

    FUN_00408550_Object empty{};
    empty.message.allocator_state = 0xFACEB00CU;
    assert(FUN_00408550(empty, empty_source) == &empty);
    assert(empty.base_message == 0x99U);
    assert(empty.message.allocator_state == 0xFACEB00CU);
    assert(empty.message.length == 0U);
    assert(empty.message.capacity == 0x0FU);
    assert(inline_data(empty.message)[0] == '\0');

    // Host-only inherited 82E0 growth guard: the native 8100 dependency is
    // linked, while the portable test seam suppresses an oversized copy when
    // no growth service is installed.
    char large_text[64] = "0123456789abcdefghij";
    FUN_00408550_Object large_source{};
    large_source.message.capacity = 0x40U;
    large_source.message.length = 20U;
    set_heap_pointer(large_source.message, large_text);

    FUN_004082E0_SetServices(nullptr);
    FUN_00408550_Object guarded{};
    assert(FUN_00408550(guarded, large_source) == &guarded);
    assert(guarded.vtable == FUN_00408550_LOGIC_ERROR_VTABLE);
    assert(guarded.message.length == 0U);
    assert(guarded.message.capacity == 0x0FU);
    assert(inline_data(guarded.message)[0] == '\0');

    // Normal growth-required SSO copy through the existing 82E0 dependency.
    GrowState grow_state{};
    FUN_004082E0_Services grow_services{
        &grow_state,
        grow,
    };
    FUN_004082E0_SetServices(&grow_services);

    FUN_00408550_Object grown{};
    assert(FUN_00408550(grown, large_source) == &grown);
    assert(grow_state.calls == 1U);
    assert(grow_state.required_length == 20U);
    assert(grow_state.preserved_length == 0U);
    assert(grown.message.length == 20U);
    assert(grown.message.capacity == 0x40U);
    assert(std::strcmp(
               grow_state.storage.data(),
               "0123456789abcdefghij") == 0);

    FUN_004082E0_SetServices(nullptr);
    FUN_00408550_SetServices(nullptr);
}
