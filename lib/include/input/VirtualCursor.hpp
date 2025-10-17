#pragma once

#include "gui/Gui.hpp"
#include "input/Input.hpp"
#include <DGM/dgm.hpp>

/**
 *  \brief Class that represents menu cursor that can be
 *  moved on screen by means of gamepad and other sources.
 */
class [[nodiscard]] VirtualCursor final
{
public:
    VirtualCursor(
        sf::RenderWindow& window,
        Input& input,
        const sf::Texture& cursorTexture)
        : window(window), input(input), sprite(cursorTexture)
    {
    }

public:
    void update(const dgm::Time& time, const float cursorSpeed);

    [[nodiscard]] const sf::Vector2f& getPosition() const noexcept
    {
        return position;
    }

    void draw();

private:
    static sf::Vector2f clampPositionByWindow(
        const sf::Vector2f& currentPosition, const sf::Vector2u& windowSize);

private:
    sf::RenderWindow& window;
    Input& input;
    sf::Sprite sprite;
    sf::Vector2f position;
};