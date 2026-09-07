#pragma once

#include <cstddef>
#include <cstdint>

namespace re5::recovered {

using FUN_004011B0_Callback = void (*)(void*) noexcept;

void FUN_00401000(void* ptr) noexcept;

// 0x00401010: formats the Dev build title into a 0x104-WCHAR destination.
// If apply_to_window is true, the target finds the "MTFramework" Win32 window
// class and applies the generated text with SetWindowTextW.
void FUN_00401010(char16_t* title_buffer, const char* product_name, bool apply_to_window) noexcept;

void FUN_004011B0(
    void* first,
    std::uint32_t stride,
    std::uint32_t count,
    FUN_004011B0_Callback callback) noexcept;

[[nodiscard]] std::uintptr_t FUN_004011F0() noexcept;
[[nodiscard]] void* FUN_00401200(std::size_t size) noexcept;
void FUN_00401210(std::uintptr_t unused, const char* text) noexcept;
[[nodiscard]] bool FUN_00401220() noexcept;
void FUN_00401230(std::uintptr_t unused) noexcept;
void FUN_00401240(std::uintptr_t unused) noexcept;

} // namespace re5::recovered
