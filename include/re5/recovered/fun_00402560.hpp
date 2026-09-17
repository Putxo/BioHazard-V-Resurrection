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

struct FUN_00402E70_Node {
    void* field_00;
    const char* name_04;
    FUN_00402E70_Node* next_08;
};

struct FUN_00402DF0_Object {
    std::uint8_t reserved_00[0x2c];
    std::uint32_t state_2c;
    std::uint8_t reserved_30[0x1c];
    FUN_00402E70_Node* current_4c;
    std::int32_t selected_50;
    char name_54[0x100];
    std::uint8_t reserved_154[0x14];
    const char* pending_name_168;
    std::uint8_t reserved_16c[0x04];
    std::uint8_t flags_170;
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

struct FUN_00402E70_Services {
    FUN_00402E70_Node* list_head_01657f20;
    const char* label_format;
    void* context;
    void (*release_first)(void* context, std::int32_t selected) noexcept;
    void (*release_second)(void* context, std::int32_t selected) noexcept;
    void (*submit_label)(void* context, const char* label, std::int32_t selected) noexcept;
};

struct FUN_00402FC0_Services {
    const char* initial_format;
    const char* pending_format;
    const char* pending_message_format;
    const char* fallback_name;
    std::uintptr_t registry_016e216c;
    void* context;
    const char* (*active_name_1028)(void* context) noexcept;
    const char* (*active_name_102c)(void* context) noexcept;
    bool (*transition_ready)(void* context, bool first_probe) noexcept;
    void (*submit_pending)(void* context, const char* label) noexcept;
    void (*report)(void* context, const FUN_00402DF0_Message& message) noexcept;
    void* (*resolve)(void* context, std::uintptr_t registry, const char* name, bool create) noexcept;
    void (*activate)(void* context, void* resolved) noexcept;
};

void FUN_00402640_SetTable(const FUN_00402640_Table* table) noexcept;
void FUN_00402770_SetServices(const FUN_00402770_Services* services) noexcept;
void FUN_00402DF0_SetServices(const FUN_00402DF0_Services* services) noexcept;
void FUN_00402E70_SetServices(const FUN_00402E70_Services* services) noexcept;
void FUN_00402FC0_SetServices(const FUN_00402FC0_Services* services) noexcept;
std::uint32_t FUN_00402560(FUN_00402560_Context& context, const FUN_00402560_Node* node, std::int32_t depth) noexcept;
void FUN_004025D0(FUN_00402560_Context& context, FUN_00402560_Node* node, std::int32_t depth) noexcept;
bool FUN_00402640(const FUN_00402640_StringRef& value) noexcept;
const void* FUN_004026C0(const FUN_00402640_StringRef& value) noexcept;
void FUN_00402770(const char* text) noexcept;
void FUN_00402DF0(FUN_00402DF0_Object& object, std::int32_t selector) noexcept;
bool FUN_00402E70(FUN_00402DF0_Object& object, std::int32_t filter) noexcept;
bool FUN_00402FC0(FUN_00402DF0_Object& object) noexcept;

} // namespace re5::recovered
