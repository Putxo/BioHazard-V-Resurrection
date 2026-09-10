#include "re5/recovered/fun_00401950.hpp"

#include <cassert>
#include <cstring>

namespace {
const char* g_current_language = nullptr;

const char* get_current_language_name(void*) noexcept {
    return g_current_language;
}

int compare_language_name(const char* lhs, const char* rhs) noexcept {
    if (lhs == nullptr || rhs == nullptr) {
        return lhs == rhs ? 0 : 1;
    }
    return std::strcmp(lhs, rhs);
}
} // namespace

void test_fun_00401950() {
    using namespace re5::recovered;

    const FUN_00401950_Services services{
        nullptr,
        &get_current_language_name,
        &compare_language_name,
    };

    FUN_00401950_SetServices(&services);

    g_current_language = "japanese";
    assert(FUN_00401950() == RE5_LanguageId::Japanese);
    g_current_language = "english";
    assert(FUN_00401950() == RE5_LanguageId::English);
    g_current_language = "german";
    assert(FUN_00401950() == RE5_LanguageId::German);
    g_current_language = "french";
    assert(FUN_00401950() == RE5_LanguageId::French);
    g_current_language = "spanish";
    assert(FUN_00401950() == RE5_LanguageId::Spanish);
    g_current_language = "italian";
    assert(FUN_00401950() == RE5_LanguageId::Italian);
    g_current_language = "russian";
    assert(FUN_00401950() == RE5_LanguageId::Russian);
    g_current_language = "polish";
    assert(FUN_00401950() == RE5_LanguageId::Polish);
    g_current_language = "czech";
    assert(FUN_00401950() == RE5_LanguageId::Czech);
    g_current_language = "tchinese";
    assert(FUN_00401950() == RE5_LanguageId::TraditionalChinese);
    g_current_language = "schinese";
    assert(FUN_00401950() == RE5_LanguageId::SimplifiedChinese);
    g_current_language = "unknown";
    assert(FUN_00401950() == RE5_LanguageId::TraditionalChinese);

    FUN_00401950_SetServices(nullptr);
    assert(FUN_00401950() == RE5_LanguageId::TraditionalChinese);
}
