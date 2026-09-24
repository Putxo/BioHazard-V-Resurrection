#include "re5/recovered/fun_00407d90.hpp"

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
    auto& state = *static_cast<StringAllocatorState*>(context);
    ++state.free_calls;
}

struct BlockStorage {
    alignas(16) std::array<std::byte, 128> bytes{};

    re5::recovered::FUN_00402360_Block* set(
        std::string_view value,
        std::uint32_t refs = 1U) noexcept {
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

void test_fun_00407d90() {
    using namespace re5::recovered;

    BlockStorage source_storage{};
    auto* source_block = source_storage.set("base", 3U);
    FUN_00402360_String source{source_block};

    FUN_00402360_SetServices(nullptr);
    FUN_00407D10_SetServices(nullptr);

    // Portable-host constructor guard inherited from FUN_00402360:
    // destination is pre-cleared and no append temporary can be allocated.
    FUN_00402360_String guarded{reinterpret_cast<void*>(1)};
    assert(FUN_00407D90(source, guarded, "+tail") == &guarded);
    assert(guarded.pointer == nullptr);
    assert(source_block->ref_count == 3U);

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
    FUN_00407D10_Services append_services{
        &concat,
        concatenate,
    };
    FUN_00407D10_SetServices(&append_services);

    // Native source-null branch: 0x0137AA5C is an empty C string, so the
    // constructed destination starts empty and append adopts the suffix.
    FUN_00402360_String null_source{nullptr};
    FUN_00402360_String suffix_only{nullptr};
    assert(FUN_00407D90(null_source, suffix_only, "tail") == &suffix_only);
    auto* suffix_only_block =
        static_cast<FUN_00402360_Block*>(suffix_only.pointer);
    assert(suffix_only_block != nullptr);
    assert(suffix_only_block->ref_count == 1U);
    assert(std::strcmp(suffix_only_block->data, "tail") == 0);

    // Native delegated early exit: null suffix produces a null temporary in
    // FUN_00407D10, leaving the freshly copied source as the result.
    FUN_00402360_String copied{nullptr};
    assert(FUN_00407D90(source, copied, nullptr) == &copied);
    auto* copied_block =
        static_cast<FUN_00402360_Block*>(copied.pointer);
    assert(copied_block != nullptr);
    assert(copied_block->ref_count == 1U);
    assert(std::strcmp(copied_block->data, "base") == 0);

    // Empty suffix follows the same D10 native early-return path.
    FUN_00402360_String copied_empty_suffix{nullptr};
    assert(FUN_00407D90(
               source,
               copied_empty_suffix,
               "") == &copied_empty_suffix);
    assert(std::strcmp(
               static_cast<FUN_00402360_Block*>(
                   copied_empty_suffix.pointer)->data,
               "base") == 0);

    // Normal path: copy source, append suffix through D10, release both
    // temporary constructor blocks, and return the destination address.
    const unsigned frees_before_concat = strings.free_calls;
    const unsigned calls_before_concat = concat.calls;
    FUN_00402360_String combined{nullptr};
    assert(FUN_00407D90(source, combined, "+tail") == &combined);
    assert(concat.calls == calls_before_concat + 1U);
    auto* combined_block =
        static_cast<FUN_00402360_Block*>(combined.pointer);
    assert(combined_block != nullptr);
    assert(combined_block->ref_count == 1U);
    assert(combined_block->length == 9U);
    assert(std::strcmp(combined_block->data, "base+tail") == 0);
    assert(strings.free_calls == frees_before_concat + 2U);
    assert(source_block->ref_count == 3U);

    // Harness-only D10 missing-service guard: source copy succeeds, suffix
    // temporary is released, and the copied source remains installed.
    FUN_00407D10_SetServices(nullptr);
    const unsigned frees_before_guard = strings.free_calls;
    FUN_00402360_String append_guard{nullptr};
    assert(FUN_00407D90(source, append_guard, "+guard") == &append_guard);
    assert(std::strcmp(
               static_cast<FUN_00402360_Block*>(append_guard.pointer)->data,
               "base") == 0);
    assert(strings.free_calls == frees_before_guard + 1U);

    // Harness-only concatenate failure guard follows the same preservation
    // behavior after the source copy has been constructed.
    concat.return_null = true;
    FUN_00407D10_SetServices(&append_services);
    FUN_00402360_String concat_failure{nullptr};
    assert(FUN_00407D90(
               source,
               concat_failure,
               "+fail") == &concat_failure);
    assert(std::strcmp(
               static_cast<FUN_00402360_Block*>(
                   concat_failure.pointer)->data,
               "base") == 0);

    FUN_00407D10_SetServices(nullptr);
    FUN_00402360_SetServices(nullptr);
}
