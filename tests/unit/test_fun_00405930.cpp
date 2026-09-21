#include "re5/recovered/fun_00405930.hpp"

#include <cassert>
#include <cstdio>
#include <cstring>

namespace {
using Node = re5::recovered::FUN_00402560_Node;

struct State {
    Node** allocation = nullptr;
    std::uint32_t expected_slots = 0;
    std::uint32_t allocated_bytes = 0;
    std::uint32_t alignment = 0;
    unsigned visits = 0;
    unsigned reports = 0;
    unsigned allocations = 0;
    unsigned frees = 0;
    unsigned submits = 0;
    unsigned processed = 0;
    std::uint32_t last_count = 0;
    Node* processed_nodes[8]{};
    char last_message[0x100]{};
    bool fail_allocation = false;
};

void visit(void* context, const Node*, std::int32_t) noexcept {
    ++static_cast<State*>(context)->visits;
}

void report(void* context, std::uint32_t count) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.reports;
    state.last_count = count;
}

Node** allocate_table(void* context, std::uint32_t bytes, std::uint32_t alignment) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.allocations;
    state.allocated_bytes = bytes;
    state.alignment = alignment;
    if (state.fail_allocation) {
        return nullptr;
    }
    state.allocation = new Node*[state.expected_slots == 0U ? 1U : state.expected_slots]{};
    return state.allocation;
}

void free_table(void* context, Node** table) noexcept {
    auto& state = *static_cast<State*>(context);
    assert(table == state.allocation);
    ++state.frees;
    delete[] table;
    state.allocation = nullptr;
}

void submit(void* context, const char* message) noexcept {
    auto& state = *static_cast<State*>(context);
    ++state.submits;
    std::snprintf(state.last_message, sizeof(state.last_message), "%s", message);
}

void process(void* context, Node* node) noexcept {
    auto& state = *static_cast<State*>(context);
    state.processed_nodes[state.processed++] = node;
}

re5::recovered::FUN_00405930_Services make_services(State& state, Node& root, bool enumerate_all) {
    return {
        &state,
        &root,
        enumerate_all,
        visit,
        report,
        allocate_table,
        free_table,
        submit,
        process,
    };
}
} // namespace

void test_fun_00405930() {
    using namespace re5::recovered;

    Node child_b{};
    child_b.payload_04 = const_cast<char*>("child_b");
    Node child_a{};
    child_a.payload_04 = const_cast<char*>("child_a");
    child_a.sibling_08 = &child_b;
    Node root{};
    root.payload_04 = const_cast<char*>("root");
    root.child_0c = &child_a;

    State state{};
    state.expected_slots = 3U;
    auto services = make_services(state, root, true);
    FUN_00405930_SetServices(&services);

    FUN_00405930_Object object{};
    assert(!FUN_00405930(object));
    assert(object.state_2c == 1U);
    assert(object.class_count_30 == 3U);
    assert(object.index_38 == 0U);
    assert(state.visits == 3U);
    assert(state.reports == 1U);
    assert(state.last_count == 3U);
    assert(state.allocations == 1U);
    assert(state.allocated_bytes == 12U);
    assert(state.alignment == 0x10U);

    assert(!FUN_00405930(object));
    assert(object.state_2c == 2U);
    assert(object.current_4c == &child_a);
    assert(std::strcmp(state.last_message, "CONVERT 'child_a' ...") == 0);
    assert(!FUN_00405930(object));
    assert(object.state_2c == 1U);
    assert(object.index_38 == 1U);
    assert(state.processed_nodes[0] == &child_a);

    assert(!FUN_00405930(object));
    assert(!FUN_00405930(object));
    assert(!FUN_00405930(object));
    assert(!FUN_00405930(object));
    assert(FUN_00405930(object));
    assert(state.processed == 3U);
    assert(state.processed_nodes[1] == &child_b);
    assert(state.processed_nodes[2] == &root);
    assert(state.submits == 3U);
    assert(state.frees == 1U);
    assert(object.nodes_34 == nullptr);

    State collapsed{};
    collapsed.expected_slots = 3U;
    services = make_services(collapsed, root, false);
    FUN_00405930_SetServices(&services);
    FUN_00405930_Object root_only{};
    assert(!FUN_00405930(root_only));
    assert(root_only.class_count_30 == 1U);
    assert(root_only.nodes_34[0] == &root);
    assert(!FUN_00405930(root_only));
    assert(root_only.current_4c == &root);
    assert(std::strcmp(collapsed.last_message, "CONVERT 'root' ...") == 0);
    assert(!FUN_00405930(root_only));
    assert(collapsed.processed == 1U);
    assert(FUN_00405930(root_only));
    assert(collapsed.frees == 1U);

    State failed{};
    failed.expected_slots = 3U;
    failed.fail_allocation = true;
    services = make_services(failed, root, true);
    FUN_00405930_SetServices(&services);
    FUN_00405930_Object allocation_failure{};
    assert(FUN_00405930(allocation_failure));
    assert(allocation_failure.state_2c == 0U);
    assert(allocation_failure.nodes_34 == nullptr);
    assert(failed.reports == 1U);
    assert(failed.allocations == 1U);
    assert(failed.frees == 0U);

    State exhausted{};
    services = make_services(exhausted, root, true);
    FUN_00405930_SetServices(&services);
    exhausted.allocation = new Node*[1]{&root};
    FUN_00405930_Object done{};
    done.state_2c = 1U;
    done.class_count_30 = 0U;
    done.nodes_34 = exhausted.allocation;
    assert(FUN_00405930(done));
    assert(done.state_2c == 1U);
    assert(done.nodes_34 == nullptr);
    assert(exhausted.frees == 1U);

    FUN_00405930_Object unknown{};
    unknown.state_2c = 3U;
    assert(!FUN_00405930(unknown));
    assert(unknown.state_2c == 3U);
    assert(unknown.index_38 == 0U);

    FUN_00405930_SetServices(nullptr);
}
