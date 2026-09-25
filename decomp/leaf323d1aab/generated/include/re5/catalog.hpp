#pragma once
#include "re5/leaves.hpp"
namespace re5::leaf323d1aab {
enum class Kind : u8 { constant, zero, identity, address, load8, load16, load32, sign8, sign16 };
struct Entry { u32 va; u32 file_offset; u32 displacement; u32 immediate; u8 size; u8 encoding; u8 width; Kind kind; Function function; };
inline constexpr unsigned entry_count = 7171U;
extern const Entry entries[entry_count];
}
