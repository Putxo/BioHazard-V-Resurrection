#include "re5/detail.hpp"
namespace re5::leaf323d1aab {
// VA 0x01280C90; 4 original bytes; load32.
u32 RE5_LEAF_CC FUN_01280C90(const void* self) noexcept {
    return detail::load32(self, 20U);
}
// VA 0x012B57E0; 4 original bytes; load32.
u32 RE5_LEAF_CC FUN_012B57E0(const void* self) noexcept {
    return detail::load32(self, 8U);
}
// VA 0x012B6210; 3 original bytes; load32.
u32 RE5_LEAF_CC FUN_012B6210(const void* self) noexcept {
    return detail::load32(self, 0U);
}
} // namespace re5::leaf323d1aab
