#pragma once

#include <cstddef>

namespace re5::runtime {

[[nodiscard]] void* aligned_allocate(std::size_t size, std::size_t alignment) noexcept;
void aligned_release(void* ptr) noexcept;
void debug_output_a(const char* text) noexcept;

// Portable representation of the target's FindWindowW(class_name, nullptr) +
// SetWindowTextW(hwnd, title) side effect. On the Win32 target wchar_t is 16-bit.
void set_window_title_for_class(const char16_t* class_name, const char16_t* title) noexcept;

} // namespace re5::runtime
