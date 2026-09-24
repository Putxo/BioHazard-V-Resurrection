#include "re5/recovered/fun_00407c60.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <cstring>

namespace {
struct ScratchState {
    alignas(16) std::array<char, 0x400> storage{};
    std::size_t requested_size = 0U;
    std::size_t requested_alignment = 0U;
    unsigned allocate_calls = 0U;
    unsigned free_calls = 0U;
    bool return_null = false;
};

void* allocate_scratch(
    void* context,
    std::size_t size,
    std::size_t alignment) noexcept {
    auto& state = *static_cast<ScratchState*>(context);
    ++state.allocate_calls;
    state.requested_size = size;
    state.requested_alignment = alignment;
    return state.return_null ? nullptr : state.storage.data();
}

void free_scratch(void* context, void* pointer) noexcept {
    auto& state = *static_cast<ScratchState*>(context);
    assert(pointer == state.storage.data());
    ++state.free_calls;
}

struct StringAllocatorState {
    alignas(16) std::array<std::byte, 4096> storage{};
    std::size_t cursor = 0U;
    unsigned allocate_calls = 0U;
    unsigned free_calls = 0U;
    void* last_freed = nullptr;
};

void* allocate_string(
    void* context,
    std::size_t size,
    std::size_t alignment) noexcept {
    auto& state = *static_cast<StringAllocatorState*>(context);
    ++state.allocate_calls;

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
} // namespace

void test_fun_00407c60() {
    using namespace re5::recovered;

    FUN_00402360_Block untouched_block{};
    untouched_block.ref_count = 7U;
    FUN_00402360_String untouched{&untouched_block};

    // Harness-only guard: no scratch allocator service leaves the destination
    // completely untouched.
    FUN_00407C60_SetServices(nullptr);
    assert(FUN_00407C60(untouched, "%d", 1) == &untouched);
    assert(untouched.pointer == &untouched_block);
    assert(untouched_block.ref_count == 7U);

    ScratchState scratch{};
    FUN_00407C60_Services scratch_services{
        &scratch,
        allocate_scratch,
        free_scratch,
    };
    FUN_00407C60_SetServices(&scratch_services);

    // Harness-only guard: a failed temporary allocation returns before
    // formatting or releasing the destination.
    scratch.return_null = true;
    assert(FUN_00407C60(untouched, "%d", 2) == &untouched);
    assert(untouched.pointer == &untouched_block);
    assert(untouched_block.ref_count == 7U);
    assert(scratch.allocate_calls == 1U);
    assert(scratch.free_calls == 0U);
    scratch.return_null = false;

    StringAllocatorState strings{};
    FUN_00402360_Services string_services{
        '\0',
        &strings,
        allocate_string,
        free_string,
        "",
    };
    FUN_00402360_SetServices(&string_services);

    // Native normal path: fixed 0x400-byte / 0x10-aligned scratch buffer,
    // variadic formatting, fresh refcounted string construction, scratch free.
    FUN_00402360_String formatted{nullptr};
    assert(FUN_00407C60(
               formatted,
               "%s\\stage\\s%03d",
               "coop",
               7) == &formatted);
    assert(scratch.allocate_calls == 2U);
    assert(scratch.free_calls == 1U);
    assert(scratch.requested_size == 0x400U);
    assert(scratch.requested_alignment == 0x10U);
    assert(formatted.pointer != nullptr);
    const auto* formatted_block =
        static_cast<const FUN_00402360_Block*>(formatted.pointer);
    assert(formatted_block->ref_count == 1U);
    assert(std::strcmp(formatted_block->data, "coop\\stage\\s007") == 0);

    // Native existing-shared-string branch: decrement only; do not free when
    // the old reference count remains nonzero.
    FUN_00402360_Block shared_old{};
    shared_old.ref_count = 2U;
    FUN_00402360_String shared_destination{&shared_old};
    const unsigned frees_before_shared = strings.free_calls;
    assert(FUN_00407C60(shared_destination, "value=%d", 11) ==
           &shared_destination);
    assert(shared_old.ref_count == 1U);
    assert(strings.free_calls == frees_before_shared);
    assert(shared_destination.pointer != &shared_old);
    const auto* shared_new =
        static_cast<const FUN_00402360_Block*>(shared_destination.pointer);
    assert(std::strcmp(shared_new->data, "value=11") == 0);

    // Native existing-unique-string branch: decrement to zero and free through
    // the string allocator before constructing the replacement.
    FUN_00402360_Block unique_old{};
    unique_old.ref_count = 1U;
    FUN_00402360_String unique_destination{&unique_old};
    const unsigned frees_before_unique = strings.free_calls;
    assert(FUN_00407C60(unique_destination, "%s", "replacement") ==
           &unique_destination);
    assert(unique_old.ref_count == 0U);
    assert(strings.free_calls == frees_before_unique + 1U);
    assert(strings.last_freed == &unique_old);
    const auto* unique_new =
        static_cast<const FUN_00402360_Block*>(unique_destination.pointer);
    assert(std::strcmp(unique_new->data, "replacement") == 0);

    assert(scratch.allocate_calls == 4U);
    assert(scratch.free_calls == 3U);

    FUN_00407C60_SetServices(nullptr);
    FUN_00402360_SetServices(nullptr);
}
