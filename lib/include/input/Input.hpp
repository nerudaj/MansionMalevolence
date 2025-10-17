#pragma once

#include "input/InputKind.hpp"
#include "input/TouchController.hpp"
#include "misc/Compatibility.hpp"
#include "settings/BindingsSettings.hpp"
#include <DGM/dgm.hpp>

class [[nodiscard]] Input final
{
public:
    Input(const BindingsSettings& settings, TouchController& touchController)
        : controller(configureController(settings))
        , touchController(touchController)
    {
    }

    Input(Input&&) = delete;
    Input(const Input&) = delete;

public:
    void updateBindings(const BindingsSettings& settings);

    /// <summary>
    /// This is used in options to make sure that a configured
    /// key for confirm/back doesn't immediately mess up with
    /// the app.
    /// </summary>
    void forceRelease(InputKind action);

#pragma region For dummy entity, can be removed
    float getHorizontalVelocity() const;

    [[nodiscard]] bool isJumpPressed() const;
#pragma endregion

    [[nodiscard]] bool isMenuCycleLeftPressed() const;

    [[nodiscard]] bool isMenuCycleRightPressed() const;

    [[nodiscard]] bool isBackButtonPressed() const;

    [[nodiscard]] bool isConfirmPressed() const;

    [[nodiscard]] sf::Vector2f getCursorDelta() const;

private:
    bool readAndRelease(InputKind i) const;

    static dgm::Controller<InputKind>
    configureController(const BindingsSettings& settings);

private:
    mutable dgm::Controller<InputKind> controller;
    TouchController& touchController;
};
