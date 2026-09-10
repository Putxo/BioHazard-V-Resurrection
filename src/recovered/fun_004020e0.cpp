#include "re5/recovered/fun_004020e0.hpp"

namespace re5::recovered {
namespace {
const FUN_004020E0_Globals* g_globals = nullptr;
}

void FUN_004020E0_SetGlobals(const FUN_004020E0_Globals* globals) noexcept {
    g_globals = globals;
}

std::uintptr_t FUN_004020E0() noexcept {
    return g_globals != nullptr ? g_globals->value_016e1660 : 0U;
}

std::uintptr_t FUN_004020F0() noexcept {
    return g_globals != nullptr ? g_globals->value_016e1690 : 0U;
}

std::uintptr_t FUN_00402100() noexcept {
    return 0x016E6FF0U;
}

} // namespace re5::recovered
