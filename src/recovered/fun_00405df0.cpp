#include "re5/recovered/fun_00405df0.hpp"

#include <cstddef>
#include <cstdint>

namespace re5::recovered {
namespace {
const FUN_00405DF0_Services* g_services = nullptr;

constexpr const char* k_resource_directory = "Image\\Archive\\Msg2Resource";
constexpr const char* k_primary_format = "0x137B518";
constexpr const char* k_archive_format = "0x137AAD4";
constexpr const char* k_empty_literal = "";
constexpr std::uintptr_t k_special_destination = 0x016E216CU;
constexpr std::uintptr_t k_message_destination = 0x01692FC8U;
constexpr std::size_t k_local_path_size = 0x104U;

constexpr const char* k_message_paths[] = {
    "etc\\message\\mes_system",
    "etc\\message\\mes_item",
    "etc\\message\\mes_file",
    "etc\\message\\mes_event",
    "etc\\message\\mes_stage1",
    "etc\\message\\mes_stage2",
    "etc\\message\\mes_stage3",
    "etc\\message\\mes_stage5",
    "etc\\message\\mes_stage7",
};

void copy_text(char* out, std::size_t out_size, const char* text) noexcept {
    if (out == nullptr || out_size == 0U) {
        return;
    }
    const char* selected = text != nullptr ? text : k_empty_literal;
    std::size_t i = 0;
    while (selected[i] != '\0' && i + 1U < out_size) {
        out[i] = selected[i];
        ++i;
    }
    out[i] = '\0';
}

void append_text(char* out, std::size_t out_size, const char* text) noexcept {
    if (out == nullptr || out_size == 0U || text == nullptr) {
        return;
    }
    std::size_t i = 0;
    while (i < out_size && out[i] != '\0') {
        ++i;
    }
    if (i == out_size) {
        return;
    }
    std::size_t j = 0;
    while (text[j] != '\0' && i + 1U < out_size) {
        out[i++] = text[j++];
    }
    out[i] = '\0';
}

void format_path(
    const FUN_00405DF0_Services& services,
    const char* marker,
    const char* first,
    const char* second,
    char* out) noexcept {
    if (services.format_path != nullptr) {
        services.format_path(services.context, marker, first, second, out, k_local_path_size);
        return;
    }

    if (second == nullptr) {
        copy_text(out, k_local_path_size, first);
        return;
    }
    copy_text(out, k_local_path_size, first);
    append_text(out, k_local_path_size, "\\");
    append_text(out, k_local_path_size, second);
    append_text(out, k_local_path_size, ".arc");
}

const char* resolve_path(const FUN_00405DF0_Services& services, const char* path) noexcept {
    if (services.resolve_path != nullptr) {
        const char* resolved = services.resolve_path(services.context, path);
        if (resolved != nullptr) {
            return resolved;
        }
    }
    return path;
}

bool registry_allowed(const FUN_00405DF0_Services& services, std::uintptr_t registered) noexcept {
    return !services.registry_suppressed_a &&
        !services.registry_suppressed_b &&
        registered != 0U &&
        services.bind_registered != nullptr;
}

void finish_and_restore(
    const FUN_00405DF0_Services& services,
    std::uint32_t previous_selector,
    bool previous_loading) noexcept {
    if (services.global_loading != nullptr) {
        *services.global_loading = previous_loading;
    }
    if (services.set_active_selector != nullptr) {
        services.set_active_selector(services.context, previous_selector);
    }
    if (services.finish_trace != nullptr) {
        services.finish_trace(services.context);
    }
}
} // namespace

void FUN_00405DF0_SetServices(const FUN_00405DF0_Services* services) noexcept {
    g_services = services;
}

void FUN_00405DF0(std::uint32_t selector) noexcept {
    const auto* services = g_services;
    if (services == nullptr) {
        return;
    }

    const std::uint32_t previous_selector = services->get_active_selector != nullptr
        ? services->get_active_selector(services->context)
        : selector;
    if (services->set_active_selector != nullptr) {
        services->set_active_selector(services->context, selector);
    }

    const bool previous_loading = services->global_loading != nullptr
        ? *services->global_loading
        : false;
    if (services->global_loading != nullptr) {
        *services->global_loading = true;
    }

    char primary_path[k_local_path_size]{};
    char archive_path[k_local_path_size]{};
    const char* resolved_directory = resolve_path(*services, k_resource_directory);
    format_path(*services, k_primary_format, resolved_directory, nullptr, primary_path);
    format_path(
        *services,
        k_archive_format,
        services->global_root != nullptr ? services->global_root : k_empty_literal,
        primary_path,
        archive_path);

    if (services->global_special_enabled) {
        if (services->prepare_archive != nullptr) {
            services->prepare_archive(services->context, archive_path);
        }
        const std::uintptr_t registered = services->register_resource != nullptr
            ? services->register_resource(services->context, k_special_destination, primary_path, true)
            : 0U;
        if (registered != 0U && services->finalize_registered != nullptr) {
            services->finalize_registered(services->context, registered);
        }
        if (services->check_cancel != nullptr && services->check_cancel(services->context, archive_path)) {
            finish_and_restore(*services, previous_selector, previous_loading);
            return;
        }
    }

    if (services->begin_batch != nullptr) {
        services->begin_batch(services->context, primary_path);
    }

    for (const char* path : k_message_paths) {
        const char* resolved = resolve_path(*services, path);
        std::uintptr_t registered = services->register_resource != nullptr
            ? services->register_resource(services->context, k_message_destination, resolved, true)
            : 0U;
        if (registry_allowed(*services, registered)) {
            services->bind_registered(services->context, &registered);
        }
    }

    if (services->end_batch != nullptr) {
        services->end_batch(services->context);
    }

    finish_and_restore(*services, previous_selector, previous_loading);
}

} // namespace re5::recovered
