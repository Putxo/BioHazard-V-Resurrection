#include "re5/recovered/fun_00408620.hpp"

#include <cassert>
#include <cstdint>

void test_fun_00408620() {
    using namespace re5::recovered;

    // The native body has no guards, branches, memory reads, or side effects:
    // every call returns the same fixed 32-bit address token.
    assert(FUN_00408620() == static_cast<std::uintptr_t>(0x016F0230U));

    // Repeated calls prove the host reconstruction is equally stateless and
    // does not depend on mutable process/global state.
    assert(FUN_00408620() == static_cast<std::uintptr_t>(0x016F0230U));
}
