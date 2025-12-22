#pragma once

#include <SFML/System/Vector2.hpp>

class [[nodiscard]] CoordConverter final
{
public:
    static sf::Vector2f worldToScreen(
        const sf::Vector2f& position,
        const sf::Vector2u& worldResolution,
        const sf::Vector2u& screenResolution)
    {
        if (isOffsetOnY(worldResolution, screenResolution))
        {
            // We can only be offset on Y, but not on X
            const float scale =
                static_cast<float>(screenResolution.x) / worldResolution.x;
            const float yoffsetRelative =
                (1.f - (worldResolution.y * scale / screenResolution.y)) / 2.f;

            return position * scale
                   + sf::Vector2f(0.f, screenResolution.y * yoffsetRelative);
        }
        else
        {
            // We can only be offset on X, but not on Y
            const float scale =
                static_cast<float>(screenResolution.y) / worldResolution.y;
            const float xoffsetRelative =
                (1.f - (worldResolution.x * scale / screenResolution.x)) / 2.f;

            return position * scale
                   + sf::Vector2f(screenResolution.x * xoffsetRelative, 0.f);
        }
    }

    static float worldToScreen(
        float len,
        const sf::Vector2u& worldResolution,
        const sf::Vector2u& screenResolution)
    {
        if (isOffsetOnY(worldResolution, screenResolution))
        {
            return len * static_cast<float>(screenResolution.x)
                   / worldResolution.x;
        }
        else
        {
            return len * static_cast<float>(screenResolution.y)
                   / worldResolution.y;
        }
    }

    static sf::Vector2f screenToWorld(
        const sf::Vector2f& position,
        const sf::Vector2u& worldResolution,
        const sf::Vector2u& screenResolution)
    {
        if (isOffsetOnY(worldResolution, screenResolution))
        {
            // We can only be offset on Y, but not on X
            const float scale =
                static_cast<float>(screenResolution.x) / worldResolution.x;
            const float yoffsetRelative =
                (1.f - (worldResolution.y * scale / screenResolution.y)) / 2.f;

            return (position
                    - sf::Vector2f { 0.f,
                                     screenResolution.y * yoffsetRelative })
                   / scale;
        }
        else
        {
            // We can only be offset on X, but not on Y
            const float scale =
                static_cast<float>(screenResolution.y) / worldResolution.y;
            const float xoffsetRelative =
                (1.f - (worldResolution.x * scale / screenResolution.x)) / 2.f;

            return (position
                    - sf::Vector2f { screenResolution.x * xoffsetRelative,
                                     0.f })
                   / scale;
        }
    }

private:
    static bool isOffsetOnY(
        const sf::Vector2u& worldResolution,
        const sf::Vector2u& screenResolution)
    {
        const auto internalAspect =
            static_cast<float>(worldResolution.x) / worldResolution.y;
        const auto screenAspect =
            static_cast<float>(screenResolution.x) / screenResolution.y;

        return screenAspect <= internalAspect;
    }
};
