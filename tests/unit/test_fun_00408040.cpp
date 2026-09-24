#include "re5/recovered/fun_00408040.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string_view>

namespace {
struct StringAllocatorState {
    alignas(16) std::array<std::byte, 4096> storage{};
    std::size_t cursor = 0U;
    unsigned allocate_calls = 0U;
    unsigned free_calls = 0U;
    bool return_null = false;
};

void* allocate_string(
    void* context,
    std::size_t size,
    std::size_t alignment) noexcept {
    auto& state = *static_cast<StringAllocatorState*>(context);
    ++state.allocate_calls;
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

void free_string(void* context, void*) noexcept {
    ++static_cast<StringAllocatorState*>(context)->free_calls;
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
};

re5::recovered::FUN_00402360_Block* concatenate(
    void* context,
    const re5::recovered::FUN_00402360_Block* left,
    const re5::recovered::FUN_00402360_Block* right) noexcept {
    auto& state = *static_cast<ConcatenateState*>(context);
    ++state.calls;
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

void test_fun_00408040() {
    using namespace re5::recovered;

    StringAllocatorState strings{};
    FUN_00402360_Services string_services{
        '\0',
        &strings,
        allocate_string,
        free_string,
        "",
    };
    FUN_00402360_SetServices(&string_services);

    ConcatenateState concat{};
    FUN_00407FD0_Services append_services{
        &concat,
        concatenate,
    };
    FUN_00407FD0_SetServices(&append_services);

    // Native source-null branch: destination is overwritten with null, the
    // add-ref is skipped, and FD0 still receives the null suffix.
    FUN_00402360_String null_source{nullptr};
    FUN_00402360_String null_destination{
        reinterpret_cast<void*>(0x1234)};
    assert(FUN_00408040(
               null_source,
               null_destination,
               nullptr) == &null_destination);
    assert(null_destination.pointer == nullptr);
    assert(concat.calls == 0U);

    // Native delegated early exit: null suffix makes FD0 construct a null
    // temporary. The copied source remains installed with one extra ref.
    BlockStorage null_suffix_storage{};
    auto* null_suffix_block = null_suffix_storage.set("base", 3U);
    FUN_00402360_String null_suffix_source{null_suffix_block};
    FUN_00402360_String null_suffix_destination{
        reinterpret_cast<void*>(0x5678)};
    assert(FUN_00408040(
               null_suffix_source,
               null_suffix_destination,
               nullptr) == &null_suffix_destination);
    assert(null_suffix_destination.pointer == null_suffix_block);
    assert(null_suffix_block->ref_count == 4U);
    assert(concat.calls == 0U);

    // Empty suffix follows the same FD0 native early-return path.
    BlockStorage empty_suffix_storage{};
    auto* empty_suffix_block = empty_suffix_storage.set("base", 9U);
    FUN_00402360_String empty_suffix_source{empty_suffix_block};
    FUN_00402360_String empty_suffix_destination{nullptr};
    assert(FUN_00408040(
               empty_suffix_source,
               empty_suffix_destination,
               "") == &empty_suffix_destination);
    assert(empty_suffix_destination.pointer == empty_suffix_block);
    assert(empty_suffix_block->ref_count == 10U);

    // Normal path: target add-ref happens first; FD0 concatenates and releases
    // the copied destination reference, producing a net-zero refcount change
    // on the original source and a refcount-one result.
    BlockStorage normal_storage{};
    auto* normal_block = normal_storage.set("base", 5U);
    FUN_00402360_String normal_source{normal_block};
    FUN_00402360_String normal_destination{nullptr};
    const unsigned calls_before = concat.calls;
    const unsigned frees_before = strings.free_calls;
    assert(FUN_00408040(
               normal_source,
               normal_destination,
               "+tail") == &normal_destination);
    assert(concat.calls == calls_before + 1U);
    assert(normal_block->ref_count == 5U);
    auto* combined =
        static_cast<FUN_00402360_Block*>(normal_destination.pointer);
    assert(combined != nullptr);
    assert(combined->ref_count == 1U);
    assert(combined->length == 9U);
    assert(std::strcmp(combined->data, "base+tail") == 0);
    assert(strings.free_calls == frees_before + 1U);

    // Null source plus real suffix enters FD0 with an empty destination; FD0
    // adopts its temporary and balances that temporary back to refcount one.
    FUN_00402360_String suffix_only{nullptr};
    assert(FUN_00408040(
               null_source,
               suffix_only,
               "solo") == &suffix_only);
    auto* suffix_only_block =
        static_cast<FUN_00402360_Block*>(suffix_only.pointer);
    assert(suffix_only_block != nullptr);
    assert(suffix_only_block->ref_count == 1U);
    assert(std::strcmp(suffix_only_block->data, "solo") == 0);

    // Harness-only FD0 service guard: after this wrapper has copied/add-ref'd
    // the source, FD0 releases only its temporary and preserves destination.
    BlockStorage guard_storage{};
    auto* guard_block = guard_storage.set("guard", 2U);
    FUN_00402360_String guard_source{guard_block};
    FUN_00402360_String guard_destination{nullptr};
    FUN_00407FD0_SetServices(nullptr);
    assert(FUN_00408040(
               guard_source,
               guard_destination,
               "+x") == &guard_destination);
    assert(guard_destination.pointer == guard_block);
    assert(guard_block->ref_count == 3U);

    // Harness-only concatenate failure follows the same preservation contract.
    concat.return_null = true;
    FUN_00407FD0_SetServices(&append_services);
    BlockStorage concat_fail_storage{};
    auto* concat_fail_block = concat_fail_storage.set("fail", 4U);
    FUN_00402360_String concat_fail_source{concat_fail_block};
    FUN_00402360_String concat_fail_destination{nullptr};
    assert(FUN_00408040(
               concat_fail_source,
               concat_fail_destination,
               "+x") == &concat_fail_destination);
    assert(concat_fail_destination.pointer == concat_fail_block);
    assert(concat_fail_block->ref_count == 5U);
    concat.return_null = false;

    // Harness-only temporary allocation failure: FD0 sees a null temporary and
    // returns immediately, again leaving the copied source/add-ref intact.
    strings.return_null = true;
    BlockStorage alloc_fail_storage{};
    auto* alloc_fail_block = alloc_fail_storage.set("alloc", 6U);
    FUN_00402360_String alloc_fail_source{alloc_fail_block};
    FUN_00402360_String alloc_fail_destination{nullptr};
    assert(FUN_00408040(
               alloc_fail_source,
               alloc_fail_destination,
               "+x") == &alloc_fail_destination);
    assert(alloc_fail_destination.pointer == alloc_fail_block);
    assert(alloc_fail_block->ref_count == 7U);

    FUN_00407FD0_SetServices(nullptr);
    FUN_00402360_SetServices(nullptr);
}
