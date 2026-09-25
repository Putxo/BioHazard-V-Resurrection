#include "re5/scalar_detail.hpp"
namespace re5::scalar323d1aab {
// VA 0x011F4E30; 7 original bytes; store_imm32.
void RE5_SCALAR_CC FUN_011F4E30(void* self) noexcept {
    detail::store32(self,0U,0x01540180U);
}
// VA 0x011FDA60; 7 original bytes; store_imm32.
void RE5_SCALAR_CC FUN_011FDA60(void* self) noexcept {
    detail::store32(self,0U,0x0137A97CU);
}
// VA 0x0120C030; 10 original bytes; store_arg32.
void RE5_SCALAR_CC FUN_0120C030(void* self, u32 value) noexcept {
    detail::store32(self,100U,value);
}
// VA 0x0120C040; 10 original bytes; store_arg32.
void RE5_SCALAR_CC FUN_0120C040(void* self, u32 value) noexcept {
    detail::store32(self,96U,value);
}
// VA 0x0120C710; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_0120C710(u32, u32) noexcept {
}
// VA 0x01230890; 10 original bytes; store_arg32.
void RE5_SCALAR_CC FUN_01230890(void* self, u32 value) noexcept {
    detail::store32(self,56U,value);
}
// VA 0x0123B4D0; 4 original bytes; load8.
u8 RE5_SCALAR_CC FUN_0123B4D0(const void* self) noexcept {
    return static_cast<const u8*>(self)[40U];
}
// VA 0x0124E850; 7 original bytes; store_imm32.
void RE5_SCALAR_CC FUN_0124E850(void* self) noexcept {
    detail::store32(self,0U,0x01389064U);
}
// VA 0x01273880; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_01273880() noexcept {
}
// VA 0x012761D0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_012761D0() noexcept {
}
// VA 0x01278330; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_01278330(u32) noexcept {
}
// VA 0x01278340; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_01278340() noexcept {
}
// VA 0x01278350; 7 original bytes; store_imm32.
void RE5_SCALAR_CC FUN_01278350(void* self) noexcept {
    detail::store32(self,0U,0xFFFFFFD1U);
}
// VA 0x01279B10; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_01279B10(u32, u32, u32) noexcept {
}
// VA 0x01279B20; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_01279B20() noexcept {
}
// VA 0x01285CC0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_01285CC0() noexcept {
}
// VA 0x01291510; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_01291510() noexcept {
}
// VA 0x01291520; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_01291520() noexcept {
}
// VA 0x012B3230; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_012B3230() noexcept {
}
// VA 0x012B5250; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_012B5250(u32) noexcept {
}
// VA 0x012B5260; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_012B5260(u32) noexcept {
}
// VA 0x012B61B0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_012B61B0() noexcept {
}
// VA 0x01329BA0; 11 original bytes; global_copy32.
u32 FUN_01329BA0() noexcept {
    const u32 value=detail::load32(detail::address(0x015F0478U),0);
    detail::store32(detail::address(0x015F0500U),0,value);
    return value;
}
// VA 0x01329D20; 11 original bytes; global_copy32.
u32 FUN_01329D20() noexcept {
    const u32 value=detail::load32(detail::address(0x015F06C8U),0);
    detail::store32(detail::address(0x015F0740U),0,value);
    return value;
}
// VA 0x01329D60; 11 original bytes; global_copy32.
u32 FUN_01329D60() noexcept {
    const u32 value=detail::load32(detail::address(0x015F0754U),0);
    detail::store32(detail::address(0x015F0830U),0,value);
    return value;
}
// VA 0x0132A790; 11 original bytes; global_copy32.
u32 FUN_0132A790() noexcept {
    const u32 value=detail::load32(detail::address(0x015F1860U),0);
    detail::store32(detail::address(0x015F1880U),0,value);
    return value;
}
// VA 0x0132A7D0; 11 original bytes; global_copy32.
u32 FUN_0132A7D0() noexcept {
    const u32 value=detail::load32(detail::address(0x015F18F0U),0);
    detail::store32(detail::address(0x015F1910U),0,value);
    return value;
}
// VA 0x0132A960; 11 original bytes; global_copy32.
u32 FUN_0132A960() noexcept {
    const u32 value=detail::load32(detail::address(0x015F19E0U),0);
    detail::store32(detail::address(0x015F1A24U),0,value);
    return value;
}
// VA 0x0132A9D0; 11 original bytes; global_copy32.
u32 FUN_0132A9D0() noexcept {
    const u32 value=detail::load32(detail::address(0x015F19E0U),0);
    detail::store32(detail::address(0x015F1A74U),0,value);
    return value;
}
// VA 0x0132AA10; 11 original bytes; global_copy32.
u32 FUN_0132AA10() noexcept {
    const u32 value=detail::load32(detail::address(0x015F19E0U),0);
    detail::store32(detail::address(0x015F1AB4U),0,value);
    return value;
}
// VA 0x0132AA80; 11 original bytes; global_copy32.
u32 FUN_0132AA80() noexcept {
    const u32 value=detail::load32(detail::address(0x015F1B40U),0);
    detail::store32(detail::address(0x015FB780U),0,value);
    return value;
}
// VA 0x0132AB20; 11 original bytes; global_copy32.
u32 FUN_0132AB20() noexcept {
    const u32 value=detail::load32(detail::address(0x015F1BA4U),0);
    detail::store32(detail::address(0x015FB798U),0,value);
    return value;
}
// VA 0x0132AFD0; 11 original bytes; global_copy32.
u32 FUN_0132AFD0() noexcept {
    const u32 value=detail::load32(detail::address(0x015F21FCU),0);
    detail::store32(detail::address(0x015FBA08U),0,value);
    return value;
}
// VA 0x0132B260; 11 original bytes; global_copy32.
u32 FUN_0132B260() noexcept {
    const u32 value=detail::load32(detail::address(0x015F2C50U),0);
    detail::store32(detail::address(0x015FBBD0U),0,value);
    return value;
}
// VA 0x0132B460; 11 original bytes; global_copy32.
u32 FUN_0132B460() noexcept {
    const u32 value=detail::load32(detail::address(0x015F2F70U),0);
    detail::store32(detail::address(0x015FBC48U),0,value);
    return value;
}
// VA 0x0132B4E0; 11 original bytes; global_copy32.
u32 FUN_0132B4E0() noexcept {
    const u32 value=detail::load32(detail::address(0x015F2F74U),0);
    detail::store32(detail::address(0x015FBCA8U),0,value);
    return value;
}
// VA 0x0132B550; 11 original bytes; global_copy32.
u32 FUN_0132B550() noexcept {
    const u32 value=detail::load32(detail::address(0x015F3024U),0);
    detail::store32(detail::address(0x015FBF18U),0,value);
    return value;
}
// VA 0x0132B900; 11 original bytes; global_copy32.
u32 FUN_0132B900() noexcept {
    const u32 value=detail::load32(detail::address(0x015F39D0U),0);
    detail::store32(detail::address(0x015FC068U),0,value);
    return value;
}
// VA 0x0132BAA0; 11 original bytes; global_copy32.
u32 FUN_0132BAA0() noexcept {
    const u32 value=detail::load32(detail::address(0x015F39D4U),0);
    detail::store32(detail::address(0x015FC0F8U),0,value);
    return value;
}
// VA 0x0132BC00; 11 original bytes; global_copy32.
u32 FUN_0132BC00() noexcept {
    const u32 value=detail::load32(detail::address(0x015F3F60U),0);
    detail::store32(detail::address(0x015FC1E8U),0,value);
    return value;
}
// VA 0x0132BC80; 11 original bytes; global_copy32.
u32 FUN_0132BC80() noexcept {
    const u32 value=detail::load32(detail::address(0x015F3F7CU),0);
    detail::store32(detail::address(0x015FC290U),0,value);
    return value;
}
// VA 0x0132C000; 11 original bytes; global_copy32.
u32 FUN_0132C000() noexcept {
    const u32 value=detail::load32(detail::address(0x015F4420U),0);
    detail::store32(detail::address(0x015FC3F8U),0,value);
    return value;
}
// VA 0x0132C0D0; 11 original bytes; global_copy32.
u32 FUN_0132C0D0() noexcept {
    const u32 value=detail::load32(detail::address(0x015F3FCCU),0);
    detail::store32(detail::address(0x015FC500U),0,value);
    return value;
}
// VA 0x0132C240; 11 original bytes; global_copy32.
u32 FUN_0132C240() noexcept {
    const u32 value=detail::load32(detail::address(0x015F45BCU),0);
    detail::store32(detail::address(0x015FC878U),0,value);
    return value;
}
// VA 0x0132C8E0; 11 original bytes; global_copy32.
u32 FUN_0132C8E0() noexcept {
    const u32 value=detail::load32(detail::address(0x015F5614U),0);
    detail::store32(detail::address(0x015FD0D0U),0,value);
    return value;
}
// VA 0x0132C980; 11 original bytes; global_copy32.
u32 FUN_0132C980() noexcept {
    const u32 value=detail::load32(detail::address(0x015F57E0U),0);
    detail::store32(detail::address(0x015FD160U),0,value);
    return value;
}
// VA 0x0132CCD0; 11 original bytes; global_copy32.
u32 FUN_0132CCD0() noexcept {
    const u32 value=detail::load32(detail::address(0x015F5E30U),0);
    detail::store32(detail::address(0x015FD5E0U),0,value);
    return value;
}
// VA 0x0132CD70; 11 original bytes; global_copy32.
u32 FUN_0132CD70() noexcept {
    const u32 value=detail::load32(detail::address(0x015F5E68U),0);
    detail::store32(detail::address(0x015FD730U),0,value);
    return value;
}
// VA 0x0132CDE0; 11 original bytes; global_copy32.
u32 FUN_0132CDE0() noexcept {
    const u32 value=detail::load32(detail::address(0x015F5E6CU),0);
    detail::store32(detail::address(0x015FD7A8U),0,value);
    return value;
}
// VA 0x0132CFF0; 11 original bytes; global_copy32.
u32 FUN_0132CFF0() noexcept {
    const u32 value=detail::load32(detail::address(0x015F6168U),0);
    detail::store32(detail::address(0x015FD8B0U),0,value);
    return value;
}
// VA 0x0132D030; 11 original bytes; global_copy32.
u32 FUN_0132D030() noexcept {
    const u32 value=detail::load32(detail::address(0x015F6168U),0);
    detail::store32(detail::address(0x015FD8E0U),0,value);
    return value;
}
// VA 0x0132D0A0; 11 original bytes; global_copy32.
u32 FUN_0132D0A0() noexcept {
    const u32 value=detail::load32(detail::address(0x015F6168U),0);
    detail::store32(detail::address(0x015FD910U),0,value);
    return value;
}
// VA 0x0132D350; 11 original bytes; global_copy32.
u32 FUN_0132D350() noexcept {
    const u32 value=detail::load32(detail::address(0x015F62A4U),0);
    detail::store32(detail::address(0x015FD928U),0,value);
    return value;
}
// VA 0x0132D490; 11 original bytes; global_copy32.
u32 FUN_0132D490() noexcept {
    const u32 value=detail::load32(detail::address(0x015F65B0U),0);
    detail::store32(detail::address(0x015FD9E8U),0,value);
    return value;
}
// VA 0x0132DA10; 11 original bytes; global_copy32.
u32 FUN_0132DA10() noexcept {
    const u32 value=detail::load32(detail::address(0x015F7200U),0);
    detail::store32(detail::address(0x015FDA78U),0,value);
    return value;
}
// VA 0x0132DCB0; 11 original bytes; global_copy32.
u32 FUN_0132DCB0() noexcept {
    const u32 value=detail::load32(detail::address(0x015F72C8U),0);
    detail::store32(detail::address(0x015FDBF8U),0,value);
    return value;
}
// VA 0x0132DD20; 11 original bytes; global_copy32.
u32 FUN_0132DD20() noexcept {
    const u32 value=detail::load32(detail::address(0x015F7DB0U),0);
    detail::store32(detail::address(0x015FDCE8U),0,value);
    return value;
}
// VA 0x0132DE30; 11 original bytes; global_copy32.
u32 FUN_0132DE30() noexcept {
    const u32 value=detail::load32(detail::address(0x015F7DB0U),0);
    detail::store32(detail::address(0x015FDDA8U),0,value);
    return value;
}
// VA 0x0132E1B0; 11 original bytes; global_copy32.
u32 FUN_0132E1B0() noexcept {
    const u32 value=detail::load32(detail::address(0x015F7DB0U),0);
    detail::store32(detail::address(0x015FDDD8U),0,value);
    return value;
}
// VA 0x0132E290; 11 original bytes; global_copy32.
u32 FUN_0132E290() noexcept {
    const u32 value=detail::load32(detail::address(0x015F7DB0U),0);
    detail::store32(detail::address(0x015FE030U),0,value);
    return value;
}
// VA 0x0132E670; 11 original bytes; global_copy32.
u32 FUN_0132E670() noexcept {
    const u32 value=detail::load32(detail::address(0x015F7DBCU),0);
    detail::store32(detail::address(0x015FE078U),0,value);
    return value;
}
// VA 0x0132ED30; 11 original bytes; global_copy32.
u32 FUN_0132ED30() noexcept {
    const u32 value=detail::load32(detail::address(0x015F8638U),0);
    detail::store32(detail::address(0x015FE150U),0,value);
    return value;
}
// VA 0x0132EFC0; 11 original bytes; global_copy32.
u32 FUN_0132EFC0() noexcept {
    const u32 value=detail::load32(detail::address(0x015F8898U),0);
    detail::store32(detail::address(0x015FE2B8U),0,value);
    return value;
}
// VA 0x0132F5A0; 11 original bytes; global_copy32.
u32 FUN_0132F5A0() noexcept {
    const u32 value=detail::load32(detail::address(0x015F8F8CU),0);
    detail::store32(detail::address(0x015FE2E8U),0,value);
    return value;
}
// VA 0x0132F610; 11 original bytes; global_copy32.
u32 FUN_0132F610() noexcept {
    const u32 value=detail::load32(detail::address(0x015F9248U),0);
    detail::store32(detail::address(0x015FE318U),0,value);
    return value;
}
// VA 0x0132F930; 11 original bytes; global_copy32.
u32 FUN_0132F930() noexcept {
    const u32 value=detail::load32(detail::address(0x015F9614U),0);
    detail::store32(detail::address(0x015FE378U),0,value);
    return value;
}
// VA 0x0132FEE0; 11 original bytes; global_copy32.
u32 FUN_0132FEE0() noexcept {
    const u32 value=detail::load32(detail::address(0x015FA090U),0);
    detail::store32(detail::address(0x015FE960U),0,value);
    return value;
}
// VA 0x01330280; 11 original bytes; global_copy32.
u32 FUN_01330280() noexcept {
    const u32 value=detail::load32(detail::address(0x015FA0ACU),0);
    detail::store32(detail::address(0x015FE9F0U),0,value);
    return value;
}
// VA 0x01330B30; 11 original bytes; global_copy32.
u32 FUN_01330B30() noexcept {
    const u32 value=detail::load32(detail::address(0x015FEB38U),0);
    detail::store32(detail::address(0x01608730U),0,value);
    return value;
}
// VA 0x01330BD0; 11 original bytes; global_copy32.
u32 FUN_01330BD0() noexcept {
    const u32 value=detail::load32(detail::address(0x015FEB9CU),0);
    detail::store32(detail::address(0x01608748U),0,value);
    return value;
}
// VA 0x01331050; 11 original bytes; global_copy32.
u32 FUN_01331050() noexcept {
    const u32 value=detail::load32(detail::address(0x015FF1C4U),0);
    detail::store32(detail::address(0x016089E8U),0,value);
    return value;
}
// VA 0x013312E0; 11 original bytes; global_copy32.
u32 FUN_013312E0() noexcept {
    const u32 value=detail::load32(detail::address(0x015FFC18U),0);
    detail::store32(detail::address(0x01608BB0U),0,value);
    return value;
}
// VA 0x013314E0; 11 original bytes; global_copy32.
u32 FUN_013314E0() noexcept {
    const u32 value=detail::load32(detail::address(0x015FFF38U),0);
    detail::store32(detail::address(0x01608C28U),0,value);
    return value;
}
// VA 0x01331560; 11 original bytes; global_copy32.
u32 FUN_01331560() noexcept {
    const u32 value=detail::load32(detail::address(0x015FFF3CU),0);
    detail::store32(detail::address(0x01608C88U),0,value);
    return value;
}
// VA 0x013315D0; 11 original bytes; global_copy32.
u32 FUN_013315D0() noexcept {
    const u32 value=detail::load32(detail::address(0x015FFFECU),0);
    detail::store32(detail::address(0x01608EF8U),0,value);
    return value;
}
// VA 0x01331980; 11 original bytes; global_copy32.
u32 FUN_01331980() noexcept {
    const u32 value=detail::load32(detail::address(0x01600968U),0);
    detail::store32(detail::address(0x01609048U),0,value);
    return value;
}
// VA 0x01331B20; 11 original bytes; global_copy32.
u32 FUN_01331B20() noexcept {
    const u32 value=detail::load32(detail::address(0x0160096CU),0);
    detail::store32(detail::address(0x016090D8U),0,value);
    return value;
}
// VA 0x01331C80; 11 original bytes; global_copy32.
u32 FUN_01331C80() noexcept {
    const u32 value=detail::load32(detail::address(0x01600EF8U),0);
    detail::store32(detail::address(0x016091C8U),0,value);
    return value;
}
// VA 0x01331D00; 11 original bytes; global_copy32.
u32 FUN_01331D00() noexcept {
    const u32 value=detail::load32(detail::address(0x01600F14U),0);
    detail::store32(detail::address(0x01609270U),0,value);
    return value;
}
// VA 0x01332080; 11 original bytes; global_copy32.
u32 FUN_01332080() noexcept {
    const u32 value=detail::load32(detail::address(0x016013B8U),0);
    detail::store32(detail::address(0x016093D8U),0,value);
    return value;
}
// VA 0x01332150; 11 original bytes; global_copy32.
u32 FUN_01332150() noexcept {
    const u32 value=detail::load32(detail::address(0x01600F64U),0);
    detail::store32(detail::address(0x016094E0U),0,value);
    return value;
}
// VA 0x013322C0; 11 original bytes; global_copy32.
u32 FUN_013322C0() noexcept {
    const u32 value=detail::load32(detail::address(0x01601554U),0);
    detail::store32(detail::address(0x01609858U),0,value);
    return value;
}
// VA 0x01332960; 11 original bytes; global_copy32.
u32 FUN_01332960() noexcept {
    const u32 value=detail::load32(detail::address(0x016025ACU),0);
    detail::store32(detail::address(0x0160A0B0U),0,value);
    return value;
}
// VA 0x01332A00; 11 original bytes; global_copy32.
u32 FUN_01332A00() noexcept {
    const u32 value=detail::load32(detail::address(0x01602778U),0);
    detail::store32(detail::address(0x0160A140U),0,value);
    return value;
}
// VA 0x01332D50; 11 original bytes; global_copy32.
u32 FUN_01332D50() noexcept {
    const u32 value=detail::load32(detail::address(0x01602DC8U),0);
    detail::store32(detail::address(0x0160A5C0U),0,value);
    return value;
}
// VA 0x01332DF0; 11 original bytes; global_copy32.
u32 FUN_01332DF0() noexcept {
    const u32 value=detail::load32(detail::address(0x01602E00U),0);
    detail::store32(detail::address(0x0160A710U),0,value);
    return value;
}
// VA 0x01332E60; 11 original bytes; global_copy32.
u32 FUN_01332E60() noexcept {
    const u32 value=detail::load32(detail::address(0x01602E04U),0);
    detail::store32(detail::address(0x0160A788U),0,value);
    return value;
}
// VA 0x01333070; 11 original bytes; global_copy32.
u32 FUN_01333070() noexcept {
    const u32 value=detail::load32(detail::address(0x01603100U),0);
    detail::store32(detail::address(0x0160A890U),0,value);
    return value;
}
// VA 0x013330B0; 11 original bytes; global_copy32.
u32 FUN_013330B0() noexcept {
    const u32 value=detail::load32(detail::address(0x01603100U),0);
    detail::store32(detail::address(0x0160A8C0U),0,value);
    return value;
}
// VA 0x01333120; 11 original bytes; global_copy32.
u32 FUN_01333120() noexcept {
    const u32 value=detail::load32(detail::address(0x01603100U),0);
    detail::store32(detail::address(0x0160A8F0U),0,value);
    return value;
}
// VA 0x013333D0; 11 original bytes; global_copy32.
u32 FUN_013333D0() noexcept {
    const u32 value=detail::load32(detail::address(0x0160323CU),0);
    detail::store32(detail::address(0x0160A908U),0,value);
    return value;
}
// VA 0x01333510; 11 original bytes; global_copy32.
u32 FUN_01333510() noexcept {
    const u32 value=detail::load32(detail::address(0x01603548U),0);
    detail::store32(detail::address(0x0160A9C8U),0,value);
    return value;
}
// VA 0x01333A90; 11 original bytes; global_copy32.
u32 FUN_01333A90() noexcept {
    const u32 value=detail::load32(detail::address(0x01604198U),0);
    detail::store32(detail::address(0x0160AA58U),0,value);
    return value;
}
// VA 0x01333D30; 11 original bytes; global_copy32.
u32 FUN_01333D30() noexcept {
    const u32 value=detail::load32(detail::address(0x01604260U),0);
    detail::store32(detail::address(0x0160ABD8U),0,value);
    return value;
}
// VA 0x01333DA0; 11 original bytes; global_copy32.
u32 FUN_01333DA0() noexcept {
    const u32 value=detail::load32(detail::address(0x01604D18U),0);
    detail::store32(detail::address(0x0160ACC8U),0,value);
    return value;
}
// VA 0x01333EB0; 11 original bytes; global_copy32.
u32 FUN_01333EB0() noexcept {
    const u32 value=detail::load32(detail::address(0x01604D18U),0);
    detail::store32(detail::address(0x0160AD88U),0,value);
    return value;
}
// VA 0x01334230; 11 original bytes; global_copy32.
u32 FUN_01334230() noexcept {
    const u32 value=detail::load32(detail::address(0x01604D18U),0);
    detail::store32(detail::address(0x0160ADB8U),0,value);
    return value;
}
// VA 0x01334310; 11 original bytes; global_copy32.
u32 FUN_01334310() noexcept {
    const u32 value=detail::load32(detail::address(0x01604D18U),0);
    detail::store32(detail::address(0x0160B010U),0,value);
    return value;
}
// VA 0x013346F0; 11 original bytes; global_copy32.
u32 FUN_013346F0() noexcept {
    const u32 value=detail::load32(detail::address(0x01604D24U),0);
    detail::store32(detail::address(0x0160B058U),0,value);
    return value;
}
// VA 0x01334DB0; 11 original bytes; global_copy32.
u32 FUN_01334DB0() noexcept {
    const u32 value=detail::load32(detail::address(0x016055A0U),0);
    detail::store32(detail::address(0x0160B130U),0,value);
    return value;
}
// VA 0x01335040; 11 original bytes; global_copy32.
u32 FUN_01335040() noexcept {
    const u32 value=detail::load32(detail::address(0x01605800U),0);
    detail::store32(detail::address(0x0160B298U),0,value);
    return value;
}
// VA 0x01335620; 11 original bytes; global_copy32.
u32 FUN_01335620() noexcept {
    const u32 value=detail::load32(detail::address(0x01605EF4U),0);
    detail::store32(detail::address(0x0160B2C8U),0,value);
    return value;
}
// VA 0x01335690; 11 original bytes; global_copy32.
u32 FUN_01335690() noexcept {
    const u32 value=detail::load32(detail::address(0x016061B0U),0);
    detail::store32(detail::address(0x0160B2F8U),0,value);
    return value;
}
// VA 0x013359B0; 11 original bytes; global_copy32.
u32 FUN_013359B0() noexcept {
    const u32 value=detail::load32(detail::address(0x0160657CU),0);
    detail::store32(detail::address(0x0160B358U),0,value);
    return value;
}
// VA 0x013362F0; 11 original bytes; global_copy32.
u32 FUN_013362F0() noexcept {
    const u32 value=detail::load32(detail::address(0x016076E8U),0);
    detail::store32(detail::address(0x0160B988U),0,value);
    return value;
}
// VA 0x01336BA0; 11 original bytes; global_copy32.
u32 FUN_01336BA0() noexcept {
    const u32 value=detail::load32(detail::address(0x0160BAD0U),0);
    detail::store32(detail::address(0x016152C0U),0,value);
    return value;
}
// VA 0x01336C40; 11 original bytes; global_copy32.
u32 FUN_01336C40() noexcept {
    const u32 value=detail::load32(detail::address(0x0160BB34U),0);
    detail::store32(detail::address(0x016152D8U),0,value);
    return value;
}
// VA 0x01337040; 11 original bytes; global_copy32.
u32 FUN_01337040() noexcept {
    const u32 value=detail::load32(detail::address(0x0160BFECU),0);
    detail::store32(detail::address(0x01615410U),0,value);
    return value;
}
// VA 0x01337300; 11 original bytes; global_copy32.
u32 FUN_01337300() noexcept {
    const u32 value=detail::load32(detail::address(0x0160CA70U),0);
    detail::store32(detail::address(0x01615578U),0,value);
    return value;
}
// VA 0x01337400; 11 original bytes; global_copy32.
u32 FUN_01337400() noexcept {
    const u32 value=detail::load32(detail::address(0x0160CA74U),0);
    detail::store32(detail::address(0x016155F0U),0,value);
    return value;
}
// VA 0x013374C0; 11 original bytes; global_copy32.
u32 FUN_013374C0() noexcept {
    const u32 value=detail::load32(detail::address(0x0160CB9CU),0);
    detail::store32(detail::address(0x01615638U),0,value);
    return value;
}
// VA 0x01337530; 11 original bytes; global_copy32.
u32 FUN_01337530() noexcept {
    const u32 value=detail::load32(detail::address(0x0160CD9CU),0);
    detail::store32(detail::address(0x016158A8U),0,value);
    return value;
}
// VA 0x01337A00; 11 original bytes; global_copy32.
u32 FUN_01337A00() noexcept {
    const u32 value=detail::load32(detail::address(0x0160D880U),0);
    detail::store32(detail::address(0x01615980U),0,value);
    return value;
}
// VA 0x01337EC0; 11 original bytes; global_copy32.
u32 FUN_01337EC0() noexcept {
    const u32 value=detail::load32(detail::address(0x0160DF94U),0);
    detail::store32(detail::address(0x01615B18U),0,value);
    return value;
}
// VA 0x01337F90; 11 original bytes; global_copy32.
u32 FUN_01337F90() noexcept {
    const u32 value=detail::load32(detail::address(0x0160DF90U),0);
    detail::store32(detail::address(0x01615C20U),0,value);
    return value;
}
// VA 0x01338100; 11 original bytes; global_copy32.
u32 FUN_01338100() noexcept {
    const u32 value=detail::load32(detail::address(0x0160E310U),0);
    detail::store32(detail::address(0x01615F80U),0,value);
    return value;
}
// VA 0x013386E0; 11 original bytes; global_copy32.
u32 FUN_013386E0() noexcept {
    const u32 value=detail::load32(detail::address(0x0160ECA4U),0);
    detail::store32(detail::address(0x01616748U),0,value);
    return value;
}
// VA 0x01338760; 11 original bytes; global_copy32.
u32 FUN_01338760() noexcept {
    const u32 value=detail::load32(detail::address(0x0160F170U),0);
    detail::store32(detail::address(0x016167C0U),0,value);
    return value;
}
// VA 0x01338800; 11 original bytes; global_copy32.
u32 FUN_01338800() noexcept {
    const u32 value=detail::load32(detail::address(0x0160F174U),0);
    detail::store32(detail::address(0x01616850U),0,value);
    return value;
}
// VA 0x01338B50; 11 original bytes; global_copy32.
u32 FUN_01338B50() noexcept {
    const u32 value=detail::load32(detail::address(0x0160F7DCU),0);
    detail::store32(detail::address(0x01616CD0U),0,value);
    return value;
}
// VA 0x01338BF0; 11 original bytes; global_copy32.
u32 FUN_01338BF0() noexcept {
    const u32 value=detail::load32(detail::address(0x0160F91CU),0);
    detail::store32(detail::address(0x01616E20U),0,value);
    return value;
}
// VA 0x01338C60; 11 original bytes; global_copy32.
u32 FUN_01338C60() noexcept {
    const u32 value=detail::load32(detail::address(0x0160F920U),0);
    detail::store32(detail::address(0x01616E98U),0,value);
    return value;
}
// VA 0x01338E70; 11 original bytes; global_copy32.
u32 FUN_01338E70() noexcept {
    const u32 value=detail::load32(detail::address(0x0160FAB4U),0);
    detail::store32(detail::address(0x01616FA0U),0,value);
    return value;
}
// VA 0x01338EB0; 11 original bytes; global_copy32.
u32 FUN_01338EB0() noexcept {
    const u32 value=detail::load32(detail::address(0x0160FAB4U),0);
    detail::store32(detail::address(0x01616FD0U),0,value);
    return value;
}
// VA 0x01338F20; 11 original bytes; global_copy32.
u32 FUN_01338F20() noexcept {
    const u32 value=detail::load32(detail::address(0x0160FAB4U),0);
    detail::store32(detail::address(0x01617000U),0,value);
    return value;
}
// VA 0x013391D0; 11 original bytes; global_copy32.
u32 FUN_013391D0() noexcept {
    const u32 value=detail::load32(detail::address(0x0160FF20U),0);
    detail::store32(detail::address(0x01617018U),0,value);
    return value;
}
// VA 0x01339310; 11 original bytes; global_copy32.
u32 FUN_01339310() noexcept {
    const u32 value=detail::load32(detail::address(0x01610064U),0);
    detail::store32(detail::address(0x016170D8U),0,value);
    return value;
}
// VA 0x01339890; 11 original bytes; global_copy32.
u32 FUN_01339890() noexcept {
    const u32 value=detail::load32(detail::address(0x01610B04U),0);
    detail::store32(detail::address(0x01617150U),0,value);
    return value;
}
// VA 0x01339B60; 11 original bytes; global_copy32.
u32 FUN_01339B60() noexcept {
    const u32 value=detail::load32(detail::address(0x01610CBCU),0);
    detail::store32(detail::address(0x016172D0U),0,value);
    return value;
}
// VA 0x01339BD0; 11 original bytes; global_copy32.
u32 FUN_01339BD0() noexcept {
    const u32 value=detail::load32(detail::address(0x016113CCU),0);
    detail::store32(detail::address(0x016173C0U),0,value);
    return value;
}
// VA 0x01339D20; 11 original bytes; global_copy32.
u32 FUN_01339D20() noexcept {
    const u32 value=detail::load32(detail::address(0x016113CCU),0);
    detail::store32(detail::address(0x016175E8U),0,value);
    return value;
}
// VA 0x01339DA0; 11 original bytes; global_copy32.
u32 FUN_01339DA0() noexcept {
    const u32 value=detail::load32(detail::address(0x016113CCU),0);
    detail::store32(detail::address(0x01617690U),0,value);
    return value;
}
// VA 0x01339F60; 11 original bytes; global_copy32.
u32 FUN_01339F60() noexcept {
    const u32 value=detail::load32(detail::address(0x016113CCU),0);
    detail::store32(detail::address(0x01617708U),0,value);
    return value;
}
// VA 0x0133A220; 11 original bytes; global_copy32.
u32 FUN_0133A220() noexcept {
    const u32 value=detail::load32(detail::address(0x016113CCU),0);
    detail::store32(detail::address(0x01617798U),0,value);
    return value;
}
// VA 0x0133A590; 11 original bytes; global_copy32.
u32 FUN_0133A590() noexcept {
    const u32 value=detail::load32(detail::address(0x01611900U),0);
    detail::store32(detail::address(0x01617828U),0,value);
    return value;
}
// VA 0x0133AC20; 11 original bytes; global_copy32.
u32 FUN_0133AC20() noexcept {
    const u32 value=detail::load32(detail::address(0x01611CCCU),0);
    detail::store32(detail::address(0x01617900U),0,value);
    return value;
}
// VA 0x0133AEB0; 11 original bytes; global_copy32.
u32 FUN_0133AEB0() noexcept {
    const u32 value=detail::load32(detail::address(0x016122BCU),0);
    detail::store32(detail::address(0x01617A68U),0,value);
    return value;
}
// VA 0x0133B490; 11 original bytes; global_copy32.
u32 FUN_0133B490() noexcept {
    const u32 value=detail::load32(detail::address(0x01612D58U),0);
    detail::store32(detail::address(0x01617A98U),0,value);
    return value;
}
// VA 0x0133B500; 11 original bytes; global_copy32.
u32 FUN_0133B500() noexcept {
    const u32 value=detail::load32(detail::address(0x01612D8CU),0);
    detail::store32(detail::address(0x01617AC8U),0,value);
    return value;
}
// VA 0x0133B820; 11 original bytes; global_copy32.
u32 FUN_0133B820() noexcept {
    const u32 value=detail::load32(detail::address(0x01613200U),0);
    detail::store32(detail::address(0x01617B28U),0,value);
    return value;
}
// VA 0x0133C160; 11 original bytes; global_copy32.
u32 FUN_0133C160() noexcept {
    const u32 value=detail::load32(detail::address(0x01613A9CU),0);
    detail::store32(detail::address(0x01618110U),0,value);
    return value;
}
// VA 0x0133CA10; 11 original bytes; global_copy32.
u32 FUN_0133CA10() noexcept {
    const u32 value=detail::load32(detail::address(0x01618258U),0);
    detail::store32(detail::address(0x01621868U),0,value);
    return value;
}
// VA 0x0133CAB0; 11 original bytes; global_copy32.
u32 FUN_0133CAB0() noexcept {
    const u32 value=detail::load32(detail::address(0x016182BCU),0);
    detail::store32(detail::address(0x01621880U),0,value);
    return value;
}
// VA 0x0133CEB0; 11 original bytes; global_copy32.
u32 FUN_0133CEB0() noexcept {
    const u32 value=detail::load32(detail::address(0x01618774U),0);
    detail::store32(detail::address(0x016219B8U),0,value);
    return value;
}
// VA 0x0133D170; 11 original bytes; global_copy32.
u32 FUN_0133D170() noexcept {
    const u32 value=detail::load32(detail::address(0x016191F8U),0);
    detail::store32(detail::address(0x01621B20U),0,value);
    return value;
}
// VA 0x0133D270; 11 original bytes; global_copy32.
u32 FUN_0133D270() noexcept {
    const u32 value=detail::load32(detail::address(0x016191FCU),0);
    detail::store32(detail::address(0x01621B98U),0,value);
    return value;
}
// VA 0x0133D330; 11 original bytes; global_copy32.
u32 FUN_0133D330() noexcept {
    const u32 value=detail::load32(detail::address(0x01619324U),0);
    detail::store32(detail::address(0x01621BE0U),0,value);
    return value;
}
// VA 0x0133D3A0; 11 original bytes; global_copy32.
u32 FUN_0133D3A0() noexcept {
    const u32 value=detail::load32(detail::address(0x0161950CU),0);
    detail::store32(detail::address(0x01621E38U),0,value);
    return value;
}
// VA 0x0133D810; 11 original bytes; global_copy32.
u32 FUN_0133D810() noexcept {
    const u32 value=detail::load32(detail::address(0x01619E70U),0);
    detail::store32(detail::address(0x01621F10U),0,value);
    return value;
}
// VA 0x0133DCD0; 11 original bytes; global_copy32.
u32 FUN_0133DCD0() noexcept {
    const u32 value=detail::load32(detail::address(0x0161A584U),0);
    detail::store32(detail::address(0x016220A8U),0,value);
    return value;
}
// VA 0x0133DDA0; 11 original bytes; global_copy32.
u32 FUN_0133DDA0() noexcept {
    const u32 value=detail::load32(detail::address(0x0161A580U),0);
    detail::store32(detail::address(0x016221B0U),0,value);
    return value;
}
// VA 0x0133DF10; 11 original bytes; global_copy32.
u32 FUN_0133DF10() noexcept {
    const u32 value=detail::load32(detail::address(0x0161A77CU),0);
    detail::store32(detail::address(0x01622510U),0,value);
    return value;
}
// VA 0x0133E4C0; 11 original bytes; global_copy32.
u32 FUN_0133E4C0() noexcept {
    const u32 value=detail::load32(detail::address(0x0161B6B0U),0);
    detail::store32(detail::address(0x01622CD8U),0,value);
    return value;
}
// VA 0x0133E540; 11 original bytes; global_copy32.
u32 FUN_0133E540() noexcept {
    const u32 value=detail::load32(detail::address(0x0161B6B4U),0);
    detail::store32(detail::address(0x01622D50U),0,value);
    return value;
}
// VA 0x0133E5E0; 11 original bytes; global_copy32.
u32 FUN_0133E5E0() noexcept {
    const u32 value=detail::load32(detail::address(0x0161B880U),0);
    detail::store32(detail::address(0x01622DE0U),0,value);
    return value;
}
// VA 0x0133E930; 11 original bytes; global_copy32.
u32 FUN_0133E930() noexcept {
    const u32 value=detail::load32(detail::address(0x0161BEA0U),0);
    detail::store32(detail::address(0x01623260U),0,value);
    return value;
}
// VA 0x0133E9D0; 11 original bytes; global_copy32.
u32 FUN_0133E9D0() noexcept {
    const u32 value=detail::load32(detail::address(0x0161BED8U),0);
    detail::store32(detail::address(0x016233B0U),0,value);
    return value;
}
// VA 0x0133EA40; 11 original bytes; global_copy32.
u32 FUN_0133EA40() noexcept {
    const u32 value=detail::load32(detail::address(0x0161BEDCU),0);
    detail::store32(detail::address(0x01623428U),0,value);
    return value;
}
// VA 0x0133EC50; 11 original bytes; global_copy32.
u32 FUN_0133EC50() noexcept {
    const u32 value=detail::load32(detail::address(0x0161C1D8U),0);
    detail::store32(detail::address(0x01623530U),0,value);
    return value;
}
// VA 0x0133EC90; 11 original bytes; global_copy32.
u32 FUN_0133EC90() noexcept {
    const u32 value=detail::load32(detail::address(0x0161C1D8U),0);
    detail::store32(detail::address(0x01623560U),0,value);
    return value;
}
// VA 0x0133ED00; 11 original bytes; global_copy32.
u32 FUN_0133ED00() noexcept {
    const u32 value=detail::load32(detail::address(0x0161C1D8U),0);
    detail::store32(detail::address(0x01623590U),0,value);
    return value;
}
// VA 0x0133EFB0; 11 original bytes; global_copy32.
u32 FUN_0133EFB0() noexcept {
    const u32 value=detail::load32(detail::address(0x0161C314U),0);
    detail::store32(detail::address(0x016235A8U),0,value);
    return value;
}
// VA 0x0133F0F0; 11 original bytes; global_copy32.
u32 FUN_0133F0F0() noexcept {
    const u32 value=detail::load32(detail::address(0x0161C620U),0);
    detail::store32(detail::address(0x01623668U),0,value);
    return value;
}
// VA 0x0133F670; 11 original bytes; global_copy32.
u32 FUN_0133F670() noexcept {
    const u32 value=detail::load32(detail::address(0x0161D270U),0);
    detail::store32(detail::address(0x016236E0U),0,value);
    return value;
}
// VA 0x0133F940; 11 original bytes; global_copy32.
u32 FUN_0133F940() noexcept {
    const u32 value=detail::load32(detail::address(0x0161D380U),0);
    detail::store32(detail::address(0x01623860U),0,value);
    return value;
}
// VA 0x0133F9B0; 11 original bytes; global_copy32.
u32 FUN_0133F9B0() noexcept {
    const u32 value=detail::load32(detail::address(0x0161DEB0U),0);
    detail::store32(detail::address(0x01623950U),0,value);
    return value;
}
// VA 0x0133FB00; 11 original bytes; global_copy32.
u32 FUN_0133FB00() noexcept {
    const u32 value=detail::load32(detail::address(0x0161DEB0U),0);
    detail::store32(detail::address(0x01623B78U),0,value);
    return value;
}
// VA 0x0133FB80; 11 original bytes; global_copy32.
u32 FUN_0133FB80() noexcept {
    const u32 value=detail::load32(detail::address(0x0161DEB0U),0);
    detail::store32(detail::address(0x01623C20U),0,value);
    return value;
}
// VA 0x0133FD40; 11 original bytes; global_copy32.
u32 FUN_0133FD40() noexcept {
    const u32 value=detail::load32(detail::address(0x0161DEB0U),0);
    detail::store32(detail::address(0x01623C98U),0,value);
    return value;
}
// VA 0x01340000; 11 original bytes; global_copy32.
u32 FUN_01340000() noexcept {
    const u32 value=detail::load32(detail::address(0x0161DEB0U),0);
    detail::store32(detail::address(0x01623D28U),0,value);
    return value;
}
// VA 0x01340370; 11 original bytes; global_copy32.
u32 FUN_01340370() noexcept {
    const u32 value=detail::load32(detail::address(0x0161DEBCU),0);
    detail::store32(detail::address(0x01623DB8U),0,value);
    return value;
}
// VA 0x01340A00; 11 original bytes; global_copy32.
u32 FUN_01340A00() noexcept {
    const u32 value=detail::load32(detail::address(0x0161E720U),0);
    detail::store32(detail::address(0x01623E90U),0,value);
    return value;
}
// VA 0x01340C90; 11 original bytes; global_copy32.
u32 FUN_01340C90() noexcept {
    const u32 value=detail::load32(detail::address(0x0161E980U),0);
    detail::store32(detail::address(0x01623FF8U),0,value);
    return value;
}
// VA 0x01341270; 11 original bytes; global_copy32.
u32 FUN_01341270() noexcept {
    const u32 value=detail::load32(detail::address(0x0161F074U),0);
    detail::store32(detail::address(0x01624028U),0,value);
    return value;
}
// VA 0x013412E0; 11 original bytes; global_copy32.
u32 FUN_013412E0() noexcept {
    const u32 value=detail::load32(detail::address(0x0161F330U),0);
    detail::store32(detail::address(0x01624058U),0,value);
    return value;
}
// VA 0x01341600; 11 original bytes; global_copy32.
u32 FUN_01341600() noexcept {
    const u32 value=detail::load32(detail::address(0x0161F6FCU),0);
    detail::store32(detail::address(0x016240B8U),0,value);
    return value;
}
// VA 0x01341F40; 11 original bytes; global_copy32.
u32 FUN_01341F40() noexcept {
    const u32 value=detail::load32(detail::address(0x01620838U),0);
    detail::store32(detail::address(0x016246A0U),0,value);
    return value;
}
// VA 0x013427F0; 11 original bytes; global_copy32.
u32 FUN_013427F0() noexcept {
    const u32 value=detail::load32(detail::address(0x016247E8U),0);
    detail::store32(detail::address(0x0162DE50U),0,value);
    return value;
}
// VA 0x01342890; 11 original bytes; global_copy32.
u32 FUN_01342890() noexcept {
    const u32 value=detail::load32(detail::address(0x0162484CU),0);
    detail::store32(detail::address(0x0162DE68U),0,value);
    return value;
}
// VA 0x01342C90; 11 original bytes; global_copy32.
u32 FUN_01342C90() noexcept {
    const u32 value=detail::load32(detail::address(0x01624D04U),0);
    detail::store32(detail::address(0x0162DFA0U),0,value);
    return value;
}
// VA 0x01342D30; 11 original bytes; global_copy32.
u32 FUN_01342D30() noexcept {
    const u32 value=detail::load32(detail::address(0x016251D0U),0);
    detail::store32(detail::address(0x0162DFB8U),0,value);
    return value;
}
// VA 0x01342F20; 11 original bytes; global_copy32.
u32 FUN_01342F20() noexcept {
    const u32 value=detail::load32(detail::address(0x016256C0U),0);
    detail::store32(detail::address(0x0162E108U),0,value);
    return value;
}
// VA 0x01343020; 11 original bytes; global_copy32.
u32 FUN_01343020() noexcept {
    const u32 value=detail::load32(detail::address(0x016256C4U),0);
    detail::store32(detail::address(0x0162E180U),0,value);
    return value;
}
// VA 0x013430E0; 11 original bytes; global_copy32.
u32 FUN_013430E0() noexcept {
    const u32 value=detail::load32(detail::address(0x016257ECU),0);
    detail::store32(detail::address(0x0162E1C8U),0,value);
    return value;
}
// VA 0x01343160; 11 original bytes; global_copy32.
u32 FUN_01343160() noexcept {
    const u32 value=detail::load32(detail::address(0x016259D4U),0);
    detail::store32(detail::address(0x0162E450U),0,value);
    return value;
}
// VA 0x01343560; 11 original bytes; global_copy32.
u32 FUN_01343560() noexcept {
    const u32 value=detail::load32(detail::address(0x016262C0U),0);
    detail::store32(detail::address(0x0162E528U),0,value);
    return value;
}
// VA 0x01343A80; 11 original bytes; global_copy32.
u32 FUN_01343A80() noexcept {
    const u32 value=detail::load32(detail::address(0x01626A94U),0);
    detail::store32(detail::address(0x0162E6C0U),0,value);
    return value;
}
// VA 0x01343B50; 11 original bytes; global_copy32.
u32 FUN_01343B50() noexcept {
    const u32 value=detail::load32(detail::address(0x01626A90U),0);
    detail::store32(detail::address(0x0162E7C8U),0,value);
    return value;
}
// VA 0x01343D50; 11 original bytes; global_copy32.
u32 FUN_01343D50() noexcept {
    const u32 value=detail::load32(detail::address(0x01626C8CU),0);
    detail::store32(detail::address(0x0162EB28U),0,value);
    return value;
}
// VA 0x013442A0; 11 original bytes; global_copy32.
u32 FUN_013442A0() noexcept {
    const u32 value=detail::load32(detail::address(0x01627CB0U),0);
    detail::store32(detail::address(0x0162F2A8U),0,value);
    return value;
}
// VA 0x01344320; 11 original bytes; global_copy32.
u32 FUN_01344320() noexcept {
    const u32 value=detail::load32(detail::address(0x01627CB4U),0);
    detail::store32(detail::address(0x0162F320U),0,value);
    return value;
}
// VA 0x013443C0; 11 original bytes; global_copy32.
u32 FUN_013443C0() noexcept {
    const u32 value=detail::load32(detail::address(0x01627E80U),0);
    detail::store32(detail::address(0x0162F3B0U),0,value);
    return value;
}
// VA 0x01344710; 11 original bytes; global_copy32.
u32 FUN_01344710() noexcept {
    const u32 value=detail::load32(detail::address(0x016284A0U),0);
    detail::store32(detail::address(0x0162F830U),0,value);
    return value;
}
// VA 0x013447B0; 11 original bytes; global_copy32.
u32 FUN_013447B0() noexcept {
    const u32 value=detail::load32(detail::address(0x016284D8U),0);
    detail::store32(detail::address(0x0162F980U),0,value);
    return value;
}
// VA 0x01344820; 11 original bytes; global_copy32.
u32 FUN_01344820() noexcept {
    const u32 value=detail::load32(detail::address(0x016284DCU),0);
    detail::store32(detail::address(0x0162F9F8U),0,value);
    return value;
}
// VA 0x01344A30; 11 original bytes; global_copy32.
u32 FUN_01344A30() noexcept {
    const u32 value=detail::load32(detail::address(0x01628674U),0);
    detail::store32(detail::address(0x0162FB00U),0,value);
    return value;
}
// VA 0x01344A70; 11 original bytes; global_copy32.
u32 FUN_01344A70() noexcept {
    const u32 value=detail::load32(detail::address(0x01628674U),0);
    detail::store32(detail::address(0x0162FB30U),0,value);
    return value;
}
// VA 0x01344AE0; 11 original bytes; global_copy32.
u32 FUN_01344AE0() noexcept {
    const u32 value=detail::load32(detail::address(0x01628674U),0);
    detail::store32(detail::address(0x0162FB60U),0,value);
    return value;
}
// VA 0x01344D90; 11 original bytes; global_copy32.
u32 FUN_01344D90() noexcept {
    const u32 value=detail::load32(detail::address(0x01628AE0U),0);
    detail::store32(detail::address(0x0162FB78U),0,value);
    return value;
}
// VA 0x01344ED0; 11 original bytes; global_copy32.
u32 FUN_01344ED0() noexcept {
    const u32 value=detail::load32(detail::address(0x01628C24U),0);
    detail::store32(detail::address(0x0162FC38U),0,value);
    return value;
}
// VA 0x01345450; 11 original bytes; global_copy32.
u32 FUN_01345450() noexcept {
    const u32 value=detail::load32(detail::address(0x016296C4U),0);
    detail::store32(detail::address(0x0162FCB0U),0,value);
    return value;
}
// VA 0x01345720; 11 original bytes; global_copy32.
u32 FUN_01345720() noexcept {
    const u32 value=detail::load32(detail::address(0x0162987CU),0);
    detail::store32(detail::address(0x0162FE30U),0,value);
    return value;
}
// VA 0x01345790; 11 original bytes; global_copy32.
u32 FUN_01345790() noexcept {
    const u32 value=detail::load32(detail::address(0x01629F8CU),0);
    detail::store32(detail::address(0x0162FF20U),0,value);
    return value;
}
// VA 0x013458E0; 11 original bytes; global_copy32.
u32 FUN_013458E0() noexcept {
    const u32 value=detail::load32(detail::address(0x01629F8CU),0);
    detail::store32(detail::address(0x01630130U),0,value);
    return value;
}
// VA 0x01345960; 11 original bytes; global_copy32.
u32 FUN_01345960() noexcept {
    const u32 value=detail::load32(detail::address(0x01629F8CU),0);
    detail::store32(detail::address(0x016301D8U),0,value);
    return value;
}
// VA 0x01345B20; 11 original bytes; global_copy32.
u32 FUN_01345B20() noexcept {
    const u32 value=detail::load32(detail::address(0x01629F8CU),0);
    detail::store32(detail::address(0x01630250U),0,value);
    return value;
}
// VA 0x01345DE0; 11 original bytes; global_copy32.
u32 FUN_01345DE0() noexcept {
    const u32 value=detail::load32(detail::address(0x01629F8CU),0);
    detail::store32(detail::address(0x016302E0U),0,value);
    return value;
}
// VA 0x01346150; 11 original bytes; global_copy32.
u32 FUN_01346150() noexcept {
    const u32 value=detail::load32(detail::address(0x0162A4A8U),0);
    detail::store32(detail::address(0x01630370U),0,value);
    return value;
}
// VA 0x013467E0; 11 original bytes; global_copy32.
u32 FUN_013467E0() noexcept {
    const u32 value=detail::load32(detail::address(0x0162A85CU),0);
    detail::store32(detail::address(0x01630430U),0,value);
    return value;
}
// VA 0x01346A70; 11 original bytes; global_copy32.
u32 FUN_01346A70() noexcept {
    const u32 value=detail::load32(detail::address(0x0162AE4CU),0);
    detail::store32(detail::address(0x01630598U),0,value);
    return value;
}
// VA 0x01347050; 11 original bytes; global_copy32.
u32 FUN_01347050() noexcept {
    const u32 value=detail::load32(detail::address(0x0162B8E8U),0);
    detail::store32(detail::address(0x016305C8U),0,value);
    return value;
}
// VA 0x013470C0; 11 original bytes; global_copy32.
u32 FUN_013470C0() noexcept {
    const u32 value=detail::load32(detail::address(0x0162B91CU),0);
    detail::store32(detail::address(0x016305F8U),0,value);
    return value;
}
// VA 0x013473E0; 11 original bytes; global_copy32.
u32 FUN_013473E0() noexcept {
    const u32 value=detail::load32(detail::address(0x0162BD90U),0);
    detail::store32(detail::address(0x01630658U),0,value);
    return value;
}
// VA 0x01347D20; 11 original bytes; global_copy32.
u32 FUN_01347D20() noexcept {
    const u32 value=detail::load32(detail::address(0x0162C62CU),0);
    detail::store32(detail::address(0x01630C40U),0,value);
    return value;
}
// VA 0x013485D0; 11 original bytes; global_copy32.
u32 FUN_013485D0() noexcept {
    const u32 value=detail::load32(detail::address(0x01630D70U),0);
    detail::store32(detail::address(0x01639360U),0,value);
    return value;
}
// VA 0x01348670; 11 original bytes; global_copy32.
u32 FUN_01348670() noexcept {
    const u32 value=detail::load32(detail::address(0x01630DD4U),0);
    detail::store32(detail::address(0x01639378U),0,value);
    return value;
}
// VA 0x013489B0; 11 original bytes; global_copy32.
u32 FUN_013489B0() noexcept {
    const u32 value=detail::load32(detail::address(0x016311B4U),0);
    detail::store32(detail::address(0x01639510U),0,value);
    return value;
}
// VA 0x01348A50; 11 original bytes; global_copy32.
u32 FUN_01348A50() noexcept {
    const u32 value=detail::load32(detail::address(0x01631500U),0);
    detail::store32(detail::address(0x01639528U),0,value);
    return value;
}
// VA 0x01348D30; 11 original bytes; global_copy32.
u32 FUN_01348D30() noexcept {
    const u32 value=detail::load32(detail::address(0x01631B70U),0);
    detail::store32(detail::address(0x01639690U),0,value);
    return value;
}
// VA 0x01348DF0; 11 original bytes; global_copy32.
u32 FUN_01348DF0() noexcept {
    const u32 value=detail::load32(detail::address(0x01631CB0U),0);
    detail::store32(detail::address(0x016396D8U),0,value);
    return value;
}
// VA 0x01348E60; 11 original bytes; global_copy32.
u32 FUN_01348E60() noexcept {
    const u32 value=detail::load32(detail::address(0x01631CE8U),0);
    detail::store32(detail::address(0x016398D0U),0,value);
    return value;
}
// VA 0x01349260; 11 original bytes; global_copy32.
u32 FUN_01349260() noexcept {
    const u32 value=detail::load32(detail::address(0x01631D04U),0);
    detail::store32(detail::address(0x016399A8U),0,value);
    return value;
}
// VA 0x013497E0; 11 original bytes; global_copy32.
u32 FUN_013497E0() noexcept {
    const u32 value=detail::load32(detail::address(0x01632A14U),0);
    detail::store32(detail::address(0x01639B40U),0,value);
    return value;
}
// VA 0x01349860; 11 original bytes; global_copy32.
u32 FUN_01349860() noexcept {
    const u32 value=detail::load32(detail::address(0x01633050U),0);
    detail::store32(detail::address(0x01639D20U),0,value);
    return value;
}
// VA 0x013499C0; 11 original bytes; global_copy32.
u32 FUN_013499C0() noexcept {
    const u32 value=detail::load32(detail::address(0x01633054U),0);
    detail::store32(detail::address(0x01639E58U),0,value);
    return value;
}
// VA 0x01349CB0; 11 original bytes; global_copy32.
u32 FUN_01349CB0() noexcept {
    const u32 value=detail::load32(detail::address(0x016337D4U),0);
    detail::store32(detail::address(0x01639F78U),0,value);
    return value;
}
// VA 0x01349E40; 11 original bytes; global_copy32.
u32 FUN_01349E40() noexcept {
    const u32 value=detail::load32(detail::address(0x0163388CU),0);
    detail::store32(detail::address(0x0163A020U),0,value);
    return value;
}
// VA 0x01349E80; 11 original bytes; global_copy32.
u32 FUN_01349E80() noexcept {
    const u32 value=detail::load32(detail::address(0x0163388CU),0);
    detail::store32(detail::address(0x0163A050U),0,value);
    return value;
}
// VA 0x01349EF0; 11 original bytes; global_copy32.
u32 FUN_01349EF0() noexcept {
    const u32 value=detail::load32(detail::address(0x0163388CU),0);
    detail::store32(detail::address(0x0163A080U),0,value);
    return value;
}
// VA 0x0134A1A0; 11 original bytes; global_copy32.
u32 FUN_0134A1A0() noexcept {
    const u32 value=detail::load32(detail::address(0x01633DB8U),0);
    detail::store32(detail::address(0x0163A098U),0,value);
    return value;
}
// VA 0x0134A2E0; 11 original bytes; global_copy32.
u32 FUN_0134A2E0() noexcept {
    const u32 value=detail::load32(detail::address(0x01633EFCU),0);
    detail::store32(detail::address(0x0163A158U),0,value);
    return value;
}
// VA 0x0134A7D0; 11 original bytes; global_copy32.
u32 FUN_0134A7D0() noexcept {
    const u32 value=detail::load32(detail::address(0x01634ED8U),0);
    detail::store32(detail::address(0x0163A230U),0,value);
    return value;
}
// VA 0x0134AC30; 11 original bytes; global_copy32.
u32 FUN_0134AC30() noexcept {
    const u32 value=detail::load32(detail::address(0x01635468U),0);
    detail::store32(detail::address(0x0163A290U),0,value);
    return value;
}
// VA 0x0134B150; 11 original bytes; global_copy32.
u32 FUN_0134B150() noexcept {
    const u32 value=detail::load32(detail::address(0x0163583CU),0);
    detail::store32(detail::address(0x0163A410U),0,value);
    return value;
}
// VA 0x0134B1C0; 11 original bytes; global_copy32.
u32 FUN_0134B1C0() noexcept {
    const u32 value=detail::load32(detail::address(0x01636090U),0);
    detail::store32(detail::address(0x0163A500U),0,value);
    return value;
}
// VA 0x0134B340; 11 original bytes; global_copy32.
u32 FUN_0134B340() noexcept {
    const u32 value=detail::load32(detail::address(0x01636090U),0);
    detail::store32(detail::address(0x0163A710U),0,value);
    return value;
}
// VA 0x0134B3C0; 11 original bytes; global_copy32.
u32 FUN_0134B3C0() noexcept {
    const u32 value=detail::load32(detail::address(0x01636090U),0);
    detail::store32(detail::address(0x0163A818U),0,value);
    return value;
}
// VA 0x0134B610; 11 original bytes; global_copy32.
u32 FUN_0134B610() noexcept {
    const u32 value=detail::load32(detail::address(0x01636090U),0);
    detail::store32(detail::address(0x0163A8F0U),0,value);
    return value;
}
// VA 0x0134B800; 11 original bytes; global_copy32.
u32 FUN_0134B800() noexcept {
    const u32 value=detail::load32(detail::address(0x01636090U),0);
    detail::store32(detail::address(0x0163A920U),0,value);
    return value;
}
// VA 0x0134B8D0; 11 original bytes; global_copy32.
u32 FUN_0134B8D0() noexcept {
    const u32 value=detail::load32(detail::address(0x0163609CU),0);
    detail::store32(detail::address(0x0163A9B0U),0,value);
    return value;
}
// VA 0x0134BF60; 11 original bytes; global_copy32.
u32 FUN_0134BF60() noexcept {
    const u32 value=detail::load32(detail::address(0x016368B8U),0);
    detail::store32(detail::address(0x0163AA70U),0,value);
    return value;
}
// VA 0x0134C1F0; 11 original bytes; global_copy32.
u32 FUN_0134C1F0() noexcept {
    const u32 value=detail::load32(detail::address(0x01636B18U),0);
    detail::store32(detail::address(0x0163ABD8U),0,value);
    return value;
}
// VA 0x0134C770; 11 original bytes; global_copy32.
u32 FUN_0134C770() noexcept {
    const u32 value=detail::load32(detail::address(0x016371C4U),0);
    detail::store32(detail::address(0x0163AC08U),0,value);
    return value;
}
// VA 0x0134C7E0; 11 original bytes; global_copy32.
u32 FUN_0134C7E0() noexcept {
    const u32 value=detail::load32(detail::address(0x01637480U),0);
    detail::store32(detail::address(0x0163AC38U),0,value);
    return value;
}
// VA 0x0134CAD0; 11 original bytes; global_copy32.
u32 FUN_0134CAD0() noexcept {
    const u32 value=detail::load32(detail::address(0x01637834U),0);
    detail::store32(detail::address(0x0163AC98U),0,value);
    return value;
}
// VA 0x0134D330; 11 original bytes; global_copy32.
u32 FUN_0134D330() noexcept {
    const u32 value=detail::load32(detail::address(0x016382A0U),0);
    detail::store32(detail::address(0x0163B190U),0,value);
    return value;
}
// VA 0x0134DE80; 11 original bytes; global_copy32.
u32 FUN_0134DE80() noexcept {
    const u32 value=detail::load32(detail::address(0x0163B508U),0);
    detail::store32(detail::address(0x0163B530U),0,value);
    return value;
}
// VA 0x0134E070; 11 original bytes; global_copy32.
u32 FUN_0134E070() noexcept {
    const u32 value=detail::load32(detail::address(0x0163B5C0U),0);
    detail::store32(detail::address(0x0163B6B0U),0,value);
    return value;
}
// VA 0x0134E1E0; 11 original bytes; global_copy32.
u32 FUN_0134E1E0() noexcept {
    const u32 value=detail::load32(detail::address(0x0163B5C0U),0);
    detail::store32(detail::address(0x0163B8E8U),0,value);
    return value;
}
// VA 0x0134E340; 11 original bytes; global_copy32.
u32 FUN_0134E340() noexcept {
    const u32 value=detail::load32(detail::address(0x0163B948U),0);
    detail::store32(detail::address(0x0163BAE0U),0,value);
    return value;
}
// VA 0x0134E3E0; 11 original bytes; global_copy32.
u32 FUN_0134E3E0() noexcept {
    const u32 value=detail::load32(detail::address(0x0163B5C0U),0);
    detail::store32(detail::address(0x0163BBD0U),0,value);
    return value;
}
// VA 0x0134E580; 11 original bytes; global_copy32.
u32 FUN_0134E580() noexcept {
    const u32 value=detail::load32(detail::address(0x0163B5C0U),0);
    detail::store32(detail::address(0x0163BCD8U),0,value);
    return value;
}
// VA 0x0134F140; 11 original bytes; global_copy32.
u32 FUN_0134F140() noexcept {
    const u32 value=detail::load32(detail::address(0x0163D780U),0);
    detail::store32(detail::address(0x0163D790U),0,value);
    return value;
}
// VA 0x0134F1F0; 11 original bytes; global_copy32.
u32 FUN_0134F1F0() noexcept {
    const u32 value=detail::load32(detail::address(0x0163D890U),0);
    detail::store32(detail::address(0x0163D89CU),0,value);
    return value;
}
// VA 0x0134F350; 11 original bytes; global_copy32.
u32 FUN_0134F350() noexcept {
    const u32 value=detail::load32(detail::address(0x0163D8B0U),0);
    detail::store32(detail::address(0x0163DAA4U),0,value);
    return value;
}
// VA 0x01350870; 11 original bytes; global_copy32.
u32 FUN_01350870() noexcept {
    const u32 value=detail::load32(detail::address(0x0163E4D4U),0);
    detail::store32(detail::address(0x0163E510U),0,value);
    return value;
}
}
