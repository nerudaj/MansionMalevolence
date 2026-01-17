#include "strings/StringProvider.hpp"
#include "localization/CzLocalization.hpp"
#include "localization/EnLocalization.hpp"
#include "localization/PlLocalization.hpp"
#include <ranges>

StringProvider::StringProvider(Language primary) : current(primary)
{
    const std::map<Language, Localization>& localizations = {
        { Language::English, EN_LOCALIZATION },
        { Language::Czech, CZ_LOCALIZATION },
        { Language::Polish, PL_LOCALIZATION },
    };

    strings.resize(localizations.size());

    // Fill the database
    for (auto&& [lang, localization] : localizations)
    {
        strings[std::to_underlying(lang)].resize(
            std::to_underlying(StringId::MaxId));

        for (auto&& [id, string] : localization)
        {
            strings[std::to_underlying(lang)][std::to_underlying(id)] = string;
        }
    }

    // For untranslated strings, use strings from the primary language
    for (auto&& localization : strings)
    {
        for (auto&& idx : std::views::iota(size_t {}, localization.size()))
        {
            if (localization[idx].empty())
                localization[idx] = strings[std::to_underlying(primary)][idx];
        }
    }
}
