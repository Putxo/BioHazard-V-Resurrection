#pragma once

#include <cstdint>

namespace re5::recovered {

struct FUN_00402110_Dword {
    std::uint32_t value;
};

FUN_00402110_Dword* FUN_00402110(FUN_00402110_Dword& destination, const FUN_00402110_Dword& source) noexcept;

} // namespace re5::recovered
