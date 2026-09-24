#include "re5/recovered/fun_00408640.hpp"

namespace re5::recovered {

FUN_00408640_Object* FUN_00408640(
    FUN_00408640_Object* object) noexcept {
    // Portable host guard only. Native 0x00408640 assumes a valid ECX.
    if (object == nullptr) {
        return nullptr;
    }

    // Preserve native observable write order: base constructor 0x00A67E10
    // installs its vtable and clears +4..+7; this constructor then installs
    // the derived vtable and returns the original this pointer.
    object->vtable = 0x0149F53CU;
    object->state = 0U;
    object->vtable = 0x0137E440U;
    return object;
}

} // namespace re5::recovered
