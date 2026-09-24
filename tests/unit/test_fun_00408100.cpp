#include "re5/recovered/fun_00408100.hpp"

#include "re5/recovered/fun_00407a90.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>

namespace {
struct AllocatorProbe {
    std::array<char, 256> first{};
    std::array<char, 256> second{};
    std::vector<std::uint32_t> sizes{};
    std::vector<std::uint32_t> alignments{};
    unsigned throws_remaining = 0U;
    int next_throw_value = 1;
};

void* allocate_aligned(
    void* context,
    std::uint32_t requested_size,
    std::uint32_t alignment) {
    auto& probe = *static_cast<AllocatorProbe*>(context);
    const std::size_t index = probe.sizes.size();
    probe.sizes.push_back(requested_size);
    probe.alignments.push_back(alignment);

    if (probe.throws_remaining != 0U) {
        --probe.throws_remaining;
        throw probe.next_throw_value++;
    }

    return index == 0U
        ? static_cast<void*>(probe.first.data())
        : static_cast<void*>(probe.second.data());
}

struct FreeProbe {
    unsigned calls = 0U;
    void* pointer = nullptr;
};

void free_aligned(void* context, void* pointer) noexcept {
    auto& probe = *static_cast<FreeProbe*>(context);
    ++probe.calls;
    probe.pointer = pointer;
}

void store_heap_pointer(
    re5::recovered::FUN_00408100_Object& object,
    char* pointer) noexcept {
    std::memcpy(object.storage.data(), &pointer, sizeof(pointer));
}

char* load_heap_pointer(
    re5::recovered::FUN_00408100_Object& object) noexcept {
    char* pointer = nullptr;
    std::memcpy(&pointer, object.storage.data(), sizeof(pointer));
    return pointer;
}
} // namespace

void test_fun_00408100() {
    using namespace re5::recovered;

    AllocatorProbe allocator{};
    FUN_00407A90_Services allocation_services{
        &allocator,
        allocate_aligned,
    };
    FUN_00407A90_SetServices(&allocation_services);

    FreeProbe freer{};
    FUN_00408100_Services services{&freer, free_aligned};
    FUN_00408100_SetServices(&services);

    FUN_00408100_Object inline_object{};
    std::memcpy(inline_object.storage.data(), "hello", 5U);
    inline_object.size = 5U;
    inline_object.capacity = 15U;

    FUN_00408100(inline_object, 16U, 5U);

    assert(inline_object.capacity == 31U);
    assert(inline_object.size == 5U);
    assert(allocator.sizes.size() == 1U);
    assert(allocator.sizes[0] == 32U);
    assert(allocator.alignments[0] == 0x10U);
    assert(std::memcmp(
               load_heap_pointer(inline_object),
               "hello",
               5U) == 0);
    assert(load_heap_pointer(inline_object)[5] == '\0');
    assert(freer.calls == 0U);

    FUN_00408100_Object heap_object{};
    std::array<char, 64> old_heap{};
    std::memcpy(old_heap.data(), "abcdefghij", 10U);
    store_heap_pointer(heap_object, old_heap.data());
    heap_object.size = 10U;
    heap_object.capacity = 63U;

    allocator = AllocatorProbe{};
    FUN_00407A90_SetServices(&allocation_services);
    freer = FreeProbe{};
    FUN_00408100_SetServices(&services);

    FUN_00408100(heap_object, 64U, 10U);

    assert(heap_object.capacity == 94U);
    assert(heap_object.size == 10U);
    assert(allocator.sizes.size() == 1U);
    assert(allocator.sizes[0] == 95U);
    assert(allocator.alignments[0] == 0x10U);
    assert(freer.calls == 1U);
    assert(freer.pointer == old_heap.data());
    assert(std::memcmp(
               load_heap_pointer(heap_object),
               "abcdefghij",
               10U) == 0);
    assert(load_heap_pointer(heap_object)[10] == '\0');

    FUN_00408100_Object zero_copy{};
    allocator = AllocatorProbe{};
    FUN_00407A90_SetServices(&allocation_services);

    FUN_00408100(zero_copy, 16U, 0U);

    assert(zero_copy.capacity == 31U);
    assert(zero_copy.size == 0U);
    assert(load_heap_pointer(zero_copy)[0] == '\0');

    FUN_00408100_Object retry{};
    std::memcpy(retry.storage.data(), "data", 4U);
    retry.size = 4U;
    retry.capacity = 15U;

    allocator = AllocatorProbe{};
    allocator.throws_remaining = 1U;
    FUN_00407A90_SetServices(&allocation_services);

    FUN_00408100(retry, 16U, 4U);

    assert(allocator.sizes.size() == 2U);
    assert(allocator.sizes[0] == 32U);
    assert(allocator.sizes[1] == 17U);
    assert(retry.capacity == 16U);
    assert(retry.size == 4U);
    assert(std::memcmp(
               load_heap_pointer(retry),
               "data",
               4U) == 0);
    assert(load_heap_pointer(retry)[4] == '\0');

    FUN_00408100_Object failed_retry{};
    std::array<char, 64> old_retry_heap{};
    std::memcpy(old_retry_heap.data(), "xyz", 3U);
    store_heap_pointer(failed_retry, old_retry_heap.data());
    failed_retry.size = 3U;
    failed_retry.capacity = 31U;

    allocator = AllocatorProbe{};
    allocator.throws_remaining = 2U;
    allocator.next_throw_value = 7;
    FUN_00407A90_SetServices(&allocation_services);
    freer = FreeProbe{};
    FUN_00408100_SetServices(&services);

    bool caught_second = false;
    try {
        FUN_00408100(failed_retry, 32U, 3U);
    } catch (int value) {
        caught_second = true;
        assert(value == 8);
    }

    assert(caught_second);
    assert(allocator.sizes.size() == 2U);
    assert(allocator.sizes[0] == 48U);
    assert(allocator.sizes[1] == 33U);
    assert(freer.calls == 1U);
    assert(freer.pointer == old_retry_heap.data());
    assert(failed_retry.capacity == 15U);
    assert(failed_retry.size == 0U);
    assert(reinterpret_cast<const char*>(
               failed_retry.storage.data())[0] == '\0');

    FUN_00408100_Object missing_free{};
    std::array<char, 64> guarded_heap{};
    store_heap_pointer(missing_free, guarded_heap.data());
    missing_free.size = 2U;
    missing_free.capacity = 31U;

    allocator = AllocatorProbe{};
    FUN_00407A90_SetServices(&allocation_services);
    FUN_00408100_SetServices(nullptr);

    FUN_00408100(missing_free, 32U, 2U);

    assert(allocator.sizes.empty());
    assert(missing_free.capacity == 31U);
    assert(missing_free.size == 2U);
    assert(load_heap_pointer(missing_free) == guarded_heap.data());

    FUN_00408100_Object missing_allocator{};
    FUN_00407A90_SetServices(nullptr);
    FUN_00408100_SetServices(&services);

    FUN_00408100(missing_allocator, 16U, 0U);

    assert(missing_allocator.capacity == 15U);
    assert(missing_allocator.size == 0U);
    assert(reinterpret_cast<const char*>(
               missing_allocator.storage.data())[0] == '\0');

    FUN_00408100_SetServices(nullptr);
    FUN_00407A90_SetServices(nullptr);
}
