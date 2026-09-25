#include "re5/scalar_detail.hpp"
namespace re5::scalar323d1aab {
// VA 0x00565D10; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00565D10() noexcept {
}
// VA 0x00566D20; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00566D20() noexcept {
}
// VA 0x00566E10; 3 original bytes; constant8.
u8 RE5_SCALAR_CC FUN_00566E10(const void*) noexcept {
    return 1U;
}
// VA 0x00566E70; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00566E70() noexcept {
}
// VA 0x00567100; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00567100() noexcept {
}
// VA 0x00567B60; 7 original bytes; store_imm32.
void RE5_SCALAR_CC FUN_00567B60(void* self) noexcept {
    detail::store32(self,0U,0x0137A97CU);
}
// VA 0x00568040; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00568040() noexcept {
}
// VA 0x0056A220; 7 original bytes; store_imm32.
void RE5_SCALAR_CC FUN_0056A220(void* self) noexcept {
    detail::store32(self,0U,0x0137A97CU);
}
// VA 0x0056A230; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0056A230() noexcept {
}
// VA 0x0056A300; 3 original bytes; constant8.
u8 RE5_SCALAR_CC FUN_0056A300(const void*) noexcept {
    return 1U;
}
// VA 0x0056AE20; 12 original bytes; store_arg16.
void RE5_SCALAR_CC FUN_0056AE20(void* self, u16 value) noexcept {
    detail::store16(self,6U,value);
}
// VA 0x0056AEA0; 10 original bytes; store_arg8.
void RE5_SCALAR_CC FUN_0056AEA0(void* self, u8 value) noexcept {
    detail::store8(self,89U,value);
}
// VA 0x0056AED0; 13 original bytes; store_arg32.
void RE5_SCALAR_CC FUN_0056AED0(void* self, u32 value) noexcept {
    detail::store32(self,240U,value);
}
// VA 0x00573390; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_00573390(const void* self) noexcept {
    return static_cast<const u8*>(self)[916U];
}
// VA 0x005733A0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_005733A0(const void* self) noexcept {
    return static_cast<const u8*>(self)[917U];
}
// VA 0x005733B0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_005733B0(const void* self) noexcept {
    return static_cast<const u8*>(self)[918U];
}
// VA 0x005733C0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_005733C0(const void* self) noexcept {
    return static_cast<const u8*>(self)[919U];
}
// VA 0x005733D0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005733D0() noexcept {
}
// VA 0x005733E0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005733E0() noexcept {
}
// VA 0x005733F0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005733F0() noexcept {
}
// VA 0x00573400; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00573400() noexcept {
}
// VA 0x00573410; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00573410() noexcept {
}
// VA 0x00577070; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00577070() noexcept {
}
// VA 0x00577210; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00577210() noexcept {
}
// VA 0x00577220; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00577220() noexcept {
}
// VA 0x00577230; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00577230() noexcept {
}
// VA 0x005772B0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005772B0() noexcept {
}
// VA 0x005772C0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005772C0() noexcept {
}
// VA 0x00577300; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_00577300(u32) noexcept {
}
// VA 0x0057A2B0; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_0057A2B0(u32) noexcept {
}
// VA 0x0057B4F0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0057B4F0() noexcept {
}
// VA 0x0057B830; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0057B830() noexcept {
}
// VA 0x0057B870; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0057B870() noexcept {
}
// VA 0x0057BF00; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0057BF00() noexcept {
}
// VA 0x0057C540; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0057C540() noexcept {
}
// VA 0x0057C550; 11 original bytes; store_imm32.
void RE5_SCALAR_CC FUN_0057C550(void* self) noexcept {
    detail::store32(self,328U,0x00000000U);
}
// VA 0x0057C600; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0057C600() noexcept {
}
// VA 0x0057C610; 11 original bytes; store_imm32.
void RE5_SCALAR_CC FUN_0057C610(void* self) noexcept {
    detail::store32(self,328U,0x00000000U);
}
// VA 0x0057C6E0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0057C6E0() noexcept {
}
// VA 0x0057C900; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0057C900() noexcept {
}
// VA 0x0057C9C0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0057C9C0() noexcept {
}
// VA 0x0057C9E0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0057C9E0() noexcept {
}
// VA 0x0057D140; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0057D140() noexcept {
}
// VA 0x0057D170; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_0057D170(u32, u32) noexcept {
}
// VA 0x0057D180; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_0057D180(u32, u32) noexcept {
}
// VA 0x0057D190; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_0057D190(u32, u32) noexcept {
}
// VA 0x0057D1A0; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_0057D1A0(u32, u32) noexcept {
}
// VA 0x0057D240; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_0057D240(u32, u32) noexcept {
}
// VA 0x0057D250; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_0057D250(u32, u32) noexcept {
}
// VA 0x0057D260; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_0057D260(u32, u32) noexcept {
}
// VA 0x0057D270; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_0057D270(u32, u32) noexcept {
}
// VA 0x0057D550; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0057D550() noexcept {
}
// VA 0x0057D560; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0057D560() noexcept {
}
// VA 0x0057D580; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0057D580() noexcept {
}
// VA 0x0057D680; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_0057D680(u32) noexcept {
}
// VA 0x0057D6B0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0057D6B0() noexcept {
}
// VA 0x0057D7D0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0057D7D0() noexcept {
}
// VA 0x0057D870; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_0057D870(u32) noexcept {
}
// VA 0x0057D8A0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0057D8A0() noexcept {
}
// VA 0x0057D920; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0057D920() noexcept {
}
// VA 0x0057D930; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0057D930() noexcept {
}
// VA 0x0057DAD0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0057DAD0() noexcept {
}
// VA 0x0057DAF0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0057DAF0() noexcept {
}
// VA 0x0057DB00; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0057DB00() noexcept {
}
// VA 0x0057DF80; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0057DF80() noexcept {
}
// VA 0x0057E020; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0057E020() noexcept {
}
// VA 0x0057E220; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_0057E220(u32, u32) noexcept {
}
// VA 0x0057E2D0; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_0057E2D0(u32, u32) noexcept {
}
// VA 0x0057E2E0; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_0057E2E0(u32, u32) noexcept {
}
// VA 0x0057E2F0; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_0057E2F0(u32, u32) noexcept {
}
// VA 0x0057E300; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_0057E300(u32, u32) noexcept {
}
// VA 0x0058BBC0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0058BBC0() noexcept {
}
// VA 0x0058C010; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0058C010() noexcept {
}
// VA 0x0058C030; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0058C030() noexcept {
}
// VA 0x0058C170; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0058C170() noexcept {
}
// VA 0x0058C2C0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0058C2C0() noexcept {
}
// VA 0x0058ED80; 10 original bytes; store_arg32.
void RE5_SCALAR_CC FUN_0058ED80(void* self, u32 value) noexcept {
    detail::store32(self,28U,value);
}
// VA 0x0058F760; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0058F760() noexcept {
}
// VA 0x0058FC20; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0058FC20() noexcept {
}
// VA 0x005906A0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005906A0() noexcept {
}
// VA 0x00590720; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00590720() noexcept {
}
// VA 0x00590A30; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00590A30() noexcept {
}
// VA 0x00591240; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00591240() noexcept {
}
// VA 0x00591500; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00591500() noexcept {
}
// VA 0x005935D0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005935D0() noexcept {
}
// VA 0x005935E0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005935E0() noexcept {
}
// VA 0x00593650; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00593650() noexcept {
}
// VA 0x005936C0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005936C0() noexcept {
}
// VA 0x005936D0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005936D0() noexcept {
}
// VA 0x00593710; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00593710() noexcept {
}
// VA 0x00593E80; 13 original bytes; store_arg32.
void RE5_SCALAR_CC FUN_00593E80(void* self, u32 value) noexcept {
    detail::store32(self,220U,value);
}
// VA 0x00593EC0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00593EC0() noexcept {
}
// VA 0x00594BE0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00594BE0() noexcept {
}
// VA 0x00594C60; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00594C60() noexcept {
}
// VA 0x00594DD0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00594DD0() noexcept {
}
// VA 0x00594E50; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00594E50() noexcept {
}
// VA 0x005B1210; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005B1210() noexcept {
}
// VA 0x005B1220; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005B1220() noexcept {
}
// VA 0x005B1230; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005B1230() noexcept {
}
// VA 0x005B1240; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005B1240() noexcept {
}
// VA 0x005B6AA0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005B6AA0() noexcept {
}
// VA 0x005B6AB0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005B6AB0() noexcept {
}
// VA 0x005B6AD0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005B6AD0() noexcept {
}
// VA 0x005B6AE0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005B6AE0() noexcept {
}
// VA 0x005B6AF0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005B6AF0() noexcept {
}
// VA 0x005B6B00; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005B6B00() noexcept {
}
// VA 0x005B6B10; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005B6B10() noexcept {
}
// VA 0x005B6B20; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005B6B20() noexcept {
}
// VA 0x005B6B30; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005B6B30() noexcept {
}
// VA 0x005B6B40; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005B6B40() noexcept {
}
// VA 0x005B6B50; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005B6B50() noexcept {
}
// VA 0x005B74E0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005B74E0() noexcept {
}
// VA 0x005B7560; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005B7560() noexcept {
}
// VA 0x005B7570; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005B7570() noexcept {
}
// VA 0x005B7580; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005B7580() noexcept {
}
// VA 0x005BE0E0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005BE0E0() noexcept {
}
// VA 0x005BFEB0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005BFEB0() noexcept {
}
// VA 0x005BFED0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005BFED0() noexcept {
}
// VA 0x005BFFD0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005BFFD0() noexcept {
}
// VA 0x005C0070; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005C0070() noexcept {
}
// VA 0x005C22A0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005C22A0() noexcept {
}
// VA 0x005C22B0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005C22B0() noexcept {
}
// VA 0x005C2BF0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005C2BF0() noexcept {
}
// VA 0x005C3520; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005C3520() noexcept {
}
// VA 0x005C3810; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005C3810() noexcept {
}
// VA 0x005C3D40; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005C3D40() noexcept {
}
// VA 0x005C44E0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005C44E0() noexcept {
}
// VA 0x005C4520; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005C4520() noexcept {
}
// VA 0x005C47E0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005C47E0() noexcept {
}
// VA 0x005C48A0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005C48A0() noexcept {
}
// VA 0x005C4A40; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005C4A40() noexcept {
}
// VA 0x005CECE0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005CECE0() noexcept {
}
// VA 0x005CECF0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005CECF0() noexcept {
}
// VA 0x005CF9D0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005CF9D0() noexcept {
}
// VA 0x005CF9E0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005CF9E0() noexcept {
}
// VA 0x005CFA20; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_005CFA20(u32, u32) noexcept {
}
// VA 0x005D2290; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005D2290() noexcept {
}
// VA 0x005D2340; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005D2340() noexcept {
}
// VA 0x005D68A0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005D68A0() noexcept {
}
// VA 0x005D6920; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005D6920() noexcept {
}
// VA 0x005D6930; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_005D6930(u32) noexcept {
}
// VA 0x005D6940; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005D6940() noexcept {
}
// VA 0x005D6950; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_005D6950(u32) noexcept {
}
// VA 0x005D6960; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_005D6960(u32) noexcept {
}
// VA 0x005D6970; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005D6970() noexcept {
}
// VA 0x005D7570; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005D7570() noexcept {
}
// VA 0x005D7580; 11 original bytes; store_imm32.
void RE5_SCALAR_CC FUN_005D7580(void* self) noexcept {
    detail::store32(self,2780U,0x00000000U);
}
// VA 0x005D7590; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005D7590() noexcept {
}
// VA 0x005D75F0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005D75F0() noexcept {
}
// VA 0x005D7600; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_005D7600(u32) noexcept {
}
// VA 0x005D7620; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005D7620() noexcept {
}
// VA 0x005D77F0; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_005D77F0(u32) noexcept {
}
// VA 0x005D7800; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005D7800() noexcept {
}
// VA 0x005D7810; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005D7810() noexcept {
}
// VA 0x005D7CC0; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_005D7CC0(u32) noexcept {
}
// VA 0x005D7CE0; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_005D7CE0(u32) noexcept {
}
// VA 0x005DE880; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005DE880() noexcept {
}
// VA 0x005DEC80; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005DEC80() noexcept {
}
// VA 0x005DED10; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005DED10() noexcept {
}
// VA 0x005DEE10; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_005DEE10(u32) noexcept {
}
// VA 0x005DEE20; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005DEE20() noexcept {
}
// VA 0x005EB200; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_005EB200(const void* self) noexcept {
    return static_cast<const u8*>(self)[896U];
}
// VA 0x005EB210; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_005EB210(const void* self) noexcept {
    return static_cast<const u8*>(self)[897U];
}
// VA 0x005EB220; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_005EB220(const void* self) noexcept {
    return static_cast<const u8*>(self)[898U];
}
// VA 0x005EB230; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005EB230() noexcept {
}
// VA 0x005EB7B0; 13 original bytes; store_arg8.
void RE5_SCALAR_CC FUN_005EB7B0(void* self, u8 value) noexcept {
    detail::store8(self,897U,value);
}
// VA 0x005EB7C0; 13 original bytes; store_arg8.
void RE5_SCALAR_CC FUN_005EB7C0(void* self, u8 value) noexcept {
    detail::store8(self,898U,value);
}
// VA 0x005ED7D0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005ED7D0() noexcept {
}
// VA 0x005ED9A0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005ED9A0() noexcept {
}
// VA 0x005EDA00; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_005EDA00(u32) noexcept {
}
// VA 0x005EF300; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005EF300() noexcept {
}
// VA 0x005EFA20; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005EFA20() noexcept {
}
// VA 0x005EFA30; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005EFA30() noexcept {
}
// VA 0x005F0D50; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005F0D50() noexcept {
}
// VA 0x005F0E60; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005F0E60() noexcept {
}
// VA 0x005F0F70; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005F0F70() noexcept {
}
// VA 0x005F10D0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005F10D0() noexcept {
}
// VA 0x005F11D0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005F11D0() noexcept {
}
// VA 0x005F1380; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_005F1380(u32, u32) noexcept {
}
// VA 0x005F1520; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005F1520() noexcept {
}
// VA 0x005F1640; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_005F1640(u32) noexcept {
}
// VA 0x005F86F0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005F86F0() noexcept {
}
// VA 0x005F8700; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005F8700() noexcept {
}
// VA 0x005F8B30; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_005F8B30(const void* self) noexcept {
    return static_cast<const u8*>(self)[872U];
}
// VA 0x005FF220; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005FF220() noexcept {
}
// VA 0x005FF240; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_005FF240(u32) noexcept {
}
// VA 0x005FF390; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005FF390() noexcept {
}
// VA 0x005FF490; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005FF490() noexcept {
}
// VA 0x005FF500; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005FF500() noexcept {
}
// VA 0x005FF770; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_005FF770() noexcept {
}
// VA 0x00601F50; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_00601F50(const void* self) noexcept {
    return static_cast<const u8*>(self)[949U];
}
// VA 0x00601F60; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_00601F60(const void* self) noexcept {
    return static_cast<const u8*>(self)[950U];
}
// VA 0x00601F80; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00601F80() noexcept {
}
// VA 0x00601F90; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00601F90() noexcept {
}
// VA 0x00601FA0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00601FA0() noexcept {
}
// VA 0x00601FB0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00601FB0() noexcept {
}
// VA 0x00601FC0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00601FC0() noexcept {
}
// VA 0x00602000; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00602000() noexcept {
}
// VA 0x00602010; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00602010() noexcept {
}
// VA 0x00602020; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00602020() noexcept {
}
// VA 0x00602030; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00602030() noexcept {
}
// VA 0x006094D0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_006094D0(const void* self) noexcept {
    return static_cast<const u8*>(self)[1200U];
}
// VA 0x006094E0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_006094E0(const void* self) noexcept {
    return static_cast<const u8*>(self)[1201U];
}
// VA 0x006094F0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_006094F0(const void* self) noexcept {
    return static_cast<const u8*>(self)[1203U];
}
// VA 0x00609500; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_00609500(const void* self) noexcept {
    return static_cast<const u8*>(self)[1204U];
}
// VA 0x00609550; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00609550() noexcept {
}
// VA 0x0060A8A0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0060A8A0() noexcept {
}
// VA 0x0060A9B0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0060A9B0() noexcept {
}
// VA 0x0060AAF0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0060AAF0() noexcept {
}
// VA 0x0060ACF0; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_0060ACF0(u32, u32, u32, u32) noexcept {
}
// VA 0x0060AD40; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0060AD40() noexcept {
}
// VA 0x0060B4B0; 13 original bytes; store_arg32.
void RE5_SCALAR_CC FUN_0060B4B0(void* self, u32 value) noexcept {
    detail::store32(self,220U,value);
}
// VA 0x0060B4F0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0060B4F0() noexcept {
}
// VA 0x0060B620; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0060B620() noexcept {
}
// VA 0x0060CE90; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0060CE90() noexcept {
}
// VA 0x0060CF10; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0060CF10() noexcept {
}
// VA 0x0060D100; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0060D100() noexcept {
}
// VA 0x0060D180; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0060D180() noexcept {
}
// VA 0x0060D230; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0060D230() noexcept {
}
// VA 0x0060D2C0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0060D2C0() noexcept {
}
// VA 0x00615F10; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00615F10() noexcept {
}
// VA 0x00616F10; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00616F10() noexcept {
}
// VA 0x006175F0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_006175F0() noexcept {
}
// VA 0x00617640; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00617640() noexcept {
}
// VA 0x0061B070; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0061B070() noexcept {
}
// VA 0x0061B0D0; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_0061B0D0(u32) noexcept {
}
// VA 0x0061CBC0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0061CBC0() noexcept {
}
// VA 0x0061CCB0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0061CCB0() noexcept {
}
// VA 0x0061CED0; 8 original bytes; store_imm8.
void RE5_SCALAR_CC FUN_0061CED0(void* self) noexcept {
    detail::store8(self,284U,0x00000000U);
}
// VA 0x0061CFA0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0061CFA0() noexcept {
}
// VA 0x0061D000; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_0061D000(u32, u32) noexcept {
}
// VA 0x0061D050; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_0061D050(u32, u32, u32) noexcept {
}
// VA 0x0061E0B0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0061E0B0() noexcept {
}
// VA 0x0061E110; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0061E110() noexcept {
}
// VA 0x0061E740; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0061E740() noexcept {
}
// VA 0x0061E750; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0061E750() noexcept {
}
// VA 0x0061E760; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0061E760() noexcept {
}
// VA 0x0061E770; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0061E770() noexcept {
}
// VA 0x0061E780; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0061E780() noexcept {
}
// VA 0x0061E790; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0061E790() noexcept {
}
// VA 0x0061E7A0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_0061E7A0() noexcept {
}
// VA 0x00620C90; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_00620C90(const void* self) noexcept {
    return static_cast<const u8*>(self)[928U];
}
// VA 0x00620CA0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_00620CA0(const void* self) noexcept {
    return static_cast<const u8*>(self)[929U];
}
// VA 0x00620CB0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_00620CB0(const void* self) noexcept {
    return static_cast<const u8*>(self)[930U];
}
// VA 0x00620CC0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00620CC0() noexcept {
}
// VA 0x00620CD0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00620CD0() noexcept {
}
// VA 0x00620CE0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00620CE0() noexcept {
}
// VA 0x006212B0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_006212B0() noexcept {
}
// VA 0x006212C0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_006212C0() noexcept {
}
// VA 0x006212D0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_006212D0() noexcept {
}
// VA 0x006213C0; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_006213C0(u32, u32) noexcept {
}
// VA 0x006213F0; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_006213F0(u32) noexcept {
}
// VA 0x006238F0; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_006238F0() noexcept {
}
// VA 0x00624500; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00624500() noexcept {
}
// VA 0x00624510; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00624510() noexcept {
}
// VA 0x00624520; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_00624520() noexcept {
}
}
