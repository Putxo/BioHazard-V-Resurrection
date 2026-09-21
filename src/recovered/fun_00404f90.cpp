#include "re5/recovered/fun_00404f90.hpp"

namespace re5::recovered {
namespace {
const FUN_00404F90_Services* g_services = nullptr;

constexpr const char* k_begin_end_marker = "0x137AFEC";
constexpr const char* k_group_first = "0x137AFC0";
constexpr const char* k_group_second = "0x137AFD0";
constexpr const char* k_group_third = "0x137AFE4";
constexpr const char* k_final_marker = "0x137AF90";
constexpr std::size_t k_machine_table_count = 0x180U / 8U;
} // namespace

void FUN_00404F90_SetServices(const FUN_00404F90_Services* services) noexcept {
    g_services = services;
}

void FUN_00404F90() noexcept {
    const auto* services = g_services;
    if (services == nullptr) {
        return;
    }

    if (services->log_marker != nullptr) {
        services->log_marker(services->context, k_begin_end_marker);
    }

    const std::size_t count = services->table_count < k_machine_table_count
        ? services->table_count
        : k_machine_table_count;
    if (services->table != nullptr && services->register_entry != nullptr) {
        for (std::size_t i = 0; i < count; ++i) {
            services->register_entry(services->context, services->table[i], true);
        }
    }

    if (services->log_marker != nullptr) {
        services->log_marker(services->context, k_begin_end_marker);
    }

    if (services->register_group != nullptr) {
        services->register_group(services->context, k_group_first, k_group_second, k_group_third);
    }

    if (services->log_marker != nullptr) {
        services->log_marker(services->context, k_final_marker);
    }
}

} // namespace re5::recovered
