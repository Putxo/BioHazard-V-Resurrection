#pragma once

#include <cstddef>
#include <cstdint>

namespace re5::recovered {

using FUN_004011B0_Callback = void (*)(void*) noexcept;

// 0x00401000: tail-jump wrapper around the target CRT aligned-free routine.
void FUN_00401000(void* ptr) noexcept;

// 0x004011B0: apply an ECX-style callback to count elements separated by stride bytes.
void FUN_004011B0(
    void* first,
    std::uint32_t stride,
    std::uint32_t count,
    FUN_004011B0_Callback callback) noexcept;

// 0x004011F0: returns the original target address of a global/static object.
[[nodiscard]] std::uintptr_t FUN_004011F0() noexcept;

// 0x00401200: allocate size bytes with 16-byte alignment.
[[nodiscard]] void* FUN_00401200(std::size_t size) noexcept;

// 0x00401210: second argument is forwarded to OutputDebugStringA in the target.
void FUN_00401210(std::uintptr_t unused, const char* text) noexcept;

// 0x00401220: constant false return.
[[nodiscard]] bool FUN_00401220() noexcept;

// 0x00401230 / 0x00401240: one-argument no-op callbacks (target uses ret 4).
void FUN_00401230(std::uintptr_t unused) noexcept;
void FUN_00401240(std::uintptr_t unused) noexcept;

} // namespace re5::recovered
