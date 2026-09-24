#include "re5/recovered/fun_00407cf0.hpp"

namespace re5::recovered {

FUN_00407CF0_Object* FUN_00407CF0(
    FUN_00407CF0_Object& destination,
    const char* input) noexcept {
    // Exact native wrapper store at 0x00407CF8 before delegating.
    destination.pointer = nullptr;
    FUN_00402360(destination, input);
    return &destination;
}

} // namespace re5::recovered
