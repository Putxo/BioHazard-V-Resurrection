#include "re5/recovered/fun_00406840.hpp"

#include <array>

namespace re5::recovered {
namespace {
const FUN_00406840_Services* g_services = nullptr;

constexpr std::int32_t k_message_index = -1;
constexpr std::array<const char*, 16> k_archive_names{{
    "Image\\Archive\\ChapterEnd11",
    "Image\\Archive\\ChapterEnd12",
    "Image\\Archive\\ChapterEnd21",
    "Image\\Archive\\ChapterEnd22",
    "Image\\Archive\\ChapterEnd23",
    "Image\\Archive\\ChapterEnd31",
    "Image\\Archive\\ChapterEnd32",
    "Image\\Archive\\ChapterEnd33",
    "Image\\Archive\\ChapterEnd41",
    "Image\\Archive\\ChapterEnd42",
    "Image\\Archive\\ChapterEnd51",
    "Image\\Archive\\ChapterEnd52",
    "Image\\Archive\\ChapterEnd53",
    "Image\\Archive\\ChapterEnd61",
    "Image\\Archive\\ChapterEnd62",
    "Image\\Archive\\ChapterEnd63",
}};
constexpr std::uintptr_t k_direct_table_base = 0x01567C10U;
constexpr std::uintptr_t k_direct_table_stride = 0x10U;
constexpr std::int32_t k_direct_count = 2;
constexpr std::uintptr_t k_resolved_table = 0U;
constexpr std::int32_t k_resolved_count = 0;
} // namespace

void FUN_00406840_SetServices(const FUN_00406840_Services* services) noexcept {
    g_services = services;
}

void FUN_00406840(std::uint32_t chapter_index) noexcept {
    const auto* services = g_services;
    if (services == nullptr || services->dispatch_resource_group == nullptr) {
        return;
    }

    services->dispatch_resource_group(
        services->context,
        k_message_index,
        k_archive_names[chapter_index],
        k_direct_table_base + static_cast<std::uintptr_t>(chapter_index) * k_direct_table_stride,
        k_direct_count,
        k_resolved_table,
        k_resolved_count);
}

} // namespace re5::recovered
