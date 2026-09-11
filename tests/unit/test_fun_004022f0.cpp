#include "re5/recovered/fun_004022f0.hpp"

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
} // namespace

void test_fun_004022f0() {
    using namespace re5::recovered;

    AllocatorState allocator{};
    FUN_00402360_Services services{'X', &allocator, allocate_aligned, free_aligned};
    FUN_00402360_SetServices(&services);

    FUN_004022F0_Object text_object{nullptr, "Chris"};
    assert(FUN_004022F0(&text_object, "Chris") == 0);
    assert(FUN_004022F0(&text_object, "Chria") > 0);
    assert(FUN_004022F0(&text_object, "Zhris") < 0);
    assert(FUN_004022F0(nullptr, "Xanything") == 0);
    assert(FUN_004022F0(nullptr, "A") < 0);
    assert(FUN_004022F0(nullptr, "Z") > 0);

    std::uint32_t value = 0x12345678U;
    FUN_00402350_Object value_object{nullptr, &value};
    assert(FUN_00402350(nullptr) == 0U);
    assert(FUN_00402350(&value_object) == 0x12345678U);

    FUN_00402360_String empty{reinterpret_cast<void*>(1)};
    assert(FUN_00402360(empty, nullptr) == &empty);
    assert(empty.pointer == nullptr);
    assert(FUN_00402360(empty, "") == &empty);
    assert(empty.pointer == nullptr);
    assert(allocator.alloc_calls == 0U);

    FUN_00402360_String text{};
    assert(FUN_00402360(text, "Sheva") == &text);
    assert(allocator.alloc_calls == 1U);
    assert(allocator.requested_size == 5U + 0x0CU);
    assert(allocator.requested_alignment == 0x10U);
    auto* block = static_cast<FUN_00402360_Block*>(text.pointer);
    assert(block->ref_count == 1U);
    assert(block->length == 5U);
    assert(std::strcmp(block->data, "Sheva") == 0);

    block->ref_count = 2U;
    FUN_004023E0(text);
    assert(block->ref_count == 1U);
    assert(allocator.free_calls == 0U);
    FUN_004023E0(text);
    assert(block->ref_count == 0U);
    assert(allocator.free_calls == 1U);
    assert(allocator.freed_pointer == text.pointer);

    FUN_00402360_String null_value{};
    FUN_004023E0(null_value);
    assert(allocator.free_calls == 1U);

    FUN_00402360_SetServices(nullptr);
}
