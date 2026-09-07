#pragma once

#include <cstddef>

namespace re5::runtime {

[[nodiscard]] void* aligned_allocate(std::size_t size, std::size_t alignment) noexcept;
void aligned_release(void* ptr) noexcept;
void debug_output_a(const char* text) noexcept;

} // namespace re5::runtime
