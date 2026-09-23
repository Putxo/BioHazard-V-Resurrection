#include "re5/recovered/fun_00407250.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace re5::recovered {
namespace {
const FUN_00407250_Services* g_services = nullptr;

constexpr const char* k_game_archive = "Image\\Archive\\GameResource";
constexpr const char* k_bench_archive = "Image\\Archive\\BenchResource";

constexpr std::array<const char*, 16> k_chapter_archives{{
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

constexpr std::array<const char*, 8> k_versus_archives{{
    "Image\\Archive\\VersusResult01",
    "Image\\Archive\\VersusResult02",
    "Image\\Archive\\VersusResult03",
    "Image\\Archive\\VersusResult04",
    "Image\\Archive\\VersusResult05",
    "Image\\Archive\\VersusResult06",
    "Image\\Archive\\VersusResult07",
    "Image\\Archive\\VersusResult08",
}};

constexpr std::array<const char*, 8> k_merce_archives{{
    "Image\\Archive\\MerceResult01",
    "Image\\Archive\\MerceResult02",
    "Image\\Archive\\MerceResult03",
    "Image\\Archive\\MerceResult04",
    "Image\\Archive\\MerceResult05",
    "Image\\Archive\\MerceResult06",
    "Image\\Archive\\MerceResult07",
    "Image\\Archive\\MerceResult08",
}};

void dispatch(
    const FUN_00407250_Services& services,
    std::int32_t message_index,
    const char* archive_name,
    std::uintptr_t direct_table,
    std::int32_t direct_count,
    std::uintptr_t resolved_table,
    std::int32_t resolved_count) noexcept {
    services.dispatch_resource_group(
        services.context,
        message_index,
        archive_name,
        direct_table,
        direct_count,
        resolved_table,
        resolved_count);
}
} // namespace

void FUN_00407250_SetServices(const FUN_00407250_Services* services) noexcept {
    g_services = services;
}

void FUN_00407250() noexcept {
    const auto* services = g_services;
    if (services == nullptr || services->dispatch_resource_group == nullptr) {
        return;
    }

    for (std::int32_t message_index = 0; message_index < 11; ++message_index) {
        dispatch(
            *services,
            message_index,
            k_game_archive,
            0x01567590U,
            15,
            0x01567608U,
            7);
    }

    std::uintptr_t direct_table = 0x01567C10U;
    for (const auto* archive_name : k_chapter_archives) {
        dispatch(*services, -1, archive_name, direct_table, 2, 0U, 0);
        direct_table += 0x10U;
    }

    direct_table = 0x01567D50U;
    for (const auto* archive_name : k_versus_archives) {
        dispatch(*services, -1, archive_name, direct_table, 2, 0U, 0);
        direct_table += 0x10U;
    }

    direct_table = 0x01567DF0U;
    for (const auto* archive_name : k_merce_archives) {
        dispatch(*services, -1, archive_name, direct_table, 2, 0U, 0);
        direct_table += 0x10U;
    }

    dispatch(*services, -1, k_bench_archive, 0x01567E90U, 2, 0U, 0);
}

} // namespace re5::recovered
