#pragma once

#include <cstddef>

namespace re5::runtime {

[[nodiscard]] void* aligned_allocate(std::size_t size, std::size_t alignment) noexcept;
void aligned_release(void* ptr) noexcept;
void debug_output_a(const char* text) noexcept;
void set_window_title_for_class(const char16_t* class_name, const char16_t* title) noexcept;

// Target import adapter for steam_api!SteamAPI_RunCallbacks.
void steam_run_callbacks() noexcept;

} // namespace re5::runtime
