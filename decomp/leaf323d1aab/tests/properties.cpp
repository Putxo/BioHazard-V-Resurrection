#include "re5/catalog.hpp"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <vector>
using namespace re5::leaf323d1aab;
namespace {
unsigned long long checks = 0;
[[noreturn]] void fail(const Entry& e, u32 value, u32 expected, u32 observed) {
    std::fprintf(stderr, "FAIL %08X input=%08X expected=%08X observed=%08X\n", e.va, value, expected, observed);
    std::exit(1);
}
u32 expectation(const Entry& e, u32 value, const void* self) {
    switch (e.kind) {
    case Kind::constant: return e.immediate;
    case Kind::zero: return 0U;
    case Kind::identity: return u32(reinterpret_cast<uptr>(self));
    case Kind::address: return u32(reinterpret_cast<uptr>(self)) + e.displacement;
    case Kind::load8: return value & 0xFFU;
    case Kind::load16: return value & 0xFFFFU;
    case Kind::load32: return value;
    case Kind::sign8: return (value & 0x80U) ? ((value & 0xFFU) | 0xFFFFFF00U) : value & 0xFFU;
    case Kind::sign16: return (value & 0x8000U) ? ((value & 0xFFFFU) | 0xFFFF0000U) : value & 0xFFFFU;
    }
    std::abort();
}
void check(const Entry& e, u32 value, void* self) {
    if (e.width) {
        auto* at = static_cast<u8*>(self) + e.displacement;
        for (unsigned j=0; j<e.width; ++j) at[j] = u8(value >> (8U*j));
    }
    const u32 expected = expectation(e, value, self);
    const u32 observed = e.function(self);
    ++checks;
    if (observed != expected) fail(e, value, expected, observed);
}
}
int main() {
    u32 max_offset=0;
    for (const auto& e : entries) max_offset=std::max(max_offset, e.displacement);
    std::vector<u8> object(max_offset+64U, 0xA5U);
    const u32 edges[] = {0U, 1U, 0x7FU, 0x80U, 0xFFU, 0x100U, 0x7FFFU, 0x8000U,
                        0xFFFFU, 0x10000U, 0x7FFFFFFFU, 0x80000000U, 0xFFFFFFFEU, 0xFFFFFFFFU};
    for (const auto& e : entries) {
        if (!e.width) {
            for (u32 token : edges) check(e, 0U, reinterpret_cast<void*>(uptr(token)));
            check(e, 0U, object.data());
            continue;
        }
        std::fill(object.begin(), object.end(), 0xA5U);
        void* self=object.data()+1U; // deliberately permits unaligned fields
        if (e.width==1U || e.width==2U) {
            const u32 count = e.width==1U ? 256U : 65536U;
            for (u32 value=0; value<count; ++value) check(e, value, self);
        } else {
            u32 state=e.va;
            for (unsigned i=0; i<1024U; ++i) {
                state=state*1664525U+1013904223U;
                check(e, state, self);
            }
        }
        for (u32 value : edges) {
            check(e, value, self);
            check(e, value, object.data()+3U);
        }
    }
    std::printf("{\"test\":\"source_properties\",\"routines\":%u,\"checks\":%llu,\"status\":\"PASS\"}\n", entry_count, checks);
}
