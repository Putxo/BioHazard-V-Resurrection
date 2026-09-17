#include "re5/recovered/fun_00402b10.hpp"

#include <cassert>
#include <cstdint>

namespace {
bool accept_payload(void*, const re5::recovered::FUN_00402B10_Node& node) noexcept {
    return node.payload != nullptr;
}

re5::recovered::FUN_00402B10_Record make_record(
    void*,
    const re5::recovered::FUN_00402B10_Node& node) noexcept {
    return {node.key_04, node.payload, reinterpret_cast<const void*>(0x1234U), node.child_0c};
}
} // namespace

void test_fun_00402b10() {
    using namespace re5::recovered;

    FUN_00402B10_Table table{};
    table.count = 7U;
    FUN_00402B10_SetTable(&table);
    FUN_00402B10_Services services{"reset", accept_payload, make_record, nullptr};
    FUN_00402B10_SetServices(&services);

    int root_payload = 1;
    int child_payload = 2;
    int hidden_payload = 3;
    FUN_00402B10_Node child{"child", nullptr, nullptr, {}, 0U, &child_payload};
    FUN_00402B10_Node hidden{"hidden", nullptr, nullptr, {}, 1U, &hidden_payload};
    FUN_00402B10_Node reset{"reset", &hidden, nullptr, {}, 0U, &root_payload};
    FUN_00402B10_Node root{"root", nullptr, &child, {}, 0U, &root_payload};
    reset.sibling_08 = &root;

    FUN_00402B10(&reset);

    assert(table.count == 3U);
    assert(table.records[0].name == "reset");
    assert(table.records[0].value == &root_payload);
    assert(table.records[1].name == "child");
    assert(table.records[1].value == &child_payload);
    assert(table.records[2].name == "root");
    assert(table.records[2].value == &root_payload);

    table.count = static_cast<std::uint32_t>(table.records.size());
    FUN_00402B10(&root);
    assert(table.count == table.records.size());

    FUN_00402B10_SetServices(nullptr);
    FUN_00402B10_SetTable(nullptr);
}
