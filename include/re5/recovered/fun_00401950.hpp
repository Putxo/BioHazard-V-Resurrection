#pragma once

namespace re5::recovered {

enum class RE5_LanguageId {
    Japanese = 0,
    English = 1,
    German = 2,
    French = 3,
    Spanish = 4,
    Italian = 5,
    Russian = 6,
    Polish = 7,
    Czech = 8,
    TraditionalChinese = 9,
    SimplifiedChinese = 10,
};

struct FUN_00401950_Services {
    void* context;
    const char* (*get_current_language_name)(void* context) noexcept;
    int (*compare_language_name)(const char* lhs, const char* rhs) noexcept;
};

void FUN_00401950_SetServices(const FUN_00401950_Services* services) noexcept;
RE5_LanguageId FUN_00401950() noexcept;

} // namespace re5::recovered
