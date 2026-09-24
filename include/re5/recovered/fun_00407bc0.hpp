#pragma once

#include <cstddef>
#include <cstdint>

namespace re5::recovered {

struct FUN_00407BC0_Key {
    std::uint32_t reserved_00;
    std::uint32_t value;
};
static_assert(offsetof(FUN_00407BC0_Key, value) == 0x04);

struct FUN_00407BC0_Node {
    std::uint32_t reserved_00;
    std::uint32_t value;
    std::uint32_t reserved_08;
    std::uint32_t reserved_0c;
    FUN_00407BC0_Node* next;
};
static_assert(offsetof(FUN_00407BC0_Node, value) == 0x04);
static_assert(offsetof(FUN_00407BC0_Node, next) == 0x10);

struct FUN_00407BC0_Object {
    std::uintptr_t opaque = 0U;
};

struct FUN_00407BC0_Services {
    void* context;
    FUN_00407BC0_Node* (*first_node)(
        void* context,
        FUN_00407BC0_Object* object) noexcept;
};

void FUN_00407BC0_SetServices(const FUN_00407BC0_Services* services) noexcept;

[[nodiscard]] bool FUN_00407BC0(
    FUN_00407BC0_Object& object,
    const FUN_00407BC0_Key& key) noexcept;

} // namespace re5::recovered
