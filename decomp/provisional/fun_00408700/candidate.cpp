#include "candidate.hpp"

namespace re5::provisional::fun_00408700 {

Prefix* construct_candidate(Prefix* object) noexcept {
    if (object == nullptr) {
        return nullptr; // Host-only guard; native null behavior is not inferred.
    }

    // Documented effects of 0x00A67E10, called by 0x00A712D0.
    object->word_00 = 0x0149f53cU;
    object->word_04 = 0U;

    // Documented effects of 0x00A712D0. The source evidence records the
    // literal bits at 0x01496BFC as 0x447A0000 (1000.0f).
    constexpr std::uint32_t documented_scalar_bits = 0x447a0000U;
    object->word_08 = 0U;
    object->word_0c = 0U;
    object->word_00 = 0x0149f69cU;
    object->word_10 = 0xffffffffU;
    object->word_18 = documented_scalar_bits;

    // Regional evidence associates this final vtable with FUN_00408700.
    // Do not call FUN_004086A0: that would install its unrelated derived table.
    object->word_00 = 0x0137e470U;
    return object;
}

} // namespace re5::provisional::fun_00408700
