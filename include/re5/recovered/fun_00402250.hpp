#pragma once

#include <cstdint>

namespace re5::recovered {

struct FUN_00402250_Object {
    void* vtable;
    std::uint32_t value;
};

struct FUN_004022A0_Bytes {
    std::uint8_t b0;
    std::uint8_t b1;
    std::uint8_t b2;
    std::uint8_t b3;
};

struct FUN_004022D0_Source {
    std::uint32_t* pointer;
};

struct FUN_00402250_Services {
    void* tail_00402270_result;
    void* tail_00402290_result;
    FUN_00402250_Object* (*virtual_slot_10)(void* self) noexcept;
};

void FUN_00402250_SetServices(const FUN_00402250_Services* services) noexcept;
std::uint32_t FUN_00402250(void* self) noexcept;
std::uintptr_t FUN_00402260() noexcept;
void* FUN_00402270() noexcept;
std::uintptr_t FUN_00402280() noexcept;
void* FUN_00402290(void* self) noexcept;
FUN_004022A0_Bytes* FUN_004022A0(FUN_004022A0_Bytes& destination, std::uint8_t byte2, std::uint8_t byte1, std::uint8_t byte0, std::uint8_t byte3) noexcept;
FUN_004022A0_Bytes* FUN_004022C0(FUN_004022A0_Bytes& destination) noexcept;
FUN_004022D0_Source* FUN_004022D0(FUN_004022D0_Source& destination, const FUN_004022D0_Source& source) noexcept;

} // namespace re5::recovered
