#pragma once

enum class [[nodiscard]] CardSpecial
{
    None = 0,
    Vigilant = 2,
    Retaliate = 4,
    Evasive = 8,
    Blind = 16,
    SpawnCrimsonHead = 32,
    Combines = 64,
    BoosterPack = 128,
};

constexpr const int SPECIAL_HERBS = 1;
constexpr const int SPECIAL_MOON_CREST_PART = 2;
constexpr const int SPECIAL_SHIELD_KEYDOOR = 3;
constexpr const int SPECIAL_CREST_DOOR = 4;
constexpr const int SPECIAL_DIAMOND_KEYDOOR = 5;

constexpr CardSpecial operator|(CardSpecial a, CardSpecial b)
{
    using T = std::underlying_type_t<CardSpecial>;
    return static_cast<CardSpecial>(static_cast<T>(a) | static_cast<T>(b));
}

constexpr bool operator&(CardSpecial base, CardSpecial question)
{
    using T = std::underlying_type_t<CardSpecial>;
    return static_cast<T>(base) & static_cast<T>(question);
}
