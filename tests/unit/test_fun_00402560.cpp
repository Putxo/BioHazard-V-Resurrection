#include "re5/recovered/fun_00402560.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace {
struct VisitState {
    std::array<const re5::recovered::FUN_00402560_Node*, 8> nodes{};
    std::array<int, 8> depths{};
    std::size_t count = 0;
};

void record_visit(
    void* context,
    const re5::recovered::FUN_00402560_Node* node,
    std::int32_t depth) noexcept {
    auto& state = *static_cast<VisitState*>(context);
    state.nodes[state.count] = node;
    state.depths[state.count] = depth;
    ++state.count;
}

struct SubmitState {
    re5::recovered::FUN_00402770_Request request{};
    unsigned calls = 0;
};

void record_submit(
    void* context,
    const re5::recovered::FUN_00402770_Request& request) noexcept {
    auto& state = *static_cast<SubmitState*>(context);
    state.request = request;
    ++state.calls;
}
} // namespace

void test_fun_00402560() {
    using namespace re5::recovered;

    FUN_00402560_Node child_visible{};
    FUN_00402560_Node sibling_hidden{};
    sibling_hidden.flags_1f = 1U;
    FUN_00402560_Node root{};
    root.child_0c = &child_visible;
    child_visible.sibling_08 = &sibling_hidden;

    VisitState visits{};
    FUN_00402560_Context context{};
    context.visit = record_visit;
    context.visit_context = &visits;

    assert(FUN_00402560(context, &root, 1) == 2U);
    assert(visits.count == 2U);
    assert(visits.nodes[0] == &child_visible);
    assert(visits.depths[0] == 2);
    assert(visits.nodes[1] == &root);
    assert(visits.depths[1] == 1);

    std::array<FUN_00402560_Node*, 4> output{};
    context.output_34 = output.data();
    context.output_count_38 = 0;
    FUN_004025D0(context, &root, 1);
    assert(context.output_count_38 == 2U);
    assert(output[0] == &child_visible);
    assert(output[1] == &root);

    const char* names[] = {"alpha", "beta"};
    const std::uintptr_t alpha_value = 0x1111U;
    const std::uintptr_t beta_value = 0x2222U;
    const void* values[] = {&alpha_value, &beta_value};
    FUN_00402640_Table table{names, values, 2U, "alpha"};
    FUN_00402640_SetTable(&table);

    alignas(void*) char block[16]{};
    std::strcpy(block + 8, "beta");
    FUN_00402640_StringRef ref{block};
    assert(FUN_00402640(ref));
    assert(FUN_004026C0(ref) == &beta_value);

    FUN_00402640_StringRef fallback{};
    assert(FUN_00402640(fallback));
    assert(FUN_004026C0(fallback) == &alpha_value);

    std::strcpy(block + 8, "gamma");
    assert(!FUN_00402640(ref));
    assert(FUN_004026C0(ref) == nullptr);

    SubmitState submit{};
    FUN_00402770_Services services{0x016E201CU, 0x01652E48U, &submit, record_submit};
    FUN_00402770_SetServices(&services);
    FUN_00402770("RE5");
    assert(submit.calls == 1U);
    assert(std::strcmp(submit.request.text, "RE5") == 0);
    assert(submit.request.width_x16 == ((0x50U - 3U) << 4U));
    assert(submit.request.half_width_x16 == (((0x50U - 3U) << 4U) >> 1U));
    assert(submit.request.min_dimension == 0x10U);
    assert(submit.request.max_dimension == 0x10U);
    assert(submit.request.opcode == 0x15EU);
    assert(submit.request.resource_0165201c == 0x016E201CU);
    assert(submit.request.resource_01652e48 == 0x01652E48U);

    FUN_00402770_SetServices(nullptr);
    FUN_00402640_SetTable(nullptr);
}
