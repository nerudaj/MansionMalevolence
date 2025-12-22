#pragma once

#include <DGM/classes/Collision.hpp>
#include <DGM/classes/Controller.hpp>
#include <DGM/classes/Math.hpp>
#include <DGM/classes/Objects.hpp>
#include <SFML/Window/Event.hpp>
#include <input/InputKind.hpp>
#include <misc/Compatibility.hpp>
#include <optional>

enum class [[nodiscard]] TouchObjectKind
{
    Button,
    Joystick
};

/**
 *  Class representing either touch button or virtual joystick.
 *
 *  This is kind of discriminated union. Both inputs have some area in which
 *  the touch is supposed to be registered.
 *
 *  Joystick "hat" position equals to the touchPosition. Button's touchPosition
 *  is either outside of the touchArea (idle, not pressed) or inside (when the
 * user is actively touching the button, pressing it).
 */
class [[nodiscard]] TouchInput final
{
public:
    TouchInput(
        TouchObjectKind kind, const sf::Vector2f& position, const float radius)
        : touchArea(position, radius), touchPosition(position), kind(kind)
    {
        reset();
    }

    [[nodiscard]] bool readButton()
    {
        const bool result =
            dgm::Collision::basic(touchArea, sf::Vector2i(touchPosition));
        if (result) reset();
        return result;
    }

    [[nodiscard]] sf::Vector2f readJoystick() const
    {
        return touchPosition - touchArea.getPosition();
    }

    void reset();

public:
    dgm::Circle touchArea;
    sf::Vector2f touchPosition;
    TouchObjectKind kind;
};

/**
 * Structure representing a particular layout of touch inputs
 * for your given game. It has a list of touch input objects and
 * some convenience aliases for each of them.
 */
struct [[nodiscard]] TouchModel final
{
public:
    TouchModel(const sf::Vector2u& windowSize);

    void updateFromNewWindowSize(const sf::Vector2u& windowSize);

public:
    mutable std::array<TouchInput, 4u> objects;
    std::map<unsigned, size_t>
        fingerToTouchObject = {}; ///< Each event has finger index associated
                                  ///< with it so we can pair touch begin, touch
                                  ///< moved and touch ended with a single touch
                                  ///< input

    TouchInput& pauseButton = objects[0];
    TouchInput& takeButton = objects[1];
    TouchInput& skipButton = objects[2];
    TouchInput& dragJoystick = objects[3];
};

class [[nodiscard]] TouchController final
{
public:
    TouchController(const sf::Vector2u& windowSize) : model(windowSize) {}

public:
    void updateFromNewWindowSize(const sf::Vector2u& windowSize);

    void processEvent(const std::optional<sf::Event>& e);

    [[nodiscard]] constexpr const TouchModel& getTouchModel() const noexcept
    {
        return model;
    }

    [[nodiscard]] bool isBackPressed() const;

    [[nodiscard]] bool isTakePressed() const;

    [[nodiscard]] bool isSkipPressed() const;

    [[nodiscard]] sf::Vector2f getDragPosition() const;

private:
    void processEvent(const sf::Event::TouchBegan& e);

    void processEvent(const sf::Event::TouchEnded& e);

    void processEvent(const sf::Event::TouchMoved& e);

private:
    TouchModel model;
};
