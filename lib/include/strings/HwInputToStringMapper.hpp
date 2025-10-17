#pragma once

#include "misc/Compatibility.hpp"
#include "types/Binding.hpp"
#include "types/SemanticTypes.hpp"
#include "types/StringTypes.hpp"
#include <DGM/dgm.hpp>
#include <string>

class HwInputToStringMapper
{
public:
    [[nodiscard]] StringType operator()(sf::Keyboard::Key key)
    {
        if (sf::Keyboard::Key::A <= key && key <= sf::Keyboard::Key::Z)
            return std::string(
                1, static_cast<char>('A' + std::to_underlying(key)));
        else if (key == sf::Keyboard::Key::Space)
            return "Space";
        else if (key == sf::Keyboard::Key::Enter)
            return "Enter";
        else if (key == sf::Keyboard::Key::Escape)
            return "Escape";
        else if (key == sf::Keyboard::Key::Tab)
            return "Tab";
        else if (key == sf::Keyboard::Key::Backspace)
            return "Backspace";
        return uni::format("key {}", std::to_underlying(key));
    }

    [[nodiscard]] StringType operator()(sf::Mouse::Button btn)
    {
        if (btn == sf::Mouse::Button::Left)
            return "LMB";
        else if (btn == sf::Mouse::Button::Right)
            return "RMB";
        return uni::format("mouse {}", std::to_underlying(btn));
    }

    [[nodiscard]] StringType operator()(GamepadButton gamepadButtonIndex)
    {
        return uni::format("joy {}", gamepadButtonIndex.get());
    }

    [[nodiscard]] StringType
    operator()(std::pair<sf::Joystick::Axis, dgm::AxisHalf> axis)
    {
        auto axisName = [](sf::Joystick::Axis a) -> std::string
        {
            switch (a)
            {
            case sf::Joystick::Axis::X:
                return "X";
            case sf::Joystick::Axis::Y:
                return "Y";
            case sf::Joystick::Axis::Z:
                return "Z";
            case sf::Joystick::Axis::R:
                return "R";
            case sf::Joystick::Axis::U:
                return "U";
            case sf::Joystick::Axis::V:
                return "V";
            case sf::Joystick::Axis::PovX:
                return "PovX";
            case sf::Joystick::Axis::PovY:
                return "PovY";
            };

            return uni::format("Axis {}", std::to_underlying(a));
        };

        return uni::format(
            "{}{}",
            axisName(axis.first),
            axis.second == dgm::AxisHalf::Negative ? "-" : "+");
    }

    [[nodiscard]] StringType operator()(auto)
    {
        return "<not bound>";
    }

    [[nodiscard]] StringType getMapping(KmbBinding binding)
    {
        return std::visit(*this, binding);
    }

    [[nodiscard]] StringType getMapping(GamepadBinding binding)
    {
        return std::visit(*this, binding);
    }
};