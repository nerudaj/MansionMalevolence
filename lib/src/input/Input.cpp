#include "input/Input.hpp"
#include "types/Overloads.hpp"

void Input::updateBindings(const BindingsSettings& settings)
{
    controller = configureController(settings);
}

void Input::forceRelease(InputKind action)
{
    controller.forceRelease(action);
}

float Input::getHorizontalVelocity() const
{
    return -controller.readAnalog(InputKind::Left)
           + controller.readAnalog(InputKind::Right)
           + touchController.getHorizontalVelocity();
}

bool Input::isJumpPressed() const
{
    return controller.readDigital(InputKind::Jump)
           || touchController.isJumpPressed();
}

[[nodiscard]] bool Input::isMenuCycleLeftPressed() const
{
    return readAndRelease(InputKind::MenuCycleLeft);
}

[[nodiscard]] bool Input::isMenuCycleRightPressed() const
{
    return readAndRelease(InputKind::MenuCycleRight);
}

bool Input::isBackButtonPressed() const
{
    return readAndRelease(InputKind::BackButton) || touchController.isBackPressed();
}

bool Input::isConfirmPressed() const
{
    return readAndRelease(InputKind::ConfirmButton);
}

sf::Vector2f Input::getCursorDelta() const
{
    return sf::Vector2f {
        -controller.readAnalog(InputKind::CursorLeft)
            + controller.readAnalog(InputKind::CursorRight),
        -controller.readAnalog(InputKind::CursorUp)
            + controller.readAnalog(InputKind::CursorDown),
    };
}

bool Input::readAndRelease(InputKind i) const
{
    return controller.readDigital(i, dgm::DigitalReadKind::OnPress);
}

dgm::Controller<InputKind>
Input::configureController(const BindingsSettings& settings)
{
    auto controller = dgm::Controller<InputKind>();

    auto bindActions = [&](auto bindings)
    {
        for (auto&& [action, binding] : bindings)
        {
            auto&& [kmbBinding, gamepadBinding] = binding;

            std::visit(
                overloads { [&](sf::Keyboard::Key key)
                            { controller.bindInput(action, key); },
                            [&](sf::Mouse::Button mbtn)
                            { controller.bindInput(action, mbtn); },
                            [&](auto) {} },
                kmbBinding);
            std::visit(
                overloads {
                    [&](GamepadButton btn)
                    { controller.bindInput(action, btn.get()); },
                    [&](std::pair<sf::Joystick::Axis, dgm::AxisHalf> joy)
                    { controller.bindInput(action, joy.first, joy.second); },
                    [&](auto) {} },
                gamepadBinding);
        }
    };

    bindActions(settings.ingameBindings);
    bindActions(settings.menuBindings);

    return controller;
}
