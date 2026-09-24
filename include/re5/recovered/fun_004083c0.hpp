#pragma once

#include "re5/recovered/fun_004082e0.hpp"

#include <cstdint>

namespace re5::recovered {

struct FUN_004083C0_Services {
    void* context;
    bool (*grow)(
        void* context,
        FUN_004082E0_String& value,
        std::uint32_t required_length,
        std::uint32_t preserved_length) noexcept;
};

void FUN_004083C0_SetServices(
    const FUN_004083C0_Services* services) noexcept;

[[nodiscard]] FUN_004082E0_String* FUN_004083C0(
    FUN_004082E0_String& destination,
    const char* source,
    std::uint32_t count);

} // namespace re5::recovered
