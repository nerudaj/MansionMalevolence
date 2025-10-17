#pragma once

#include <DGM/classes/AppState.hpp>
#include <input/InputDetector.hpp>
#include <misc/DependencyContainer.hpp>
#include <settings/InputSettings.hpp>

class AppStateInputDetector : public dgm::AppState
{
public:
    template<class BindType>
#ifndef ANDROID
        requires std::is_same_v<BindType, KmbBinding>
                     || std::is_same_v<BindType, GamepadBinding>
#endif
    AppStateInputDetector(
        dgm::App& app,
        DependencyContainer& dic,
        const InputSettings& settings,
        std::function<void(BindType)> onInputDetected)
        : dgm::AppState(app)
        , dic(dic)
        , inputDetector([&] { onInputDetectorReady(); })
        , CURSOR_SPEED(settings.cursorSpeed)
    {
        buildLayout();
        inputDetector.startCheckingInputs(
            [&, onInputDetected = std::move(onInputDetected)](BindType b)
            {
                onInputDetected(b);
                app.popState();
            },
            [&] { onCancel(); });
    }
    ~AppStateInputDetector();

public:
    void input() override;
    void update() override;
    void draw() override;

private:
    void buildLayout();

    void onInputDetectorReady();

    void onCancel();

private:
    DependencyContainer& dic;
    InputDetector inputDetector;
    const float CURSOR_SPEED;
};
