#include "re5/recovered/fun_00401950.hpp"

namespace re5::recovered {
namespace {
const FUN_00401950_Services* g_services = nullptr;

bool matches_language(const FUN_00401950_Services& services, const char* current, const char* expected) noexcept {
    return services.compare_language_name != nullptr &&
           services.compare_language_name(current, expected) == 0;
}
} // namespace

void FUN_00401950_SetServices(const FUN_00401950_Services* services) noexcept {
    g_services = services;
}

RE5_LanguageId FUN_00401950() noexcept {
    const auto* services = g_services;
    if (services == nullptr || services->get_current_language_name == nullptr ||
        services->compare_language_name == nullptr) {
        return RE5_LanguageId::TraditionalChinese;
    }

    const char* current = services->get_current_language_name(services->context);
    if (matches_language(*services, current, "english")) {
        return RE5_LanguageId::English;
    }
    if (matches_language(*services, current, "japanese")) {
        return RE5_LanguageId::Japanese;
    }
    if (matches_language(*services, current, "german")) {
        return RE5_LanguageId::German;
    }
    if (matches_language(*services, current, "french")) {
        return RE5_LanguageId::French;
    }
    if (matches_language(*services, current, "spanish")) {
        return RE5_LanguageId::Spanish;
    }
    if (matches_language(*services, current, "italian")) {
        return RE5_LanguageId::Italian;
    }
    if (matches_language(*services, current, "russian")) {
        return RE5_LanguageId::Russian;
    }
    if (matches_language(*services, current, "polish")) {
        return RE5_LanguageId::Polish;
    }
    if (matches_language(*services, current, "czech")) {
        return RE5_LanguageId::Czech;
    }
    if (matches_language(*services, current, "tchinese")) {
        return RE5_LanguageId::TraditionalChinese;
    }
    if (matches_language(*services, current, "schinese")) {
        return RE5_LanguageId::SimplifiedChinese;
    }
    return RE5_LanguageId::TraditionalChinese;
}

} // namespace re5::recovered
