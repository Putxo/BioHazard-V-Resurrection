#include "re5/recovered/fun_004084d0.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>

namespace {
struct Probe {
    re5::recovered::FUN_004084D0_Object* object{};
    unsigned free_calls{};
    unsigned release_calls{};
    void* freed_pointer{};
    std::uint32_t released_message{};
    std::vector<int> order{};
};

void free_aligned(
    void* context,
    void* pointer) noexcept {
    auto& probe = *static_cast<Probe*>(context);
    ++probe.free_calls;
    probe.freed_pointer = pointer;
    probe.order.push_back(1);

    assert(probe.object != nullptr);
    assert(
        probe.object->vtable ==
        re5::recovered::
            FUN_004084D0_LOGIC_ERROR_VTABLE);
    assert(probe.object->message.capacity >= 0x10U);
}

void release_base_message(
    void* context,
    std::uint32_t message) noexcept {
    auto& probe = *static_cast<Probe*>(context);
    ++probe.release_calls;
    probe.released_message = message;
    probe.order.push_back(2);

    assert(probe.object != nullptr);
    assert(
        probe.object->vtable ==
        re5::recovered::
            FUN_004084D0_EXCEPTION_VTABLE);
    assert(probe.object->message.capacity == 0x0FU);
    assert(probe.object->message.length == 0U);
    assert(
        probe.object->message.storage[0] ==
        std::byte{0});
}

void store_heap_pointer(
    re5::recovered::FUN_004082E0_String& value,
    char* pointer) noexcept {
    std::memcpy(
        value.storage.data(),
        &pointer,
        sizeof(pointer));
}
} // namespace

void test_fun_004084d0() {
    using namespace re5::recovered;

    // Native capacity<16 and base_owns_message==0 bypass paths. Neither
    // external cleanup is called, but there is no native early return:
    // SSO reset and std::exception base teardown still occur.
    FUN_004084D0_Object inline_object{};
    inline_object.vtable = 0xDEADBEEFU;
    inline_object.base_message = 0x01020304U;
    inline_object.base_owns_message = 0U;
    inline_object.message.allocator_state = 0xAABBCCDDU;
    inline_object.message.length = 3U;
    inline_object.message.capacity = 15U;
    inline_object.message.storage[0] = std::byte{'x'};

    Probe inline_probe{};
    inline_probe.object = &inline_object;
    FUN_004084D0_Services services{
        &inline_probe,
        free_aligned,
        release_base_message,
    };
    FUN_004084D0_SetServices(&services);

    FUN_004084D0(inline_object);

    assert(inline_probe.free_calls == 0U);
    assert(inline_probe.release_calls == 0U);
    assert(inline_probe.order.empty());
    assert(
        inline_object.vtable ==
        FUN_004084D0_EXCEPTION_VTABLE);
    assert(inline_object.message.capacity == 15U);
    assert(inline_object.message.length == 0U);
    assert(
        inline_object.message.storage[0] ==
        std::byte{0});
    assert(
        inline_object.message.allocator_state ==
        0xAABBCCDDU);
    assert(
        inline_object.base_message ==
        0x01020304U);
    assert(inline_object.base_owns_message == 0U);

    // Heap-backed std::logic_error with owned base message: free the
    // embedded SSO heap first, reset it, then release the std::exception
    // base message.
    FUN_004084D0_Object heap_object{};
    std::array<char, 64> heap_storage{};
    store_heap_pointer(
        heap_object.message,
        heap_storage.data());
    heap_object.vtable = 0x11111111U;
    heap_object.base_message = 0x12345678U;
    heap_object.base_owns_message = 1U;
    heap_object.message.allocator_state = 0x55667788U;
    heap_object.message.length = 8U;
    heap_object.message.capacity = 31U;

    Probe heap_probe{};
    heap_probe.object = &heap_object;
    services.context = &heap_probe;
    FUN_004084D0_SetServices(&services);

    FUN_004084D0(heap_object);

    assert(heap_probe.free_calls == 1U);
    assert(
        heap_probe.freed_pointer ==
        heap_storage.data());
    assert(heap_probe.release_calls == 1U);
    assert(
        heap_probe.released_message ==
        0x12345678U);
    assert(
        (heap_probe.order ==
         std::vector<int>{1, 2}));
    assert(
        heap_object.vtable ==
        FUN_004084D0_EXCEPTION_VTABLE);
    assert(heap_object.message.capacity == 15U);
    assert(heap_object.message.length == 0U);
    assert(
        heap_object.message.storage[0] ==
        std::byte{0});
    assert(
        heap_object.message.allocator_state ==
        0x55667788U);

    // Host-only missing-service guard: suppress opaque external callbacks
    // without inventing an early return or suppressing deterministic state.
    FUN_004084D0_Object no_services{};
    std::array<char, 32> guarded_heap{};
    store_heap_pointer(
        no_services.message,
        guarded_heap.data());
    no_services.base_message = 7U;
    no_services.base_owns_message = 1U;
    no_services.message.length = 2U;
    no_services.message.capacity = 31U;

    FUN_004084D0_SetServices(nullptr);
    FUN_004084D0(no_services);

    assert(
        no_services.vtable ==
        FUN_004084D0_EXCEPTION_VTABLE);
    assert(no_services.message.capacity == 15U);
    assert(no_services.message.length == 0U);
    assert(
        no_services.message.storage[0] ==
        std::byte{0});

    // Missing heap-free binding must not suppress the later base-message
    // release.
    FUN_004084D0_Object missing_free{};
    std::array<char, 32> missing_free_heap{};
    store_heap_pointer(
        missing_free.message,
        missing_free_heap.data());
    missing_free.base_message = 0x55U;
    missing_free.base_owns_message = 1U;
    missing_free.message.length = 1U;
    missing_free.message.capacity = 31U;

    Probe missing_free_probe{};
    missing_free_probe.object = &missing_free;
    FUN_004084D0_Services missing_free_services{
        &missing_free_probe,
        nullptr,
        release_base_message,
    };
    FUN_004084D0_SetServices(
        &missing_free_services);
    FUN_004084D0(missing_free);

    assert(missing_free_probe.free_calls == 0U);
    assert(
        missing_free_probe.release_calls ==
        1U);
    assert(
        missing_free_probe.released_message ==
        0x55U);

    // Missing base-message release binding must not suppress SSO heap free.
    FUN_004084D0_Object missing_base{};
    std::array<char, 32> missing_base_heap{};
    store_heap_pointer(
        missing_base.message,
        missing_base_heap.data());
    missing_base.base_message = 0x66U;
    missing_base.base_owns_message = 1U;
    missing_base.message.length = 1U;
    missing_base.message.capacity = 31U;

    Probe missing_base_probe{};
    missing_base_probe.object = &missing_base;
    FUN_004084D0_Services missing_base_services{
        &missing_base_probe,
        free_aligned,
        nullptr,
    };
    FUN_004084D0_SetServices(
        &missing_base_services);
    FUN_004084D0(missing_base);

    assert(missing_base_probe.free_calls == 1U);
    assert(
        missing_base_probe.freed_pointer ==
        missing_base_heap.data());
    assert(
        missing_base_probe.release_calls ==
        0U);

    FUN_004084D0_SetServices(nullptr);
}
