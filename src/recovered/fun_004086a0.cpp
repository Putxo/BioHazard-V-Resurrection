#include "re5/recovered/fun_004086a0.hpp"

namespace re5::recovered {

FUN_004086A0_Object* FUN_004086A0(
    FUN_004086A0_Object* object) noexcept {
    // Portable host guard only. Native 0x004086A0 assumes a valid ECX.
    if (object == nullptr) {
        return nullptr;
    }

    // Exact observable state of base constructor 0x00A712D0:
    //   0x00A67E10 first clears bytes +4..+7;
    //   0x00A712D0 clears +8/+0C, installs its own vtable,
    //   writes -1 at +0x10 and 1000.0f at +0x18.
    // Offset +0x14 is deliberately untouched.
    object->vtable = 0x0149F53CU;
    object->state = 0U;
    object->object_a = 0U;
    object->object_b = 0U;
    object->vtable = 0x0149F69CU;
    object->sentinel = 0xFFFFFFFFU;
    object->scalar = 1000.0F;

    // Native FUN_004086A0 then replaces only the leading vtable word.
    object->vtable = 0x0137E458U;
    return object;
}

} // namespace re5::recovered
