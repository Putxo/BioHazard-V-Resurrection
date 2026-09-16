#include "re5/recovered/fun_00402480.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>

namespace {
struct AllocatorState {
    std::array<std::byte, 128> storage{};
    std::size_t requested_size = 0;
    std::size_t requested_alignment = 0;
    unsigned alloc_calls = 0;
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

void record_callback(void* element) noexcept {
    callback_order[callback_count++] = element;
}
} // namespace

void test_fun_00402480() {
    using namespace re5::recovered;

    assert(FUN_00402480(0x20U) == nullptr);
    assert(FUN_004024A0() == 0U);

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

    callback_count = 0;
    FUN_004024B0(nullptr, sizeof(values[0]), 4, record_callback);
    assert(callback_count == 0U);

    FUN_00402480_SetServices(nullptr);
}
