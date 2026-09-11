#include "re5/recovered/fun_00402110.hpp"

namespace re5::recovered {

FUN_00402110_Dword* FUN_00402110(FUN_00402110_Dword& destination, const FUN_00402110_Dword& source) noexcept {
    destination.value = source.value;
    return &destination;
}

} // namespace re5::recovered
