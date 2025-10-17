#include "input/InputDetector.hpp"

constexpr const float AXIS_THRESHOLD_NEG = -80.f;
constexpr const float AXIS_THRESHOLD_POS = 80.f;

void InputDetector::update()
{
    if (runMode == RunMode::Idle) return;

    if (isWaitingForInputsToClear)
    {
        isWaitingForInputsToClear = isAnyInputPressed();
        if (!isWaitingForInputsToClear)
            markReadyForInputs();
        else
            return;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
    {
        runMode = RunMode::Idle;
        cancel();
    }
    else if (runMode == RunMode::DetectingKmb)
    {
        if (tryKeyboard() == DetectionStatus::InputDetected
            || tryMouse() == DetectionStatus::InputDetected)
        {
            runMode = RunMode::Idle;
        }
    }
    else if (runMode == RunMode::DetectingGamepad)
    {
        if (tryGamepadAxis() == DetectionStatus::InputDetected
            || tryGamepadButton() == DetectionStatus::InputDetected)
        {
            runMode = RunMode::Idle;
        }
    }
}

InputDetector::DetectionStatus InputDetector::tryGamepadAxis()
{
    for (auto&& axisIdx = 0; axisIdx < sf::Joystick::AxisCount; ++axisIdx)
    {
        const auto axis = static_cast<sf::Joystick::Axis>(axisIdx);
        const auto position = sf::Joystick::getAxisPosition(0, axis);

        if (position < AXIS_THRESHOLD_NEG)
        {
            reportGamepad(std::pair { axis, dgm::AxisHalf::Negative });
            return DetectionStatus::InputDetected;
        }
        else if (position > AXIS_THRESHOLD_POS)
        {
            reportGamepad(std::pair { axis, dgm::AxisHalf::Positive });
            return DetectionStatus::InputDetected;
        }
    }

    return DetectionStatus::None;
}

InputDetector::DetectionStatus InputDetector::tryGamepadButton()
{
    for (auto&& btnIdx = 0u; btnIdx < sf::Joystick::ButtonCount; ++btnIdx)
    {
        const bool pressed = sf::Joystick::isButtonPressed(0, btnIdx);
        if (pressed)
        {
            reportGamepad(GamepadButton { btnIdx });
            return DetectionStatus::InputDetected;
        }
    }

    return DetectionStatus::None;
}

InputDetector::DetectionStatus InputDetector::tryKeyboard()
{
    for (auto&& keyIdx = 0; keyIdx < sf::Keyboard::KeyCount; ++keyIdx)
    {
        const auto key = static_cast<sf::Keyboard::Key>(keyIdx);
        const bool pressed = sf::Keyboard::isKeyPressed(key);

        if (pressed)
        {
            reportKmb(key);
            return DetectionStatus::InputDetected;
        }
    }

    return DetectionStatus::None;
}

InputDetector::DetectionStatus InputDetector::tryMouse()
{
    for (auto&& mbtnIdx = 0; mbtnIdx < sf::Mouse::ButtonCount; ++mbtnIdx)
    {
        const auto mbtn = static_cast<sf::Mouse::Button>(mbtnIdx);
        const bool pressed = sf::Mouse::isButtonPressed(mbtn);

        if (pressed)
        {
            reportKmb(mbtn);
            return DetectionStatus::InputDetected;
        }
    }

    return DetectionStatus::None;
}

[[nodiscard]] bool InputDetector::isAnyInputPressed() const
{
    for (auto&& axisIdx = 0; axisIdx < sf::Joystick::AxisCount; ++axisIdx)
        if (std::abs(sf::Joystick::getAxisPosition(
                0, static_cast<sf::Joystick::Axis>(axisIdx)))
            > AXIS_THRESHOLD_POS)
            return true;

    for (auto&& btnIdx = 0u; btnIdx < sf::Joystick::ButtonCount; ++btnIdx)
        if (sf::Joystick::isButtonPressed(0, btnIdx)) return true;

    for (auto&& keyIdx = 0; keyIdx < sf::Keyboard::KeyCount; ++keyIdx)
        if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(keyIdx)))
            return true;

    for (auto&& mbtnIdx = 0; mbtnIdx < sf::Mouse::ButtonCount; ++mbtnIdx)
        if (sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(mbtnIdx)))
            return true;

    return false;
}
