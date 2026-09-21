#include "re5/recovered/fun_00405930.hpp"

#include <cstdio>
#include <limits>

namespace re5::recovered {
namespace {
const FUN_00405930_Services* g_services = nullptr;
constexpr const char* k_convert_format = "CONVERT '%s' ...";
}

void FUN_00405930_SetServices(const FUN_00405930_Services* services) noexcept {
    g_services = services;
}

bool FUN_00405930(FUN_00405930_Object& object) noexcept {
    const auto* services = g_services;

    switch (object.state_2c) {
    case 2:
        if (services != nullptr && services->process_request != nullptr) {
            services->process_request(services->context, object.current_4c);
        }
        ++object.index_38;
        --object.state_2c;
        return false;

    case 1:
        if (object.index_38 >= object.class_count_30) {
            if (services != nullptr && services->free_node_table != nullptr) {
                services->free_node_table(services->context, object.nodes_34);
            }
            object.nodes_34 = nullptr;
            return true;
        }

        object.current_4c = object.nodes_34[object.index_38];
        {
            char message[0x100]{};
            const char* name = object.current_4c != nullptr && object.current_4c->payload_04 != nullptr
                ? static_cast<const char*>(object.current_4c->payload_04)
                : "";
            std::snprintf(message, sizeof(message), k_convert_format, name);

            if (services != nullptr && services->submit_message != nullptr) {
                services->submit_message(services->context, message);
            } else {
                FUN_00402770(message);
            }
        }
        ++object.state_2c;
        return false;

    case 0: {
        FUN_00402560_Context traversal{};
        traversal.visit = services != nullptr ? services->visit_counted_node : nullptr;
        traversal.visit_context = services != nullptr ? services->context : nullptr;
        FUN_00402560_Node* root = services != nullptr ? services->root_01657b84 : nullptr;

        object.class_count_30 = FUN_00402560(traversal, root, 0);
        if (services != nullptr && services->report_class_count != nullptr) {
            services->report_class_count(services->context, object.class_count_30);
        }

        constexpr std::uint32_t pointer_bytes = 4U;
        const std::uint32_t allocation_bytes = object.class_count_30 > (std::numeric_limits<std::uint32_t>::max() / pointer_bytes)
            ? std::numeric_limits<std::uint32_t>::max()
            : object.class_count_30 * pointer_bytes;
        object.nodes_34 = services != nullptr && services->allocate_node_table != nullptr
            ? services->allocate_node_table(services->context, allocation_bytes, 0x10U)
            : nullptr;
        if (object.nodes_34 == nullptr) {
            return true;
        }

        traversal.output_34 = object.nodes_34;
        traversal.output_count_38 = 0;
        FUN_004025D0(traversal, root, 0);

        if (!services->enumerate_all_01657b88) {
            object.nodes_34[0] = root;
            object.class_count_30 = 1U;
        }

        ++object.state_2c;
        object.index_38 = 0;
        return false;
    }

    default:
        return false;
    }
}

} // namespace re5::recovered
