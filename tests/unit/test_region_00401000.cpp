#include "re5/recovered/region_00401000.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>

namespace {
std::array<std::uintptr_t, 4> g_seen{};
std::size_t g_seen_count = 0;

void record_callback(void* ptr) noexcept {
    if (g_seen_count < g_seen.size()) {
        g_seen[g_seen_count++] = reinterpret_cast<std::uintptr_t>(ptr);
    }
}
} // namespace

void test_region_00401000() {
    using namespace re5::recovered;

    g_seen = {};
    g_seen_count = 0;
    alignas(16) std::array<std::byte, 64> storage{};
    FUN_004011B0(storage.data(), 8U, 3U, &record_callback);
    assert(g_seen_count == 3U);

    const auto base = reinterpret_cast<std::uintptr_t>(storage.data());
    assert(g_seen[0] == base);
    assert(g_seen[1] == base + 8U);
    assert(g_seen[2] == base + 16U);

    g_seen_count = 0;
    FUN_004011B0(storage.data(), 8U, 0U, &record_callback);
    assert(g_seen_count == 0U);

    assert(FUN_004011F0() == 0x01652E00U);
    assert(!FUN_00401220());

    FUN_00401230(0x1234U);
    FUN_00401240(0x5678U);

    void* aligned = FUN_00401200(64U);
    assert(aligned != nullptr);
    assert((reinterpret_cast<std::uintptr_t>(aligned) & 0xFU) == 0U);
    FUN_00401000(aligned);
}
