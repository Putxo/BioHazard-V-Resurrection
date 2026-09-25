#include "candidate.hpp"

#include <array>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string_view>

using re5::provisional::fun_00408700::Prefix;
using re5::provisional::fun_00408700::construct_candidate;

namespace {
void require(bool condition, const char* message) {
    if (!condition) {
        throw std::runtime_error(message); // Active even in Release/NDEBUG.
    }
}
void verify(const Prefix& p, std::uint32_t preserved) {
    require(p.word_00 == 0x0137e470U, "wrong final vtable");
    require(p.word_04 == 0U, "wrong +04");
    require(p.word_08 == 0U, "wrong +08");
    require(p.word_0c == 0U, "wrong +0c");
    require(p.word_10 == 0xffffffffU, "wrong sentinel");
    require(p.word_14 == preserved, "+14 was overwritten");
    require(p.word_18 == 0x447a0000U, "wrong scalar bit pattern");
}
void null_guard() {
    require(construct_candidate(nullptr) == nullptr, "host null guard failed");
}
void known_writes() {
    Prefix p{1U, 2U, 3U, 4U, 5U, 6U, 7U};
    require(construct_candidate(&p) == &p, "did not return original pointer");
    verify(p, 6U);
}
void preserved_word() {
    constexpr std::array values{0U, 1U, 0x80000000U, 0xffffffffU,
                                0x0137e470U, 0xdeadbeefU};
    for (const auto value : values) {
        Prefix p{};
        p.word_14 = value;
        require(construct_candidate(&p) == &p, "wrong preserved-test return");
        verify(p, value);
    }
}
void guard_regions() {
    struct Envelope {
        std::array<std::uint32_t, 8> before;
        Prefix object;
        std::array<std::uint32_t, 8> after;
    };
    Envelope e{};
    e.before.fill(0xa5a5a5a5U);
    e.after.fill(0x5a5a5a5aU);
    const auto before = e.before;
    const auto after = e.after;
    e.object.word_14 = 0xababababU;
    require(construct_candidate(&e.object) == &e.object, "wrong interior pointer");
    verify(e.object, 0xababababU);
    require(e.before == before, "wrote before prefix");
    require(e.after == after, "wrote beyond prefix");
}
void repeated_construction() {
    Prefix p{};
    for (std::uint32_t n = 0; n < 16; ++n) {
        p = Prefix{0x0137e470U, n+1U, n+2U, n+3U, n+4U, n+5U, n+6U};
        require(construct_candidate(&p) == &p, "wrong repeat return");
        verify(p, n+5U);
    }
}
void independent_objects() {
    Prefix a{1U, 2U, 3U, 4U, 5U, 6U, 7U};
    Prefix b{8U, 9U, 10U, 11U, 12U, 13U, 14U};
    const auto original_b = b;
    require(construct_candidate(&a) == &a, "wrong independent return");
    require(std::memcmp(&b, &original_b, sizeof(b)) == 0, "unrelated object changed");
    verify(a, 6U);
}
void byte_patterns() {
    for (int pattern = 0; pattern < 256; ++pattern) {
        Prefix p{};
        std::memset(&p, pattern, sizeof(p));
        const auto preserved = p.word_14;
        require(construct_candidate(&p) == &p, "wrong pattern return");
        verify(p, preserved);
    }
}
struct Test { std::string_view name; void (*run)(); };
constexpr std::array tests{
    Test{"null_guard", null_guard}, Test{"known_writes", known_writes},
    Test{"preserved_word", preserved_word}, Test{"guard_regions", guard_regions},
    Test{"repeated_construction", repeated_construction},
    Test{"independent_objects", independent_objects}, Test{"byte_patterns", byte_patterns}
};
} // namespace

int main(int argc, char** argv) {
    try {
        if (argc != 2) {
            throw std::runtime_error("one named candidate test is required");
        }
        for (const auto& test : tests) {
            if (test.name == argv[1]) {
                test.run();
                std::cout << "PASS (candidate only): " << test.name << '\n';
                return 0;
            }
        }
        throw std::runtime_error("unknown candidate test");
    } catch (const std::exception& error) {
        std::cerr << "FAIL: " << error.what() << '\n';
        return 1;
    }
}
