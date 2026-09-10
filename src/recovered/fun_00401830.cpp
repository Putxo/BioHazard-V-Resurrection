#include "re5/recovered/fun_00401830.hpp"

namespace re5::recovered {

bool FUN_00401830(
    const std::uint8_t* first,
    const std::uint8_t* second,
    std::uint32_t length) noexcept {
    if (length == 0U) {
        return true;
    }

    while (length != 0U) {
        if (*first != *second) {
            return false;
        }
        ++first;
        ++second;
        --length;
    }

    return true;
}

} // namespace re5::recovered
