#pragma once

#include <cstddef>
#include <cstdint>

namespace re5::recovered {

struct FUN_00402560_Node {
    void* field_00;
    void* payload_04;
    FUN_00402560_Node* sibling_08;
    FUN_00402560_Node* child_0c;
    std::uint8_t reserved_10[0x0f];
    std::uint8_t flags_1f;
};

struct FUN_00402560_Context {
    FUN_00402560_Node** output_34;
    std::uint32_t output_count_38;
    void (*visit)(void* context, const FUN_00402560_Node* node, std::int32_t depth) noexcept;
    void* visit_context;
};

struct FUN_00402640_StringRef {
    const void* pointer;
};

struct FUN_00402640_Table {
    const char* const* names;
    const void* const* values;
    std::uint32_t count;
    const char* fallback_literal;
};

void FUN_00402640_SetTable(const FUN_00402640_Table* table) noexcept;
std::uint32_t FUN_00402560(FUN_00402560_Context& context, const FUN_00402560_Node* node, std::int32_t depth) noexcept;
void FUN_004025D0(FUN_00402560_Context& context, FUN_00402560_Node* node, std::int32_t depth) noexcept;
bool FUN_00402640(const FUN_00402640_StringRef& value) noexcept;

} // namespace re5::recovered
