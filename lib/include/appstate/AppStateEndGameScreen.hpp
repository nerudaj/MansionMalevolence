#pragma once

#include "game/definitions/GameStats.hpp"
#include "game/enums/GameEndReason.hpp"
#include "misc/DependencyContainer.hpp"
#include "settings/AppSettings.hpp"
#include <DGM/classes/AppState.hpp>

class [[nodiscard]] AppStateEndGameScreen final : public dgm::AppState
{
public:
    AppStateEndGameScreen(
        dgm::App& app,
        DependencyContainer& dic,
        const AppSettings& settings,
        const GameStats& stats,
        GameEndReason endReason) noexcept;

public:
    void input() override;

    void update() override;

    void draw() override;

private:
    void buildLayout();

private:
    DependencyContainer& dic;
    const AppSettings& settings;
    GameStats stats;
    GameEndReason endReason;
};
