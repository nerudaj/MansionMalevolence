#pragma once

#include "types/BrandedType.hpp"
#include <SFML/System/Vector2.hpp>

using Direction = BrandedType<sf::Vector2f, struct DirectionTypeTag>;
using Position = BrandedType<sf::Vector2f, struct PositionTypeTag>;
using GamepadButton = BrandedType<unsigned, struct GamepadButtonTag>;

consteval float operator""_px_per_second(unsigned long long value)
{
    return static_cast<float>(value);
}

consteval bool operator""_true(const char*, size_t)
{
    return true;
}

consteval bool operator""_false(const char*, size_t)
{
    return false;
}

/* Example usage:
auto v = 10_px_per_second;
auto f = "disabled"_true;
*/
