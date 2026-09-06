#include "re5/recovered/pilot_bb9070.hpp"

namespace re5::recovered {

// Original VA: 0x00BB9070
// RVA:         0x007B9070
// Section:     .text
// Original size: 3 bytes
// x86: mov eax,[ecx] ; ret
// Original ABI evidence: ECX carries this (MSVC x86 thiscall-style accessor).
// Status: FAST_PASS
// Confidence: HIGH (INT3 function boundary + 105 decoded direct CALL targets).
std::uint32_t FUN_00BB9070(const UnknownBB90Object* self) noexcept {
    return self->field_00;
}

// Original VA: 0x00BB9080
// RVA:         0x007B9080
// Section:     .text
// Original size: 4 bytes
// x86: mov eax,[ecx+4] ; ret
// Status: FAST_PASS
// Confidence: HIGH (INT3 function boundary + 20 decoded direct CALL targets).
std::uint32_t FUN_00BB9080(const UnknownBB90Object* self) noexcept {
    return self->field_04;
}

// Original VA: 0x00BB9090
// RVA:         0x007B9090
// Section:     .text
// Original size: 4 bytes
// x86: mov eax,[ecx+4] ; ret
// Status: FAST_PASS
// Confidence: HIGH (INT3 function boundary + 7 decoded direct CALL targets).
std::uint32_t FUN_00BB9090(const UnknownBB90Object* self) noexcept {
    return self->field_04;
}

} // namespace re5::recovered
