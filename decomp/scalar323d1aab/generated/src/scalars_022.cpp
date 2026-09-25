#include "re5/scalar_detail.hpp"
namespace re5::scalar323d1aab {
// VA 0x00D90520; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_00D90520(const void* self) noexcept {
    return static_cast<const u8*>(self)[1556U];
}
// VA 0x00D90530; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_00D90530(const void* self) noexcept {
    return static_cast<const u8*>(self)[6512U];
}
// VA 0x00D9B920; 9 original bytes; bitfield8.
u8 RE5_SCALAR_CC FUN_00D9B920(const void* self) noexcept {
    return static_cast<u8>((detail::load32(self,32U) >> 2U) & 0x00000001U);
}
// VA 0x00D9B950; 9 original bytes; bitfield8.
u8 RE5_SCALAR_CC FUN_00D9B950(const void* self) noexcept {
    return static_cast<u8>((detail::load32(self,32U) >> 3U) & 0x00000001U);
}
// VA 0x00D9B980; 9 original bytes; bitfield8.
u8 RE5_SCALAR_CC FUN_00D9B980(const void* self) noexcept {
    return static_cast<u8>((detail::load32(self,32U) >> 4U) & 0x00000001U);
}
// VA 0x00D9B990; 9 original bytes; bitfield8.
u8 RE5_SCALAR_CC FUN_00D9B990(const void* self) noexcept {
    return static_cast<u8>((detail::load32(self,32U) >> 5U) & 0x00000001U);
}
// VA 0x00D9B9A0; 9 original bytes; bitfield8.
u8 RE5_SCALAR_CC FUN_00D9B9A0(const void* self) noexcept {
    return static_cast<u8>((detail::load32(self,32U) >> 6U) & 0x00000001U);
}
// VA 0x00D9B9B0; 9 original bytes; bitfield8.
u8 RE5_SCALAR_CC FUN_00D9B9B0(const void* self) noexcept {
    return static_cast<u8>((detail::load32(self,32U) >> 8U) & 0x00000001U);
}
// VA 0x00D9B9E0; 9 original bytes; bitfield8.
u8 RE5_SCALAR_CC FUN_00D9B9E0(const void* self) noexcept {
    return static_cast<u8>((detail::load32(self,32U) >> 9U) & 0x00000001U);
}
// VA 0x00D9BA10; 9 original bytes; bitfield8.
u8 RE5_SCALAR_CC FUN_00D9BA10(const void* self) noexcept {
    return static_cast<u8>((detail::load32(self,32U) >> 10U) & 0x00000001U);
}
// VA 0x00D9BA40; 9 original bytes; bitfield8.
u8 RE5_SCALAR_CC FUN_00D9BA40(const void* self) noexcept {
    return static_cast<u8>((detail::load32(self,32U) >> 11U) & 0x00000001U);
}
// VA 0x00DCC5B0; 12 original bytes; store_arg16.
void RE5_SCALAR_CC FUN_00DCC5B0(void* self, u16 value) noexcept {
    detail::store16(self,34U,value);
}
// VA 0x00DCF070; 13 original bytes; store_arg8.
void RE5_SCALAR_CC FUN_00DCF070(void* self, u8 value) noexcept {
    detail::store8(self,289216U,value);
}
// VA 0x00DCF080; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_00DCF080(const void* self) noexcept {
    return static_cast<const u8*>(self)[289216U];
}
// VA 0x00DCF0F0; 13 original bytes; bitfield32.
u32 RE5_SCALAR_CC FUN_00DCF0F0(const void* self) noexcept {
    return static_cast<u32>((detail::load32(self,292416U) >> 6U) & 0x00000001U);
}
// VA 0x00DCF100; 13 original bytes; bitfield32.
u32 RE5_SCALAR_CC FUN_00DCF100(const void* self) noexcept {
    return static_cast<u32>((detail::load32(self,292416U) >> 4U) & 0x00000001U);
}
// VA 0x00DCF110; 13 original bytes; bitfield32.
u32 RE5_SCALAR_CC FUN_00DCF110(const void* self) noexcept {
    return static_cast<u32>((detail::load32(self,292416U) >> 5U) & 0x00000001U);
}
// VA 0x00DCF120; 13 original bytes; bitfield32.
u32 RE5_SCALAR_CC FUN_00DCF120(const void* self) noexcept {
    return static_cast<u32>((detail::load32(self,292416U) >> 3U) & 0x00000001U);
}
// VA 0x00DCF130; 13 original bytes; bitfield32.
u32 RE5_SCALAR_CC FUN_00DCF130(const void* self) noexcept {
    return static_cast<u32>((detail::load32(self,292416U) >> 2U) & 0x00000001U);
}
// VA 0x00DCF160; 13 original bytes; bitfield32.
u32 RE5_SCALAR_CC FUN_00DCF160(const void* self) noexcept {
    return static_cast<u32>((detail::load32(self,292416U) >> 11U) & 0x00000001U);
}
// VA 0x00DCF170; 13 original bytes; bitfield32.
u32 RE5_SCALAR_CC FUN_00DCF170(const void* self) noexcept {
    return static_cast<u32>((detail::load32(self,292416U) >> 10U) & 0x00000001U);
}
// VA 0x00DCF180; 13 original bytes; bitfield32.
u32 RE5_SCALAR_CC FUN_00DCF180(const void* self) noexcept {
    return static_cast<u32>((detail::load32(self,292416U) >> 8U) & 0x00000001U);
}
// VA 0x00DCF190; 13 original bytes; bitfield32.
u32 RE5_SCALAR_CC FUN_00DCF190(const void* self) noexcept {
    return static_cast<u32>((detail::load32(self,292416U) >> 9U) & 0x00000001U);
}
// VA 0x00DD37D0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_00DD37D0(const void* self) noexcept {
    return static_cast<const u8*>(self)[1092U];
}
// VA 0x00DD3EE0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_00DD3EE0(const void* self) noexcept {
    return static_cast<const u8*>(self)[317736U];
}
// VA 0x00DD3EF0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_00DD3EF0(const void* self) noexcept {
    return static_cast<const u8*>(self)[317737U];
}
// VA 0x00DD4D40; 8 original bytes; store_imm32.
void RE5_SCALAR_CC FUN_00DD4D40(void* self) noexcept {
    detail::store32(self,4U,0x00000000U);
}
// VA 0x00DD7610; 13 original bytes; bitfield32.
u32 RE5_SCALAR_CC FUN_00DD7610(const void* self) noexcept {
    return static_cast<u32>((detail::load32(self,342060U) >> 2U) & 0x00000001U);
}
// VA 0x00DD7620; 13 original bytes; bitfield32.
u32 RE5_SCALAR_CC FUN_00DD7620(const void* self) noexcept {
    return static_cast<u32>((detail::load32(self,342060U) >> 3U) & 0x00000001U);
}
// VA 0x00DE8270; 10 original bytes; bitfield32.
u32 RE5_SCALAR_CC FUN_00DE8270(const void* self) noexcept {
    return static_cast<u32>((detail::load32(self,68U) >> 27U) & 0x00000007U);
}
// VA 0x00DE8280; 9 original bytes; bitfield8.
u8 RE5_SCALAR_CC FUN_00DE8280(const void* self) noexcept {
    return static_cast<u8>((detail::load32(self,68U) >> 30U) & 0x00000001U);
}
// VA 0x00DE8290; 9 original bytes; bitfield8.
u8 RE5_SCALAR_CC FUN_00DE8290(const void* self) noexcept {
    return static_cast<u8>((detail::load32(self,68U) >> 31U) & 0x00000001U);
}
// VA 0x00DE8300; 4 original bytes; load8.
u8 RE5_SCALAR_CC FUN_00DE8300(const void* self) noexcept {
    return static_cast<const u8*>(self)[92U];
}
// VA 0x00DE8490; 4 original bytes; load8.
u8 RE5_SCALAR_CC FUN_00DE8490(const void* self) noexcept {
    return static_cast<const u8*>(self)[52U];
}
// VA 0x00DE84D0; 4 original bytes; load8.
u8 RE5_SCALAR_CC FUN_00DE84D0(const void* self) noexcept {
    return static_cast<const u8*>(self)[72U];
}
// VA 0x00DE8860; 10 original bytes; bitfield32.
u32 RE5_SCALAR_CC FUN_00DE8860(const void* self) noexcept {
    return static_cast<u32>((detail::load32(self,24U) >> 8U) & 0x0000000FU);
}
// VA 0x00DE8890; 10 original bytes; bitfield32.
u32 RE5_SCALAR_CC FUN_00DE8890(const void* self) noexcept {
    return static_cast<u32>((detail::load32(self,24U) >> 12U) & 0x0000000FU);
}
// VA 0x00DE88E0; 10 original bytes; bitfield32.
u32 RE5_SCALAR_CC FUN_00DE88E0(const void* self) noexcept {
    return static_cast<u32>((detail::load32(self,32U) >> 4U) & 0x0000000FU);
}
// VA 0x00DE8920; 10 original bytes; bitfield32.
u32 RE5_SCALAR_CC FUN_00DE8920(const void* self) noexcept {
    return static_cast<u32>((detail::load32(self,40U) >> 4U) & 0x0000000FU);
}
// VA 0x00DE8930; 10 original bytes; bitfield32.
u32 RE5_SCALAR_CC FUN_00DE8930(const void* self) noexcept {
    return static_cast<u32>((detail::load32(self,40U) >> 8U) & 0x0000000FU);
}
// VA 0x00DE89D0; 10 original bytes; bitfield32.
u32 RE5_SCALAR_CC FUN_00DE89D0(const void* self) noexcept {
    return static_cast<u32>((detail::load32(self,72U) >> 8U) & 0x0000000FU);
}
// VA 0x00DE8CA0; 13 original bytes; bitfield32.
u32 RE5_SCALAR_CC FUN_00DE8CA0(const void* self) noexcept {
    return static_cast<u32>((detail::load32(self,180U) >> 20U) & 0x0000000FU);
}
// VA 0x00DE8D00; 13 original bytes; bitfield32.
u32 RE5_SCALAR_CC FUN_00DE8D00(const void* self) noexcept {
    return static_cast<u32>((detail::load32(self,184U) >> 4U) & 0x0000000FU);
}
// VA 0x00DE9560; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_00DE9560(const void* self) noexcept {
    return static_cast<const u8*>(self)[2720U];
}
// VA 0x00DFE180; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_00DFE180(const void* self) noexcept {
    return static_cast<const u8*>(self)[55893U];
}
// VA 0x00DFE190; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_00DFE190(const void* self) noexcept {
    return static_cast<const u8*>(self)[55894U];
}
// VA 0x00DFE1A0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_00DFE1A0(const void* self) noexcept {
    return static_cast<const u8*>(self)[55895U];
}
// VA 0x00DFE1B0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_00DFE1B0(const void* self) noexcept {
    return static_cast<const u8*>(self)[55896U];
}
// VA 0x00DFE1C0; 13 original bytes; store_arg32.
void RE5_SCALAR_CC FUN_00DFE1C0(void* self, u32 value) noexcept {
    detail::store32(self,55904U,value);
}
// VA 0x00DFE1D0; 13 original bytes; store_arg32.
void RE5_SCALAR_CC FUN_00DFE1D0(void* self, u32 value) noexcept {
    detail::store32(self,55908U,value);
}
// VA 0x00E00900; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_00E00900(const void* self) noexcept {
    return static_cast<const u8*>(self)[588U];
}
// VA 0x00E00910; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_00E00910(const void* self) noexcept {
    return static_cast<const u8*>(self)[4780U];
}
// VA 0x00E1DA10; 4 original bytes; load8.
u8 RE5_SCALAR_CC FUN_00E1DA10(const void* self) noexcept {
    return static_cast<const u8*>(self)[25U];
}
// VA 0x00E1DA20; 4 original bytes; load8.
u8 RE5_SCALAR_CC FUN_00E1DA20(const void* self) noexcept {
    return static_cast<const u8*>(self)[24U];
}
// VA 0x00E37CA0; 12 original bytes; store_arg16.
void RE5_SCALAR_CC FUN_00E37CA0(void* self, u16 value) noexcept {
    detail::store16(self,26U,value);
}
// VA 0x00E3B1F0; 11 original bytes; store_imm32.
void RE5_SCALAR_CC FUN_00E3B1F0(void* self) noexcept {
    detail::store32(self,136U,0x00000000U);
}
// VA 0x00E3B240; 11 original bytes; store_imm32.
void RE5_SCALAR_CC FUN_00E3B240(void* self) noexcept {
    detail::store32(self,232U,0x00000000U);
}
// VA 0x00E41DC0; 10 original bytes; store_arg8.
void RE5_SCALAR_CC FUN_00E41DC0(void* self, u8 value) noexcept {
    detail::store8(self,10U,value);
}
// VA 0x00E41DE0; 10 original bytes; bitfield32.
u32 RE5_SCALAR_CC FUN_00E41DE0(const void* self) noexcept {
    return static_cast<u32>((detail::load32(self,8U) >> 27U) & 0x00000001U);
}
// VA 0x00E41DF0; 10 original bytes; bitfield32.
u32 RE5_SCALAR_CC FUN_00E41DF0(const void* self) noexcept {
    return static_cast<u32>((detail::load32(self,8U) >> 28U) & 0x00000001U);
}
// VA 0x00E41EB0; 10 original bytes; store_arg8.
void RE5_SCALAR_CC FUN_00E41EB0(void* self, u8 value) noexcept {
    detail::store8(self,5U,value);
}
// VA 0x00E41EC0; 10 original bytes; store_arg8.
void RE5_SCALAR_CC FUN_00E41EC0(void* self, u8 value) noexcept {
    detail::store8(self,7U,value);
}
// VA 0x00EA1490; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_00EA1490(const void* self) noexcept {
    return static_cast<const u8*>(self)[272U];
}
// VA 0x00EA2DC0; 12 original bytes; bitfield8.
u8 RE5_SCALAR_CC FUN_00EA2DC0(const void* self) noexcept {
    return static_cast<u8>((detail::load32(self,224U) >> 2U) & 0x00000001U);
}
// VA 0x00EA2DD0; 12 original bytes; bitfield8.
u8 RE5_SCALAR_CC FUN_00EA2DD0(const void* self) noexcept {
    return static_cast<u8>((detail::load32(self,224U) >> 3U) & 0x00000001U);
}
// VA 0x00EA2E30; 12 original bytes; bitfield8.
u8 RE5_SCALAR_CC FUN_00EA2E30(const void* self) noexcept {
    return static_cast<u8>((detail::load32(self,224U) >> 8U) & 0x00000001U);
}
// VA 0x00EA2E70; 12 original bytes; bitfield8.
u8 RE5_SCALAR_CC FUN_00EA2E70(const void* self) noexcept {
    return static_cast<u8>((detail::load32(self,224U) >> 7U) & 0x00000001U);
}
// VA 0x00EA2EB0; 12 original bytes; bitfield8.
u8 RE5_SCALAR_CC FUN_00EA2EB0(const void* self) noexcept {
    return static_cast<u8>((detail::load32(self,224U) >> 10U) & 0x00000001U);
}
// VA 0x00EA2EC0; 12 original bytes; bitfield8.
u8 RE5_SCALAR_CC FUN_00EA2EC0(const void* self) noexcept {
    return static_cast<u8>((detail::load32(self,224U) >> 9U) & 0x00000001U);
}
// VA 0x00EA2F00; 12 original bytes; bitfield8.
u8 RE5_SCALAR_CC FUN_00EA2F00(const void* self) noexcept {
    return static_cast<u8>((detail::load32(self,224U) >> 11U) & 0x00000001U);
}
// VA 0x00EA2F10; 12 original bytes; bitfield8.
u8 RE5_SCALAR_CC FUN_00EA2F10(const void* self) noexcept {
    return static_cast<u8>((detail::load32(self,224U) >> 12U) & 0x00000001U);
}
// VA 0x00EA2F50; 12 original bytes; bitfield8.
u8 RE5_SCALAR_CC FUN_00EA2F50(const void* self) noexcept {
    return static_cast<u8>((detail::load32(self,224U) >> 13U) & 0x00000001U);
}
// VA 0x00EA2F60; 12 original bytes; bitfield8.
u8 RE5_SCALAR_CC FUN_00EA2F60(const void* self) noexcept {
    return static_cast<u8>((detail::load32(self,224U) >> 6U) & 0x00000001U);
}
// VA 0x00EA2FB0; 13 original bytes; store_arg8.
void RE5_SCALAR_CC FUN_00EA2FB0(void* self, u8 value) noexcept {
    detail::store8(self,272U,value);
}
// VA 0x00EA2FD0; 13 original bytes; store_arg8.
void RE5_SCALAR_CC FUN_00EA2FD0(void* self, u8 value) noexcept {
    detail::store8(self,273U,value);
}
// VA 0x00EA3230; 13 original bytes; bitfield32.
u32 RE5_SCALAR_CC FUN_00EA3230(const void* self) noexcept {
    return static_cast<u32>((detail::load32(self,460U) >> 4U) & 0x0000000FU);
}
// VA 0x00EA3270; 13 original bytes; store_arg8.
void RE5_SCALAR_CC FUN_00EA3270(void* self, u8 value) noexcept {
    detail::store8(self,461U,value);
}
// VA 0x00F59F00; 13 original bytes; store_arg32.
void RE5_SCALAR_CC FUN_00F59F00(void* self, u32 value) noexcept {
    detail::store32(self,128U,value);
}
// VA 0x00F59F80; 11 original bytes; store_imm32.
void RE5_SCALAR_CC FUN_00F59F80(void* self) noexcept {
    detail::store32(self,168U,0x00000000U);
}
// VA 0x00F70CF0; 13 original bytes; store_arg32.
void RE5_SCALAR_CC FUN_00F70CF0(void* self, u32 value) noexcept {
    detail::store32(self,148U,value);
}
// VA 0x00F72730; 12 original bytes; bitfield8.
u8 RE5_SCALAR_CC FUN_00F72730(const void* self) noexcept {
    return static_cast<u8>((detail::load32(self,580U) >> 2U) & 0x00000001U);
}
// VA 0x00F75C80; 10 original bytes; store_arg32.
void RE5_SCALAR_CC FUN_00F75C80(void* self, u32 value) noexcept {
    detail::store32(self,48U,value);
}
// VA 0x00F75C90; 10 original bytes; store_arg32.
void RE5_SCALAR_CC FUN_00F75C90(void* self, u32 value) noexcept {
    detail::store32(self,52U,value);
}
// VA 0x00F79610; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_00F79610(const void* self) noexcept {
    return static_cast<const u8*>(self)[564U];
}
// VA 0x00F79620; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_00F79620(const void* self) noexcept {
    return static_cast<const u8*>(self)[565U];
}
// VA 0x00F897F0; 13 original bytes; store_arg32.
void RE5_SCALAR_CC FUN_00F897F0(void* self, u32 value) noexcept {
    detail::store32(self,424U,value);
}
// VA 0x00F8A4E0; 11 original bytes; store_imm32.
void RE5_SCALAR_CC FUN_00F8A4E0(void* self) noexcept {
    detail::store32(self,864U,0x00000000U);
}
// VA 0x00F9EC90; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_00F9EC90(const void* self) noexcept {
    return static_cast<const u8*>(self)[162U];
}
// VA 0x00FA3920; 13 original bytes; store_arg8.
void RE5_SCALAR_CC FUN_00FA3920(void* self, u8 value) noexcept {
    detail::store8(self,3441U,value);
}
// VA 0x00FA3930; 13 original bytes; bitfield32.
u32 RE5_SCALAR_CC FUN_00FA3930(const void* self) noexcept {
    return static_cast<u32>((detail::load32(self,3440U) >> 5U) & 0x00000007U);
}
// VA 0x00FA3A00; 13 original bytes; store_arg8.
void RE5_SCALAR_CC FUN_00FA3A00(void* self, u8 value) noexcept {
    detail::store8(self,193U,value);
}
// VA 0x00FA8FC0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_00FA8FC0(const void* self) noexcept {
    return static_cast<const u8*>(self)[438U];
}
// VA 0x00FA9160; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_00FA9160(const void* self) noexcept {
    return static_cast<const u8*>(self)[439U];
}
// VA 0x00FBDAA0; 10 original bytes; store_arg8.
void RE5_SCALAR_CC FUN_00FBDAA0(void* self, u8 value) noexcept {
    detail::store8(self,24U,value);
}
// VA 0x00FCE860; 10 original bytes; store_arg32.
void RE5_SCALAR_CC FUN_00FCE860(void* self, u32 value) noexcept {
    detail::store32(self,68U,value);
}
// VA 0x00FE8280; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_00FE8280(const void* self) noexcept {
    return static_cast<const u8*>(self)[4820U];
}
// VA 0x0100EA90; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_0100EA90(const void* self) noexcept {
    return static_cast<const u8*>(self)[1028U];
}
// VA 0x0100FDC0; 10 original bytes; store_arg32.
void RE5_SCALAR_CC FUN_0100FDC0(void* self, u32 value) noexcept {
    detail::store32(self,92U,value);
}
// VA 0x01014800; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_01014800(const void* self) noexcept {
    return static_cast<const u8*>(self)[128U];
}
// VA 0x01014810; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_01014810(const void* self) noexcept {
    return static_cast<const u8*>(self)[130U];
}
// VA 0x01020AF0; 13 original bytes; store_arg8.
void RE5_SCALAR_CC FUN_01020AF0(void* self, u8 value) noexcept {
    detail::store8(self,220U,value);
}
// VA 0x01020B00; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_01020B00(const void* self) noexcept {
    return static_cast<const u8*>(self)[220U];
}
// VA 0x010295C0; 12 original bytes; bitfield8.
u8 RE5_SCALAR_CC FUN_010295C0(const void* self) noexcept {
    return static_cast<u8>((detail::load32(self,940U) >> 18U) & 0x00000001U);
}
// VA 0x01029600; 12 original bytes; bitfield8.
u8 RE5_SCALAR_CC FUN_01029600(const void* self) noexcept {
    return static_cast<u8>((detail::load32(self,940U) >> 19U) & 0x00000001U);
}
// VA 0x01029640; 12 original bytes; bitfield8.
u8 RE5_SCALAR_CC FUN_01029640(const void* self) noexcept {
    return static_cast<u8>((detail::load32(self,940U) >> 20U) & 0x00000001U);
}
// VA 0x01029680; 12 original bytes; bitfield8.
u8 RE5_SCALAR_CC FUN_01029680(const void* self) noexcept {
    return static_cast<u8>((detail::load32(self,940U) >> 21U) & 0x00000001U);
}
// VA 0x010870C0; 12 original bytes; store_arg16.
void RE5_SCALAR_CC FUN_010870C0(void* self, u16 value) noexcept {
    detail::store16(self,10U,value);
}
// VA 0x0108AC70; 9 original bytes; bitfield8.
u8 RE5_SCALAR_CC FUN_0108AC70(const void* self) noexcept {
    return static_cast<u8>((detail::load32(self,36U) >> 3U) & 0x00000001U);
}
// VA 0x0108ACC0; 9 original bytes; bitfield8.
u8 RE5_SCALAR_CC FUN_0108ACC0(const void* self) noexcept {
    return static_cast<u8>((detail::load32(self,36U) >> 8U) & 0x00000001U);
}
// VA 0x0108AD60; 12 original bytes; store_arg16.
void RE5_SCALAR_CC FUN_0108AD60(void* self, u16 value) noexcept {
    detail::store16(self,62U,value);
}
// VA 0x0108ADC0; 12 original bytes; store_arg16.
void RE5_SCALAR_CC FUN_0108ADC0(void* self, u16 value) noexcept {
    detail::store16(self,100U,value);
}
// VA 0x0108AE70; 10 original bytes; store_arg32.
void RE5_SCALAR_CC FUN_0108AE70(void* self, u32 value) noexcept {
    detail::store32(self,116U,value);
}
// VA 0x0108CDC0; 9 original bytes; bitfield8.
u8 RE5_SCALAR_CC FUN_0108CDC0(const void* self) noexcept {
    return static_cast<u8>((detail::load32(self,36U) >> 4U) & 0x00000001U);
}
// VA 0x01091F70; 12 original bytes; store_arg16.
void RE5_SCALAR_CC FUN_01091F70(void* self, u16 value) noexcept {
    detail::store16(self,36U,value);
}
// VA 0x01092030; 10 original bytes; store_arg8.
void RE5_SCALAR_CC FUN_01092030(void* self, u8 value) noexcept {
    detail::store8(self,60U,value);
}
// VA 0x01092050; 10 original bytes; store_arg8.
void RE5_SCALAR_CC FUN_01092050(void* self, u8 value) noexcept {
    detail::store8(self,61U,value);
}
// VA 0x01092070; 10 original bytes; store_arg8.
void RE5_SCALAR_CC FUN_01092070(void* self, u8 value) noexcept {
    detail::store8(self,62U,value);
}
// VA 0x01092090; 10 original bytes; store_arg8.
void RE5_SCALAR_CC FUN_01092090(void* self, u8 value) noexcept {
    detail::store8(self,76U,value);
}
// VA 0x010920B0; 10 original bytes; store_arg8.
void RE5_SCALAR_CC FUN_010920B0(void* self, u8 value) noexcept {
    detail::store8(self,77U,value);
}
// VA 0x010920D0; 10 original bytes; store_arg8.
void RE5_SCALAR_CC FUN_010920D0(void* self, u8 value) noexcept {
    detail::store8(self,78U,value);
}
// VA 0x010920F0; 10 original bytes; store_arg8.
void RE5_SCALAR_CC FUN_010920F0(void* self, u8 value) noexcept {
    detail::store8(self,79U,value);
}
// VA 0x01092110; 12 original bytes; store_arg16.
void RE5_SCALAR_CC FUN_01092110(void* self, u16 value) noexcept {
    detail::store16(self,80U,value);
}
// VA 0x01092130; 12 original bytes; store_arg16.
void RE5_SCALAR_CC FUN_01092130(void* self, u16 value) noexcept {
    detail::store16(self,82U,value);
}
// VA 0x01092150; 12 original bytes; store_arg16.
void RE5_SCALAR_CC FUN_01092150(void* self, u16 value) noexcept {
    detail::store16(self,84U,value);
}
// VA 0x01092170; 12 original bytes; store_arg16.
void RE5_SCALAR_CC FUN_01092170(void* self, u16 value) noexcept {
    detail::store16(self,86U,value);
}
// VA 0x01092190; 12 original bytes; store_arg16.
void RE5_SCALAR_CC FUN_01092190(void* self, u16 value) noexcept {
    detail::store16(self,88U,value);
}
// VA 0x010921B0; 12 original bytes; store_arg16.
void RE5_SCALAR_CC FUN_010921B0(void* self, u16 value) noexcept {
    detail::store16(self,90U,value);
}
// VA 0x01098540; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_01098540(const void* self) noexcept {
    return static_cast<const u8*>(self)[880U];
}
// VA 0x01098550; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_01098550(const void* self) noexcept {
    return static_cast<const u8*>(self)[881U];
}
// VA 0x0109C410; 8 original bytes; store_imm8.
void RE5_SCALAR_CC FUN_0109C410(void* self) noexcept {
    detail::store8(self,665U,0x00000001U);
}
// VA 0x010B6B30; 13 original bytes; store_arg32.
void RE5_SCALAR_CC FUN_010B6B30(void* self, u32 value) noexcept {
    detail::store32(self,1028U,value);
}
// VA 0x010B6B50; 13 original bytes; store_arg32.
void RE5_SCALAR_CC FUN_010B6B50(void* self, u32 value) noexcept {
    detail::store32(self,1032U,value);
}
// VA 0x010B6B70; 13 original bytes; store_arg32.
void RE5_SCALAR_CC FUN_010B6B70(void* self, u32 value) noexcept {
    detail::store32(self,1036U,value);
}
// VA 0x010B6B90; 13 original bytes; store_arg32.
void RE5_SCALAR_CC FUN_010B6B90(void* self, u32 value) noexcept {
    detail::store32(self,1040U,value);
}
// VA 0x010B6BB0; 13 original bytes; store_arg32.
void RE5_SCALAR_CC FUN_010B6BB0(void* self, u32 value) noexcept {
    detail::store32(self,1044U,value);
}
// VA 0x010C0D80; 8 original bytes; store_imm8.
void RE5_SCALAR_CC FUN_010C0D80(void* self) noexcept {
    detail::store8(self,929U,0x00000001U);
}
// VA 0x010D2CC0; 4 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D2CC0(const void* self) noexcept {
    return static_cast<const u8*>(self)[9U];
}
// VA 0x010D2D10; 4 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D2D10(const void* self) noexcept {
    return static_cast<const u8*>(self)[11U];
}
// VA 0x010D2D40; 4 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D2D40(const void* self) noexcept {
    return static_cast<const u8*>(self)[12U];
}
// VA 0x010D2D90; 4 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D2D90(const void* self) noexcept {
    return static_cast<const u8*>(self)[17U];
}
// VA 0x010D2DC0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D2DC0(const void* self) noexcept {
    return static_cast<const u8*>(self)[240U];
}
// VA 0x010D2DD0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D2DD0(const void* self) noexcept {
    return static_cast<const u8*>(self)[241U];
}
// VA 0x010D2DE0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D2DE0(const void* self) noexcept {
    return static_cast<const u8*>(self)[242U];
}
// VA 0x010D2DF0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D2DF0(const void* self) noexcept {
    return static_cast<const u8*>(self)[243U];
}
// VA 0x010D2E00; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D2E00(const void* self) noexcept {
    return static_cast<const u8*>(self)[244U];
}
// VA 0x010D2E20; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D2E20(const void* self) noexcept {
    return static_cast<const u8*>(self)[273U];
}
// VA 0x010D2E30; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D2E30(const void* self) noexcept {
    return static_cast<const u8*>(self)[280U];
}
// VA 0x010D2E40; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D2E40(const void* self) noexcept {
    return static_cast<const u8*>(self)[281U];
}
// VA 0x010D2E50; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D2E50(const void* self) noexcept {
    return static_cast<const u8*>(self)[368U];
}
// VA 0x010D2E60; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D2E60(const void* self) noexcept {
    return static_cast<const u8*>(self)[369U];
}
// VA 0x010D2EC0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D2EC0(const void* self) noexcept {
    return static_cast<const u8*>(self)[433U];
}
// VA 0x010D2ED0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D2ED0(const void* self) noexcept {
    return static_cast<const u8*>(self)[434U];
}
// VA 0x010D2EE0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D2EE0(const void* self) noexcept {
    return static_cast<const u8*>(self)[435U];
}
// VA 0x010D2F00; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D2F00(const void* self) noexcept {
    return static_cast<const u8*>(self)[436U];
}
// VA 0x010D2F10; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D2F10(const void* self) noexcept {
    return static_cast<const u8*>(self)[444U];
}
// VA 0x010D2F20; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D2F20(const void* self) noexcept {
    return static_cast<const u8*>(self)[445U];
}
// VA 0x010D2F30; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D2F30(const void* self) noexcept {
    return static_cast<const u8*>(self)[520U];
}
// VA 0x010D2F40; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D2F40(const void* self) noexcept {
    return static_cast<const u8*>(self)[524U];
}
// VA 0x010D2F90; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D2F90(const void* self) noexcept {
    return static_cast<const u8*>(self)[544U];
}
// VA 0x010D3010; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3010(const void* self) noexcept {
    return static_cast<const u8*>(self)[708U];
}
// VA 0x010D3020; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3020(const void* self) noexcept {
    return static_cast<const u8*>(self)[709U];
}
// VA 0x010D3040; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3040(const void* self) noexcept {
    return static_cast<const u8*>(self)[964U];
}
// VA 0x010D3050; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3050(const void* self) noexcept {
    return static_cast<const u8*>(self)[965U];
}
// VA 0x010D3060; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3060(const void* self) noexcept {
    return static_cast<const u8*>(self)[966U];
}
// VA 0x010D3070; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3070(const void* self) noexcept {
    return static_cast<const u8*>(self)[967U];
}
// VA 0x010D3080; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3080(const void* self) noexcept {
    return static_cast<const u8*>(self)[968U];
}
// VA 0x010D3090; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3090(const void* self) noexcept {
    return static_cast<const u8*>(self)[969U];
}
// VA 0x010D30A0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D30A0(const void* self) noexcept {
    return static_cast<const u8*>(self)[1084U];
}
// VA 0x010D3110; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3110(const void* self) noexcept {
    return static_cast<const u8*>(self)[1592U];
}
// VA 0x010D3130; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3130(const void* self) noexcept {
    return static_cast<const u8*>(self)[1324U];
}
// VA 0x010D3140; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3140(const void* self) noexcept {
    return static_cast<const u8*>(self)[1325U];
}
// VA 0x010D3190; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3190(const void* self) noexcept {
    return static_cast<const u8*>(self)[1812U];
}
// VA 0x010D31A0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D31A0(const void* self) noexcept {
    return static_cast<const u8*>(self)[1813U];
}
// VA 0x010D3240; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3240(const void* self) noexcept {
    return static_cast<const u8*>(self)[1908U];
}
// VA 0x010D32B0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D32B0(const void* self) noexcept {
    return static_cast<const u8*>(self)[1914U];
}
// VA 0x010D36B0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D36B0(const void* self) noexcept {
    return static_cast<const u8*>(self)[2108U];
}
// VA 0x010D36F0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D36F0(const void* self) noexcept {
    return static_cast<const u8*>(self)[2420U];
}
// VA 0x010D3700; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3700(const void* self) noexcept {
    return static_cast<const u8*>(self)[2421U];
}
// VA 0x010D3750; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3750(const void* self) noexcept {
    return static_cast<const u8*>(self)[2688U];
}
// VA 0x010D3760; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3760(const void* self) noexcept {
    return static_cast<const u8*>(self)[2689U];
}
// VA 0x010D3770; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3770(const void* self) noexcept {
    return static_cast<const u8*>(self)[2852U];
}
// VA 0x010D37E0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D37E0(const void* self) noexcept {
    return static_cast<const u8*>(self)[3284U];
}
// VA 0x010D37F0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D37F0(const void* self) noexcept {
    return static_cast<const u8*>(self)[3285U];
}
// VA 0x010D3820; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3820(const void* self) noexcept {
    return static_cast<const u8*>(self)[3308U];
}
// VA 0x010D3830; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3830(const void* self) noexcept {
    return static_cast<const u8*>(self)[3309U];
}
// VA 0x010D3880; 4 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3880(const void* self) noexcept {
    return static_cast<const u8*>(self)[112U];
}
// VA 0x010D38A0; 4 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D38A0(const void* self) noexcept {
    return static_cast<const u8*>(self)[20U];
}
// VA 0x010D38C0; 4 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D38C0(const void* self) noexcept {
    return static_cast<const u8*>(self)[21U];
}
// VA 0x010D3BF0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3BF0(const void* self) noexcept {
    return static_cast<const u8*>(self)[49376U];
}
// VA 0x010D3C00; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3C00(const void* self) noexcept {
    return static_cast<const u8*>(self)[49377U];
}
// VA 0x010D3C10; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3C10(const void* self) noexcept {
    return static_cast<const u8*>(self)[49404U];
}
// VA 0x010D3C20; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3C20(const void* self) noexcept {
    return static_cast<const u8*>(self)[49405U];
}
// VA 0x010D3C40; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3C40(const void* self) noexcept {
    return static_cast<const u8*>(self)[49412U];
}
// VA 0x010D3C50; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3C50(const void* self) noexcept {
    return static_cast<const u8*>(self)[49413U];
}
// VA 0x010D3C60; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3C60(const void* self) noexcept {
    return static_cast<const u8*>(self)[49572U];
}
// VA 0x010D3C70; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3C70(const void* self) noexcept {
    return static_cast<const u8*>(self)[49573U];
}
// VA 0x010D3C90; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3C90(const void* self) noexcept {
    return static_cast<const u8*>(self)[49576U];
}
// VA 0x010D3CB0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3CB0(const void* self) noexcept {
    return static_cast<const u8*>(self)[49580U];
}
// VA 0x010D3D60; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3D60(const void* self) noexcept {
    return static_cast<const u8*>(self)[49940U];
}
// VA 0x010D3D70; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3D70(const void* self) noexcept {
    return static_cast<const u8*>(self)[49941U];
}
// VA 0x010D3DB0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3DB0(const void* self) noexcept {
    return static_cast<const u8*>(self)[50008U];
}
// VA 0x010D3E00; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3E00(const void* self) noexcept {
    return static_cast<const u8*>(self)[50320U];
}
// VA 0x010D3E10; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3E10(const void* self) noexcept {
    return static_cast<const u8*>(self)[50321U];
}
// VA 0x010D3E50; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3E50(const void* self) noexcept {
    return static_cast<const u8*>(self)[50388U];
}
// VA 0x010D3EB0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3EB0(const void* self) noexcept {
    return static_cast<const u8*>(self)[50696U];
}
// VA 0x010D3EC0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3EC0(const void* self) noexcept {
    return static_cast<const u8*>(self)[50697U];
}
// VA 0x010D3EF0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3EF0(const void* self) noexcept {
    return static_cast<const u8*>(self)[50720U];
}
// VA 0x010D3F00; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010D3F00(const void* self) noexcept {
    return static_cast<const u8*>(self)[50721U];
}
// VA 0x010EE500; 9 original bytes; bitfield8.
u8 RE5_SCALAR_CC FUN_010EE500(const void* self) noexcept {
    return static_cast<u8>((detail::load32(self,124U) >> 2U) & 0x00000001U);
}
// VA 0x010EE560; 9 original bytes; bitfield8.
u8 RE5_SCALAR_CC FUN_010EE560(const void* self) noexcept {
    return static_cast<u8>((detail::load32(self,124U) >> 8U) & 0x00000001U);
}
// VA 0x010EE5D0; 12 original bytes; bitfield8.
u8 RE5_SCALAR_CC FUN_010EE5D0(const void* self) noexcept {
    return static_cast<u8>((detail::load32(self,176U) >> 26U) & 0x00000001U);
}
// VA 0x010EE5F0; 13 original bytes; store_arg32.
void RE5_SCALAR_CC FUN_010EE5F0(void* self, u32 value) noexcept {
    detail::store32(self,168U,value);
}
// VA 0x010EE610; 13 original bytes; store_arg32.
void RE5_SCALAR_CC FUN_010EE610(void* self, u32 value) noexcept {
    detail::store32(self,172U,value);
}
// VA 0x010EE630; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010EE630(const void* self) noexcept {
    return static_cast<const u8*>(self)[193U];
}
// VA 0x010EE640; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010EE640(const void* self) noexcept {
    return static_cast<const u8*>(self)[176U];
}
// VA 0x010EE650; 13 original bytes; store_arg8.
void RE5_SCALAR_CC FUN_010EE650(void* self, u8 value) noexcept {
    detail::store8(self,176U,value);
}
// VA 0x010EE6A0; 9 original bytes; bitfield8.
u8 RE5_SCALAR_CC FUN_010EE6A0(const void* self) noexcept {
    return static_cast<u8>((detail::load32(self,124U) >> 9U) & 0x00000001U);
}
// VA 0x010EE6F0; 13 original bytes; store_arg8.
void RE5_SCALAR_CC FUN_010EE6F0(void* self, u8 value) noexcept {
    detail::store8(self,177U,value);
}
// VA 0x010EE700; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010EE700(const void* self) noexcept {
    return static_cast<const u8*>(self)[194U];
}
// VA 0x010EE710; 13 original bytes; store_arg8.
void RE5_SCALAR_CC FUN_010EE710(void* self, u8 value) noexcept {
    detail::store8(self,194U,value);
}
// VA 0x010EE720; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_010EE720(const void* self) noexcept {
    return static_cast<const u8*>(self)[195U];
}
// VA 0x010EE730; 13 original bytes; store_arg8.
void RE5_SCALAR_CC FUN_010EE730(void* self, u8 value) noexcept {
    detail::store8(self,195U,value);
}
// VA 0x01116E30; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_01116E30(u32) noexcept {
}
// VA 0x01129510; 10 original bytes; store_arg32.
void RE5_SCALAR_CC FUN_01129510(void* self, u32 value) noexcept {
    detail::store32(self,20U,value);
}
// VA 0x01129A90; 10 original bytes; store_arg32.
void RE5_SCALAR_CC FUN_01129A90(void* self, u32 value) noexcept {
    detail::store32(self,12U,value);
}
// VA 0x0112C960; 13 original bytes; store_arg32.
void RE5_SCALAR_CC FUN_0112C960(void* self, u32 value) noexcept {
    detail::store32(self,3672U,value);
}
// VA 0x0112CFA0; 13 original bytes; store_arg32.
void RE5_SCALAR_CC FUN_0112CFA0(void* self, u32 value) noexcept {
    detail::store32(self,848U,value);
}
// VA 0x0113ABD0; 8 original bytes; store_imm8.
void RE5_SCALAR_CC FUN_0113ABD0(void* self) noexcept {
    detail::store8(self,1921U,0x00000000U);
}
// VA 0x011474D0; 13 original bytes; store_arg32.
void RE5_SCALAR_CC FUN_011474D0(void* self, u32 value) noexcept {
    detail::store32(self,860U,value);
}
// VA 0x011492D0; 13 original bytes; store_arg32.
void RE5_SCALAR_CC FUN_011492D0(void* self, u32 value) noexcept {
    detail::store32(self,960U,value);
}
// VA 0x0114F930; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_0114F930(const void* self) noexcept {
    return static_cast<const u8*>(self)[168U];
}
// VA 0x01151DD0; 8 original bytes; store_imm8.
void RE5_SCALAR_CC FUN_01151DD0(void* self) noexcept {
    detail::store8(self,884U,0x00000001U);
}
// VA 0x01151DE0; 8 original bytes; store_imm8.
void RE5_SCALAR_CC FUN_01151DE0(void* self) noexcept {
    detail::store8(self,885U,0x00000001U);
}
// VA 0x01157030; 3 original bytes; noop.
void RE5_SCALAR_POP FUN_01157030(u32, u32, u32) noexcept {
}
// VA 0x0115CD70; 4 original bytes; load8.
u8 RE5_SCALAR_CC FUN_0115CD70(const void* self) noexcept {
    return static_cast<const u8*>(self)[48U];
}
// VA 0x01164400; 4 original bytes; load8.
u8 RE5_SCALAR_CC FUN_01164400(const void* self) noexcept {
    return static_cast<const u8*>(self)[100U];
}
// VA 0x01164410; 4 original bytes; load8.
u8 RE5_SCALAR_CC FUN_01164410(const void* self) noexcept {
    return static_cast<const u8*>(self)[101U];
}
// VA 0x01164420; 4 original bytes; load8.
u8 RE5_SCALAR_CC FUN_01164420(const void* self) noexcept {
    return static_cast<const u8*>(self)[102U];
}
// VA 0x01164430; 4 original bytes; load8.
u8 RE5_SCALAR_CC FUN_01164430(const void* self) noexcept {
    return static_cast<const u8*>(self)[103U];
}
// VA 0x01169730; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_01169730(const void* self) noexcept {
    return static_cast<const u8*>(self)[992U];
}
// VA 0x01169740; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_01169740(const void* self) noexcept {
    return static_cast<const u8*>(self)[993U];
}
// VA 0x01169750; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_01169750(const void* self) noexcept {
    return static_cast<const u8*>(self)[994U];
}
// VA 0x01169760; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_01169760(const void* self) noexcept {
    return static_cast<const u8*>(self)[995U];
}
// VA 0x011697A0; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_011697A0(const void* self) noexcept {
    return static_cast<const u8*>(self)[921U];
}
// VA 0x011699D0; 8 original bytes; store_imm8.
void RE5_SCALAR_CC FUN_011699D0(void* self) noexcept {
    detail::store8(self,1005U,0x00000001U);
}
// VA 0x011699E0; 8 original bytes; store_imm8.
void RE5_SCALAR_CC FUN_011699E0(void* self) noexcept {
    detail::store8(self,1007U,0x00000001U);
}
// VA 0x011699F0; 8 original bytes; store_imm8.
void RE5_SCALAR_CC FUN_011699F0(void* self) noexcept {
    detail::store8(self,1010U,0x00000001U);
}
// VA 0x01169A00; 8 original bytes; store_imm8.
void RE5_SCALAR_CC FUN_01169A00(void* self) noexcept {
    detail::store8(self,1011U,0x00000001U);
}
// VA 0x01183240; 8 original bytes; store_imm8.
void RE5_SCALAR_CC FUN_01183240(void* self) noexcept {
    detail::store8(self,901U,0x00000001U);
}
// VA 0x0118B160; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_0118B160(const void* self) noexcept {
    return static_cast<const u8*>(self)[920U];
}
// VA 0x0118B170; 7 original bytes; load8.
u8 RE5_SCALAR_CC FUN_0118B170(const void* self) noexcept {
    return static_cast<const u8*>(self)[948U];
}
// VA 0x01196410; 1 original bytes; noop.
void RE5_SCALAR_POP FUN_01196410() noexcept {
}
// VA 0x011A1650; 4 original bytes; load8.
u8 RE5_SCALAR_CC FUN_011A1650(const void* self) noexcept {
    return static_cast<const u8*>(self)[90U];
}
// VA 0x011D9B50; 13 original bytes; store_arg32.
void RE5_SCALAR_CC FUN_011D9B50(void* self, u32 value) noexcept {
    detail::store32(self,8772U,value);
}
// VA 0x011DAE50; 13 original bytes; store_arg32.
void RE5_SCALAR_CC FUN_011DAE50(void* self, u32 value) noexcept {
    detail::store32(self,8496U,value);
}
// VA 0x011F4D10; 7 original bytes; store_imm32.
void RE5_SCALAR_CC FUN_011F4D10(void* self) noexcept {
    detail::store32(self,0U,0x015400F0U);
}
}
