#include "re5/recovered/fun_00402420.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace {
struct AllocatorState {
    std::array<std::byte, 128> storage{};
    std::size_t requested_size = 0;
    std::size_t requested_alignment = 0;
    unsigned alloc_calls = 0;
    unsigned free_calls = 0;
    void* freed_pointer = nullptr;
};

std::array<void*, 8> callback_order{};
std::size_t callback_count = 0;

void* allocate_aligned(void* context, std::size_t size, std::size_t alignment) noexcept {
    auto& state = *static_cast<AllocatorState*>(context);
    ++state.alloc_calls;
    state.requested_size = size;
    state.requested_alignment = alignment;
    return state.storage.data();
}

void free_aligned(void* context, void* pointer) noexcept {
    auto& state = *static_cast<AllocatorState*>(context);
    ++state.free_calls;
    state.freed_pointer = pointer;
}

void record_callback(void* element) noexcept {
    callback_order.at(callback_count++) = element;
}
} // namespace

void test_fun_00402420() {
    using namespace re5::recovered;

    AllocatorState string_allocator{};
    FUN_00402360_Services string_services{'X', &string_allocator, allocate_aligned, free_aligned};
    FUN_00402360_SetServices(&string_services);

    FUN_00402360_String empty{};
    assert(std::strcmp(FUN_00402420(empty), "") == 0);

    FUN_00402360_String source{};
    assert(FUN_00402360(source, "Jill") == &source);
    assert(std::strcmp(FUN_00402420(source), "Jill") == 0);

    AllocatorState destination_allocator{};
    FUN_00402360_String destination{};
    auto* destination_block = reinterpret_cast<FUN_00402360_Block*>(destination_allocator.storage.data());
    destination_block->ref_count = 1U;
    destination_block->length = 3U;
    std::memcpy(destination_block->data, "old", 4U);
    destination.pointer = destination_block;

    FUN_00402360_Services destination_services{'X', &destination_allocator, allocate_aligned, free_aligned};
    FUN_00402360_SetServices(&destination_services);
    assert(FUN_00402430(destination, source) == &destination);
    assert(destination_allocator.free_calls == 1U);
    assert(destination_allocator.freed_pointer == destination_block);
    assert(destination_allocator.alloc_calls == 1U);
    assert(destination_allocator.requested_size == 4U + 0x0CU);
    assert(destination_allocator.requested_alignment == 0x10U);
    assert(std::strcmp(FUN_00402420(destination), "Jill") == 0);

    AllocatorState allocation_wrapper{};
    FUN_00402480_Services services{&allocation_wrapper, allocate_aligned, 0x016E201CU};
    FUN_00402480_SetServices(&services);
    assert(FUN_00402480(0x44U) == allocation_wrapper.storage.data());
    assert(allocation_wrapper.alloc_calls == 1U);
    assert(allocation_wrapper.requested_size == 0x44U);
    assert(allocation_wrapper.requested_alignment == 0x10U);
    assert(FUN_004024A0() == 0x016E201CU);

    std::array<std::uint32_t, 4> values{10U, 20U, 30U, 40U};
    callback_count = 0;
    FUN_004024B0(values.data(), sizeof(values[0]), 4, record_callback);
    assert(callback_count == 4U);
    assert(callback_order[0] == &values[3]);
    assert(callback_order[1] == &values[2]);
    assert(callback_order[2] == &values[1]);
    assert(callback_order[3] == &values[0]);

    callback_count = 0;
    FUN_004024B0(values.data(), sizeof(values[0]), 0, record_callback);
    assert(callback_count == 0U);

    FUN_00402360_SetServices(nullptr);
    FUN_00402480_SetServices(nullptr);
}
