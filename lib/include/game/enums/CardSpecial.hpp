#pragma once

enum class [[nodiscard]] CardSpecial
{
    None,
    Combines,
    Vigilant,
    Retaliate,
    Blind,
    SpawnCrimsonHead,
    Deposit,
};

constexpr const int SPECIAL_HERBS = 1;
constexpr const int SPECIAL_MOON_CREST_PART = 2;
constexpr const int SPECIAL_SHIELD_KEYDOOR = 3;
constexpr const int SPECIAL_CREST_DOOR = 4;
