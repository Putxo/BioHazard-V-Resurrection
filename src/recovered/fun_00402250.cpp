#include "re5/recovered/fun_00402250.hpp"

namespace re5::recovered {
namespace {
const FUN_00402250_Services* g_services = nullptr;
}

void FUN_00402250_SetServices(const FUN_00402250_Services* services) noexcept {
    g_services = services;
}

std::uint32_t FUN_00402250(void* self) noexcept {
    const auto* services = g_services;
    if (services == nullptr || services->virtual_slot_10 == nullptr) {
        return 0U;
    }

    const auto* object = services->virtual_slot_10(self);
    return object != nullptr ? object->value : 0U;
}

std::uintptr_t FUN_00402260() noexcept {
    return 0x01655B5CU;
}

void* FUN_00402270() noexcept {
    const auto* services = g_services;
    return services != nullptr ? services->tail_00402270_result : nullptr;
}

std::uintptr_t FUN_00402280() noexcept {
    return 0x016E1D3CU;
}

void* FUN_00402290(void* self) noexcept {
    auto* object = static_cast<FUN_00402250_Object*>(self);
    if (object != nullptr) {
        object->vtable = reinterpret_cast<void*>(0x01541F48U);
    }

    const auto* services = g_services;
    return services != nullptr ? services->tail_00402290_result : object;
}

FUN_004022A0_Bytes* FUN_004022A0(
    FUN_004022A0_Bytes& destination,
    std::uint8_t byte2,
    std::uint8_t byte1,
    std::uint8_t byte0,
    std::uint8_t byte3) noexcept {
    destination.b0 = byte0;
    destination.b1 = byte1;
    destination.b2 = byte2;
    destination.b3 = byte3;
    return &destination;
}

FUN_004022A0_Bytes* FUN_004022C0(FUN_004022A0_Bytes& destination) noexcept {
    destination = {};
    return &destination;
}

FUN_004022D0_Source* FUN_004022D0(FUN_004022D0_Source& destination, const FUN_004022D0_Source& source) noexcept {
    destination.pointer = source.pointer;
    if (destination.pointer != nullptr) {
        ++*destination.pointer;
    }
    return &destination;
}

} // namespace re5::recovered
