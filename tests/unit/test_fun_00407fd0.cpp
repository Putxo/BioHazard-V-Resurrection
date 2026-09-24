#include "re5/recovered/fun_00407fd0.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <cstring>
#include <string_view>

namespace {
struct StringAllocatorState {
    alignas(16) std::array<std::byte, 4096> storage{};
    std::size_t cursor = 0U;
    std::size_t requested_size = 0U;
    std::size_t requested_alignment = 0U;
    unsigned allocate_calls = 0U;
    unsigned free_calls = 0U;
    void* last_freed = nullptr;
    bool return_null = false;
};

void* allocate_string(
    void* context,
    std::size_t size,
    std::size_t alignment) noexcept {
    auto& state = *static_cast<StringAllocatorState*>(context);
    ++state.allocate_calls;
    state.requested_size = size;
    state.requested_alignment = alignment;

    if (state.return_null) {
        return nullptr;
    }

    const std::size_t mask = alignment - 1U;
    const std::size_t aligned = (state.cursor + mask) & ~mask;
    assert(aligned + size <= state.storage.size());

    void* result = state.storage.data() + aligned;
    state.cursor = aligned + size;
    return result;
}

void free_string(void* context, void* pointer) noexcept {
    auto& state = *static_cast<StringAllocatorState*>(context);
    ++state.free_calls;
    state.last_freed = pointer;
}

struct BlockStorage {
    alignas(16) std::array<std::byte, 128> bytes{};

    re5::recovered::FUN_00402360_Block* set(
        std::string_view value,
        std::uint32_t refs) noexcept {
        auto* block =
            reinterpret_cast<re5::recovered::FUN_00402360_Block*>(
                bytes.data());
        block->ref_count = refs;
        block->length = static_cast<std::uint32_t>(value.size());
        std::memcpy(block->data, value.data(), value.size());
        block->data[value.size()] = '\0';
        return block;
    }
};

struct ConcatenateState {
    alignas(16) std::array<std::byte, 512> storage{};
    unsigned calls = 0U;
    bool return_null = false;
    const re5::recovered::FUN_00402360_Block* last_left = nullptr;
    const re5::recovered::FUN_00402360_Block* last_right = nullptr;
};

re5::recovered::FUN_00402360_Block* concatenate(
    void* context,
    const re5::recovered::FUN_00402360_Block* left,
    const re5::recovered::FUN_00402360_Block* right) noexcept {
    auto& state = *static_cast<ConcatenateState*>(context);
    ++state.calls;
    state.last_left = left;
    state.last_right = right;

    if (state.return_null) {
        return nullptr;
    }

    auto* result =
        reinterpret_cast<re5::recovered::FUN_00402360_Block*>(
            state.storage.data());
    result->ref_count = 1U;
    result->length = left->length + right->length;
    std::memcpy(result->data, left->data, left->length);
    std::memcpy(
        result->data + left->length,
        right->data,
        right->length + 1U);
    return result;
}
} // namespace

