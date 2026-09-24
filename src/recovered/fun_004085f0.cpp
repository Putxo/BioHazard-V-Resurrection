#include "re5/recovered/fun_004085f0.hpp"

namespace re5::recovered {

void FUN_004085F0(
    FUN_004085F0_Object& object,
    std::uint8_t enabled) noexcept {
    // Native code tests only the low byte of the single stack argument.
    if (enabled != 0U) {
        object.flags |= FUN_004085F0_FIX_MASK;
        return;
    }

    object.flags &= ~FUN_004085F0_FIX_MASK;
}

} // namespace re5::recovered
