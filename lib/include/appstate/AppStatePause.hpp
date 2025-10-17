#pragma once

#include "misc/DependencyContainer.hpp"
#include "settings/AppSettings.hpp"
#include <DGM/dgm.hpp>

class [[nodiscard]] AppStatePause final : public dgm::AppState
{
public:
    AppStatePause(
        dgm::App& app, DependencyContainer& dic, AppSettings& settings) noexcept
        : dgm::AppState(
              app,
              dgm::AppStateConfig {
                  .shouldDrawUnderlyingState = true,
              })
        , dic(dic)
        , settings(settings)
    {
        buildLayout();
    }

public:
    void input() override;

    void update() override;

    void draw() override;

private:
    void buildLayout();

    void onResume();

    void onOptions();

    void onBackToMenu();

    void onExit();

    void restoreFocusImpl(const std::string& msg) override;

private:
    DependencyContainer& dic;
    AppSettings& settings;
};
