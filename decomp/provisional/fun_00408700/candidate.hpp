#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace re5::provisional::fun_00408700 {

// A documented base-constructor PREFIX, not a claim about this derived
// object's total allocation size. No native binary was available in this run.
struct Prefix {
    std::uint32_t word_00;
    std::uint32_t word_04;
    std::uint32_t word_08;
    std::uint32_t word_0c;
    std::uint32_t word_10;
    std::uint32_t word_14; // Must remain untouched.
    std::uint32_t word_18; // Raw movss bits; do not reinterpret as a pointer.
};

static_assert(std::is_standard_layout_v<Prefix>);
static_assert(sizeof(Prefix) == 0x1c);
static_assert(offsetof(Prefix, word_00) == 0x00);
static_assert(offsetof(Prefix, word_04) == 0x04);
static_assert(offsetof(Prefix, word_08) == 0x08);
static_assert(offsetof(Prefix, word_0c) == 0x0c);
static_assert(offsetof(Prefix, word_10) == 0x10);
static_assert(offsetof(Prefix, word_14) == 0x14);
static_assert(offsetof(Prefix, word_18) == 0x18);

// Original portable candidate derived from committed textual evidence.
// NOT a validated native implementation, ABI match, or FAST_PASS promotion.
// A non-null argument must point to a live, writable Prefix.
// The null guard is a host safety policy, NOT an asserted native branch.
[[nodiscard]] Prefix* construct_candidate(Prefix* object) noexcept;

} // namespace re5::provisional::fun_00408700
