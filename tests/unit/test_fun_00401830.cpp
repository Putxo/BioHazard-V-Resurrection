#include "re5/recovered/fun_00401830.hpp"

#include <array>
#include <cassert>
#include <cstdint>

void test_fun_00401830() {
    using re5::recovered::FUN_00401830;

    const std::array<std::uint8_t, 5> first{1,2,3,4,5};
    const std::array<std::uint8_t, 5> same{1,2,3,4,5};
    const std::array<std::uint8_t, 5> different{1,2,9,4,5};

    assert(FUN_00401830(first.data(), same.data(), 5U));
    assert(!FUN_00401830(first.data(), different.data(), 5U));
    assert(FUN_00401830(first.data(), different.data(), 2U));
    assert(FUN_00401830(nullptr, nullptr, 0U));
}
