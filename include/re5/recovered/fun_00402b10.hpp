#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace re5::recovered {

struct FUN_00402B10_Node {
    const char* key_04;
    FUN_00402B10_Node* sibling_08;
    FUN_00402B10_Node* child_0c;
    std::uint8_t reserved_10[0x0f];
    std::uint8_t flags_1f;
    void* payload;
};

struct FUN_00402B10_Record {
    const char* name;
    const void* value;
    const void* extra;
    const void* parent;
};

struct FUN_00402B10_Table {
    std::array<FUN_00402B10_Record, 0x200> records{};
    std::uint32_t count = 0;
};

struct FUN_00402B10_Services {
    const char* reset_key;
    bool (*accept)(void* context, const FUN_00402B10_Node& node) noexcept;
    FUN_00402B10_Record (*make_record)(void* context, const FUN_00402B10_Node& node) noexcept;
    void* context;
};

void FUN_00402B10_SetServices(const FUN_00402B10_Services* services) noexcept;
void FUN_00402B10_SetTable(FUN_00402B10_Table* table) noexcept;
void FUN_00402B10(FUN_00402B10_Node* node) noexcept;

} // namespace re5::recovered
