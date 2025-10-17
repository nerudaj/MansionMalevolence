#pragma once

#include "input/InputKind.hpp"
#include "types/Binding.hpp"
#include <map>
#include <nlohmann/json.hpp>

struct [[nodiscard]] BindingsSettings final
{
    std::map<InputKind, Binding> ingameBindings = {
        { InputKind::Left,
          Binding {
              sf::Keyboard::Key::A,
              std::pair { sf::Joystick::Axis::X, dgm::AxisHalf::Negative } } },
        { InputKind::Right,
          Binding {
              sf::Keyboard::Key::D,
              std::pair { sf::Joystick::Axis::X, dgm::AxisHalf::Positive } } },
        { InputKind::Jump,
          Binding { sf::Keyboard::Key::Space, GamepadButton { 0 } } },
    };

    std::map<InputKind, Binding> menuBindings = {
        { InputKind::BackButton,
          { sf::Keyboard::Key::Escape, GamepadButton { 6 } } },
        { InputKind::ConfirmButton,
          { std::monostate {}, GamepadButton { 0 } } },
        { InputKind::CursorUp,
          { std::monostate {},
            std::pair { sf::Joystick::Axis::Y, dgm::AxisHalf::Negative } } },
        { InputKind::CursorDown,
          { std::monostate {},
            std::pair { sf::Joystick::Axis::Y, dgm::AxisHalf::Positive } } },
        { InputKind::CursorLeft,
          { std::monostate {},
            std::pair { sf::Joystick::Axis::X, dgm::AxisHalf::Negative } } },
        { InputKind::CursorRight,
          { std::monostate {},
            std::pair { sf::Joystick::Axis::X, dgm::AxisHalf::Positive } } },
        { InputKind::MenuCycleLeft,
          { std::monostate {}, GamepadButton { 4 } } },
        { InputKind::MenuCycleRight,
          { std::monostate {}, GamepadButton { 5 } } },
    };
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
    BindingsSettings, ingameBindings, menuBindings);