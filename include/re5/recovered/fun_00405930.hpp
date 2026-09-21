#pragma once

#include "re5/recovered/fun_00402560.hpp"

#include <cstdint>

namespace re5::recovered {

struct FUN_00405930_Object {
    std::uint32_t state_2c = 0;
    std::uint32_t class_count_30 = 0;
    FUN_00402560_Node** nodes_34 = nullptr;
    std::uint32_t index_38 = 0;
    FUN_00402560_Node* current_4c = nullptr;
};

struct FUN_00405930_Services {
    void* context = nullptr;
    FUN_00402560_Node* root_01657b84 = nullptr;
    bool enumerate_all_01657b88 = false;
    void (*visit_counted_node)(void* context, const FUN_00402560_Node* node, std::int32_t depth) noexcept = nullptr;
    void (*report_class_count)(void* context, std::uint32_t count) noexcept = nullptr;
    FUN_00402560_Node** (*allocate_node_table)(void* context, std::uint32_t bytes, std::uint32_t alignment) noexcept = nullptr;
    void (*free_node_table)(void* context, FUN_00402560_Node** table) noexcept = nullptr;
    void (*submit_message)(void* context, const char* message) noexcept = nullptr;
    void (*process_request)(void* context, FUN_00402560_Node* node) noexcept = nullptr;
};

void FUN_00405930_SetServices(const FUN_00405930_Services* services) noexcept;
bool FUN_00405930(FUN_00405930_Object& object) noexcept;

} // namespace re5::recovered