void test_fun_00407fd0() {
    using namespace re5::recovered;

    BlockStorage untouched_storage{};
    auto* untouched_block = untouched_storage.set("old", 7U);
    FUN_00402360_String untouched{untouched_block};

    FUN_00407FD0_SetServices(nullptr);
    FUN_00402360_SetServices(nullptr);

    // Native temporary-null exits: null/empty suffix leaves destination alone.
    FUN_00407FD0(untouched, nullptr);
    assert(untouched.pointer == untouched_block);
    assert(untouched_block->ref_count == 7U);

    FUN_00407FD0(untouched, "");
    assert(untouched.pointer == untouched_block);
    assert(untouched_block->ref_count == 7U);

    // Inherited portable allocator guard from FUN_00402360.
    FUN_00407FD0(untouched, "suffix");
    assert(untouched.pointer == untouched_block);
    assert(untouched_block->ref_count == 7U);

    StringAllocatorState strings{};
    FUN_00402360_Services string_services{
        '\0',
        &strings,
        allocate_string,
        free_string,
        "",
    };
    FUN_00402360_SetServices(&string_services);

    // Empty destination: native increments the adopted temporary reference,
    // then releases the temporary. Net stored refcount is one.
    FUN_00402360_String empty{nullptr};
    FUN_00407FD0(empty, "Jill");
    auto* adopted =
        static_cast<FUN_00402360_Block*>(empty.pointer);
    assert(adopted != nullptr);
    assert(adopted->ref_count == 1U);
    assert(adopted->length == 4U);
    assert(std::strcmp(adopted->data, "Jill") == 0);
    assert(strings.requested_size == 4U + 0x0CU);
    assert(strings.requested_alignment == 0x10U);

    // Host-only missing-concatenate guard releases the constructed temporary
    // but preserves the existing destination.
    const unsigned frees_before_missing = strings.free_calls;
    FUN_00407FD0(untouched, "+x");
    assert(untouched.pointer == untouched_block);
    assert(untouched_block->ref_count == 7U);
    assert(strings.free_calls == frees_before_missing + 1U);

    ConcatenateState concat{};
    FUN_00407FD0_Services services{
        &concat,
        concatenate,
    };
    FUN_00407FD0_SetServices(&services);

    // Shared old destination: decrement but do not free it, install combined
    // old+suffix block, then release the temporary suffix.
    BlockStorage shared_storage{};
    auto* shared_block = shared_storage.set("old", 2U);
    FUN_00402360_String shared{shared_block};
    const unsigned frees_before_shared = strings.free_calls;

    FUN_00407FD0(shared, "+new");

    assert(concat.calls == 1U);
    assert(concat.last_left == shared_block);
    assert(shared_block->ref_count == 1U);
    assert(strings.free_calls == frees_before_shared + 1U);
    auto* combined =
        static_cast<FUN_00402360_Block*>(shared.pointer);
    assert(combined->ref_count == 1U);
    assert(combined->length == 7U);
    assert(std::strcmp(combined->data, "old+new") == 0);

    // Unique old destination: old release reaches zero and frees before the
    // returned combined block is installed; temporary release is the second
    // free in this operation.
    BlockStorage unique_storage{};
    auto* unique_block = unique_storage.set("one", 1U);
    FUN_00402360_String unique{unique_block};
    const unsigned frees_before_unique = strings.free_calls;

    FUN_00407FD0(unique, "two");

    assert(unique_block->ref_count == 0U);
    assert(strings.free_calls == frees_before_unique + 2U);
    auto* unique_combined =
        static_cast<FUN_00402360_Block*>(unique.pointer);
    assert(unique_combined->ref_count == 1U);
    assert(unique_combined->length == 6U);
    assert(std::strcmp(unique_combined->data, "onetwo") == 0);

    // Host-only concatenate/allocation-failure guard: preserve old destination
    // and still destroy the constructed temporary.
    concat.return_null = true;
    BlockStorage failure_storage{};
    auto* failure_block = failure_storage.set("keep", 5U);
    FUN_00402360_String failure{failure_block};
    const unsigned calls_before_failure = concat.calls;
    const unsigned frees_before_failure = strings.free_calls;

    FUN_00407FD0(failure, "fail");

    assert(concat.calls == calls_before_failure + 1U);
    assert(failure.pointer == failure_block);
    assert(failure_block->ref_count == 5U);
    assert(strings.free_calls == frees_before_failure + 1U);

    // Portable temporary-allocation guard also leaves destination unchanged.
    strings.return_null = true;
    FUN_00407FD0(failure, "alloc-fail");
    assert(failure.pointer == failure_block);
    assert(failure_block->ref_count == 5U);
    strings.return_null = false;

    FUN_00407FD0_SetServices(nullptr);
    FUN_00402360_SetServices(nullptr);
}
