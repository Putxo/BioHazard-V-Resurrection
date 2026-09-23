#include "re5/recovered/fun_00407af0.hpp"

namespace re5::recovered {
namespace {
const FUN_00407AF0_Services* g_services = nullptr;

constexpr std::uintptr_t k_bad_alloc_vtable = 0x01541F48U;
} // namespace

void FUN_00407AF0_SetServices(const FUN_00407AF0_Services* services) noexcept {
    g_services = services;
}

FUN_00407AF0_Object* FUN_00407AF0(
    FUN_00407AF0_Object& destination,
    const FUN_00407AF0_Object& source) noexcept {
    const auto* services = g_services;
    if (services == nullptr || services->copy_exception_base == nullptr) {
        return &destination;
    }

    services->copy_exception_base(
        services->context,
        &destination,
        &source);

    destination.vtable = k_bad_alloc_vtable;
    return &destination;
}

} // namespace re5::recovered
