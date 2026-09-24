#include "re5/recovered/fun_00408040.hpp"

namespace re5::recovered {

FUN_00402360_String* FUN_00408040(
    const FUN_00402360_String& source,
    FUN_00402360_String& destination,
    const char* suffix) noexcept {
    destination.pointer = source.pointer;
    if (source.pointer != nullptr) {
        auto* block = static_cast<FUN_00402360_Block*>(source.pointer);
        ++block->ref_count;
    }

    FUN_00407FD0(destination, suffix);
    return &destination;
}

} // namespace re5::recovered
