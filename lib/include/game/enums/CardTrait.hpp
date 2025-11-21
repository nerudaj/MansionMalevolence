#pragma once

#include <utility>

enum class [[nodiscard]] CardTrait
{
    None = 0,
    Pickable = 1,
    Enemy = 2,
    Weapon = 4,
    Healing = 8,
    Ammo = 16,
    KeyItem = 32,
    KeyTarget = 64,
    KeyItemPart = 128,
};

constexpr CardTrait operator|(CardTrait a, CardTrait b)
{
    using T = std::underlying_type_t<CardTrait>;
    return static_cast<CardTrait>(static_cast<T>(a) | static_cast<T>(b));
}

constexpr bool operator&(CardTrait base, CardTrait question)
{
    using T = std::underlying_type_t<CardTrait>;
    return static_cast<T>(base) & static_cast<T>(question);
}
