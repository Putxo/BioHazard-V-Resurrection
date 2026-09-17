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

struct FUN_00402770_Request {
    const char* text;
    std::uint32_t width_x16;
    std::uint32_t half_width_x16;
    std::uint32_t min_dimension;
    std::uint32_t max_dimension;
    std::uint32_t opcode;
    std::uintptr_t resource_0165201c;
    std::uintptr_t resource_01652e48;
};

struct FUN_00402770_Services {
    std::uintptr_t resource_0165201c;
    std::uintptr_t resource_01652e48;
    void* context;
    void (*submit)(void* context, const FUN_00402770_Request& request) noexcept;
};

struct FUN_00402DF0_Object {
    std::uint8_t reserved_00[0x54];
    char name_54[0x100];
};

struct FUN_00402DF0_Message {
    const char* formatted_name;
    const char* active_name;
    char text[0x100];
};

struct FUN_00402DF0_Services {
    const char* name_format;
    const char* message_format;
    const char* fallback_active_name;
    std::uintptr_t registry_016e216c;
    void* context;
    const char* (*active_name)(void* context) noexcept;
    void (*report)(void* context, const FUN_00402DF0_Message& message) noexcept;
    void* (*resolve)(void* context, std::uintptr_t registry, const char* name, bool create) noexcept;
    void (*activate)(void* context, void* resolved) noexcept;
};

void FUN_00402640_SetTable(const FUN_00402640_Table* table) noexcept;
void FUN_00402770_SetServices(const FUN_00402770_Services* services) noexcept;
void FUN_00402DF0_SetServices(const FUN_00402DF0_Services* services) noexcept;
std::uint32_t FUN_00402560(FUN_00402560_Context& context, const FUN_00402560_Node* node, std::int32_t depth) noexcept;
void FUN_004025D0(FUN_00402560_Context& context, FUN_00402560_Node* node, std::int32_t depth) noexcept;
bool FUN_00402640(const FUN_00402640_StringRef& value) noexcept;
const void* FUN_004026C0(const FUN_00402640_StringRef& value) noexcept;
void FUN_00402770(const char* text) noexcept;
void FUN_00402DF0(FUN_00402DF0_Object& object, std::int32_t selector) noexcept;

} // namespace re5::recovered
