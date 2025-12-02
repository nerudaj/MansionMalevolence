#pragma once

#include "game/enums/GameScenario.hpp"
#include "misc/DependencyContainer.hpp"
#include <DGM/classes/AppState.hpp>

class [[nodiscard]] AppStatePreGame final : public dgm::AppState
{
public:
    AppStatePreGame(
        dgm::App& app,
        DependencyContainer& dic,
        AppSettings& settings,
        GameScenario scenario);

public:
    void input() override;

    void update() override;

    void draw() override;

private:
    void restoreFocusImpl(const std::string& msg) override;

private:
    DependencyContainer& dic;
    AppSettings& settings;
    GameScenario scenario;
};
