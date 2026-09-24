#include "re5/recovered/fun_00408510.hpp"

#include "re5/recovered/fun_004083c0.hpp"

#include <cstdint>

namespace re5::recovered {

FUN_00408510_String* FUN_00408510(
    FUN_00408510_String& destination,
    const char* source) {
    destination.capacity = 0x0FU;
    destination.length = 0U;
    reinterpret_cast<char*>(destination.storage.data())[0] = '\0';

    // Native code performs its own byte scan after initializing the object.
    // Keep that ordering instead of precomputing the length.
    const char* cursor = source;
    while (*cursor != '\0') {
        ++cursor;
    }

    const auto length =
        static_cast<std::uint32_t>(cursor - source);
    (void)FUN_004083C0(destination, source, length);
    return &destination;
}

} // namespace re5::recovered
