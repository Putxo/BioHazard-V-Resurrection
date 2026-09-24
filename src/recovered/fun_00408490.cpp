#include "re5/recovered/fun_00408490.hpp"

namespace re5::recovered {
namespace {
constexpr std::uint32_t kLogicErrorVtable = 0x01541F54U;
}

FUN_00408490_Object* FUN_00408490(
    FUN_00408490_Object& object,
    const FUN_004082E0_String& source) {
    // Exact observable state produced by the native std::exception base
    // constructor at 0x01251B2C before the derived vtable overwrite.
    object.base_message = 0U;
    object.base_owns_message = 0U;
    object.vtable = kLogicErrorVtable;

    // Native constructor initializes only the visible SSO invariants. The
    // allocator_state dword at message+0x00 is intentionally left untouched.
    object.message.length = 0U;
    object.message.capacity = 0x0FU;
    reinterpret_cast<char*>(object.message.storage.data())[0] = '\0';

    (void)FUN_004082E0(
        object.message,
        source,
        0U,
        0xFFFFFFFFU);
    return &object;
}

} // namespace re5::recovered
