#include "re5/recovered/fun_00408510.hpp"

#include "re5/recovered/fun_004083c0.hpp"

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>

namespace {

char* inline_data(
    re5::recovered::FUN_00408510_String& value) noexcept {
    return reinterpret_cast<char*>(value.storage.data());
}

void set_heap_pointer(
    re5::recovered::FUN_004082E0_String& value,
    char* pointer) noexcept {
    std::memset(value.storage.data(), 0, value.storage.size());
    std::memcpy(value.storage.data(), &pointer, sizeof(pointer));
}

struct GrowState {
    std::array<char, 64> storage{};
    unsigned calls = 0U;
    std::uint32_t required_length = 0U;
    std::uint32_t preserved_length = 0U;
};

bool grow(
    void* context,
    re5::recovered::FUN_004082E0_String& value,
    std::uint32_t required_length,
    std::uint32_t preserved_length) noexcept {
    auto& state = *static_cast<GrowState*>(context);
    ++state.calls;
    state.required_length = required_length;
    state.preserved_length = preserved_length;

    set_heap_pointer(value, state.storage.data());
    value.capacity = 0x40U;
    return true;
}

} // namespace

void test_fun_00408510() {
    using namespace re5::recovered;

    FUN_004083C0_SetServices(nullptr);

    // Normal inline construction. Native code deliberately leaves the opaque
    // dword at +0 untouched while replacing length/capacity/first data byte.
    FUN_00408510_String inline_value{};
    inline_value.allocator_state = 0x12345678U;
    inline_value.capacity = 0xFFFFFFFFU;
    inline_value.length = 0xFFFFFFFFU;
    std::memset(
        inline_value.storage.data(),
        0x5A,
        inline_value.storage.size());

    assert(FUN_00408510(inline_value, "hello") == &inline_value);
    assert(inline_value.allocator_state == 0x12345678U);
    assert(inline_value.capacity == 0x0FU);
    assert(inline_value.length == 5U);
    assert(std::strcmp(inline_data(inline_value), "hello") == 0);

    // Empty C string reaches the already-recovered 83C0 zero-count path.
    FUN_00408510_String empty{};
    empty.allocator_state = 9U;
    assert(FUN_00408510(empty, "") == &empty);
    assert(empty.allocator_state == 9U);
    assert(empty.capacity == 0x0FU);
    assert(empty.length == 0U);
    assert(inline_data(empty)[0] == '\0');

    // Ordering proof: native initializes data[0] before scanning source. If
    // source aliases the previous inline storage, the first byte becomes NUL
    // before the length loop and the result is empty.
    FUN_00408510_String aliased{};
    aliased.allocator_state = 7U;
    std::strcpy(inline_data(aliased), "stale");
    const char* aliased_source = inline_data(aliased);
    assert(FUN_00408510(
               aliased,
               aliased_source) == &aliased);
    assert(aliased.length == 0U);
    assert(aliased.capacity == 0x0FU);
    assert(inline_data(aliased)[0] == '\0');

    constexpr char large_source[] = "0123456789abcdefghi";

    // Host-only guard inherited through FUN_004083C0: native 8100 is linked,
    // but an absent portable growth service must not overflow the 15-byte SSO
    // destination. Constructor initialization has already occurred.
    FUN_00408510_String missing_growth{};
    missing_growth.allocator_state = 0xA5A5A5A5U;
    std::memset(
        missing_growth.storage.data(),
        0x44,
        missing_growth.storage.size());

    assert(FUN_00408510(
               missing_growth,
               large_source) == &missing_growth);
    assert(missing_growth.allocator_state == 0xA5A5A5A5U);
    assert(missing_growth.capacity == 0x0FU);
    assert(missing_growth.length == 0U);
    assert(inline_data(missing_growth)[0] == '\0');

    // Growth-required construction forwards the exact strlen result to 83C0
    // and, because this constructor initialized length to zero first, requests
    // preserved_length == 0 from the 8100-shaped growth seam.
    GrowState grow_state{};
    FUN_004083C0_Services services{
        &grow_state,
        grow,
    };
    FUN_004083C0_SetServices(&services);

    FUN_00408510_String grown{};
    grown.allocator_state = 0xCAFEBABEU;
    assert(FUN_00408510(grown, large_source) == &grown);
    assert(grow_state.calls == 1U);
    assert(grow_state.required_length == 19U);
    assert(grow_state.preserved_length == 0U);
    assert(grown.allocator_state == 0xCAFEBABEU);
    assert(grown.capacity == 0x40U);
    assert(grown.length == 19U);
    assert(std::strcmp(
               grow_state.storage.data(),
               large_source) == 0);

    FUN_004083C0_SetServices(nullptr);
}
