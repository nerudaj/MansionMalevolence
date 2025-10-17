#pragma once

#include "strings/Language.hpp"
#include "types/StringTypes.hpp"
#include <string>
#include <utility>
#include <vector>

class [[nodiscard]] StringProvider final
{
public:
    StringProvider(Language primary);

    StringProvider(StringProvider&&) = delete;
    StringProvider(const StringProvider&) = delete;

public:
    constexpr void changeLanguage(Language newLang) noexcept
    {
        current = newLang;
    }

    constexpr const CharType* getString(StringId id) const noexcept
    {
        const auto result =
            strings[std::to_underlying(current)][std::to_underlying(id)].data();
        if (!result) return "--missing string--";
        return result;
    }

private:
    Language current = Language {};
    std::vector<std::vector<std::string_view>> strings;
};
