#include "re5/recovered/fun_00407e70.hpp"

#include "re5/recovered/fun_00407a90.hpp"

namespace re5::recovered {

void* FUN_00407E70(
    const FUN_00407E70_Object& object,
    std::uint32_t requested_size) {
    // The native wrapper receives this in ECX but never reads it.
    (void)object;

    // Exact body behavior: forward the single stack size argument to the
    // recovered allocator adapter with a null type/tag placeholder.
    return FUN_00407A90(requested_size, nullptr);
}

} // namespace re5::recovered
