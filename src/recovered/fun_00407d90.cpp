#include "re5/recovered/fun_00407d90.hpp"

namespace re5::recovered {

FUN_00402360_String* FUN_00407D90(
    const FUN_00402360_String& source,
    FUN_00402360_String& destination,
    const char* suffix) noexcept {
    const char* source_text = "";
    if (source.pointer != nullptr) {
        const auto* source_block =
            static_cast<const FUN_00402360_Block*>(source.pointer);
        source_text = source_block->data;
    }

    FUN_00402360(destination, source_text);
    FUN_00407D10(destination, suffix);
    return &destination;
}

} // namespace re5::recovered
