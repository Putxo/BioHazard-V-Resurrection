#include "re5/recovered/region_00401000.hpp"
#include "re5/runtime/host_runtime.hpp"

namespace re5::recovered {

// Original VA: 0x00401000
// RVA:         0x00001000
// x86: jmp 0x0125145C
// 0x0125145C is the target CRT aligned-free implementation.
void FUN_00401000(void* ptr) noexcept {
    re5::runtime::aligned_release(ptr);
}

// Original VA: 0x004011B0
// RVA:         0x000011B0
// Original ABI: callee pops four stack arguments (ret 0x10). The callback is
// invoked with the current element in ECX, then the pointer advances by stride.
void FUN_004011B0(
    void* first,
    std::uint32_t stride,
    std::uint32_t count,
    FUN_004011B0_Callback callback) noexcept {
    // Original code subtracts one first and tests the sign flag. Model that
    // with 32-bit modular arithmetic so count==0 exits without a callback.
    std::uint32_t counter = count - 1U;
    if ((counter & 0x80000000U) != 0U) {
        return;
    }

    auto cursor = reinterpret_cast<std::uintptr_t>(first);
    for (;;) {
        callback(reinterpret_cast<void*>(cursor));
        cursor += stride;
        counter -= 1U;
        if ((counter & 0x80000000U) != 0U) {
            break;
        }
    }
}

// Original VA: 0x004011F0
// RVA:         0x000011F0
// x86: mov eax,0x01652E00 ; ret
std::uintptr_t FUN_004011F0() noexcept {
    return 0x01652E00U;
}

// Original VA: 0x00401200
// RVA:         0x00001200
// x86 wrapper passes (size, 0x10) to 0x01251471, which forwards to the
// target aligned-allocation CRT routine with offset zero.
void* FUN_00401200(std::size_t size) noexcept {
    return re5::runtime::aligned_allocate(size, 16U);
}

// Original VA: 0x00401210
// RVA:         0x00001210
// x86 loads the second stack argument and calls IAT 0x01371120,
// identified as KERNEL32!OutputDebugStringA.
void FUN_00401210(std::uintptr_t /*unused*/, const char* text) noexcept {
    re5::runtime::debug_output_a(text);
}

// Original VA: 0x00401220
// RVA:         0x00001220
// x86: xor al,al ; ret
bool FUN_00401220() noexcept {
    return false;
}

// Original VAs: 0x00401230 / 0x00401240
// x86: ret 4
void FUN_00401230(std::uintptr_t /*unused*/) noexcept {}
void FUN_00401240(std::uintptr_t /*unused*/) noexcept {}

} // namespace re5::recovered
