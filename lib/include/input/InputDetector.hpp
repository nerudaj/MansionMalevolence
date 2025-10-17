#pragma once

#include "types/Binding.hpp"
#include <functional>

/**
 * \brief Class used for detecting inputs when rebinding controls
 *
 * It can be configured to start checking inputs either for gamepad
 * or for keyboard+mouse combo. This checking can either be cancelled by
 * pressing the Escape key or it is finished by pressing any appropriate
 * input.
 *
 * This pressed input is returned via reportCallback.
 *
 * Since inputs can be pressed when this class is asked to look for any,
 * it first waits until all inputs are released (and fires appropriate callback
 * when that happens).
 */
class [[nodiscard]] InputDetector final
{
public:
    using ReportKmbCallbackType = std::function<void(KmbBinding)>;
    using ReportGamepadCallbackType = std::function<void(GamepadBinding)>;
    using CancelCallbackType = std::function<void()>;
    using ReadyForInputsCallbackType = std::function<void()>;

public:
    InputDetector(ReadyForInputsCallbackType markReadyForInputs)
        : markReadyForInputs(markReadyForInputs)
    {
    }

public:
    void startCheckingInputs(
        ReportKmbCallbackType reportCallback, CancelCallbackType cancelCallback)
    {
        reportKmb = reportCallback;
        cancel = cancelCallback;
        runMode = RunMode::DetectingKmb;
        isWaitingForInputsToClear = true;
    }

    void startCheckingInputs(
        ReportGamepadCallbackType reportCallback,
        CancelCallbackType cancelCallback)
    {
        reportGamepad = reportCallback;
        cancel = cancelCallback;
        runMode = RunMode::DetectingGamepad;
        isWaitingForInputsToClear = true;
    }

    void update();

    [[nodiscard]] bool isDetectionInProgress() const noexcept
    {
        return runMode != RunMode::Idle;
    }

private:
    enum class [[nodiscard]] RunMode
    {
        Idle,
        DetectingKmb,
        DetectingGamepad,
    };

    enum class [[nodiscard]] DetectionStatus
    {
        None,
        InputDetected
    };

    DetectionStatus tryGamepadAxis();

    DetectionStatus tryGamepadButton();

    DetectionStatus tryKeyboard();

    DetectionStatus tryMouse();

    [[nodiscard]] bool isAnyInputPressed() const;

private:
    RunMode runMode = RunMode::Idle;
    ReadyForInputsCallbackType markReadyForInputs;
    ReportKmbCallbackType reportKmb;
    ReportGamepadCallbackType reportGamepad;
    CancelCallbackType cancel;
    bool isWaitingForInputsToClear = true;
};
