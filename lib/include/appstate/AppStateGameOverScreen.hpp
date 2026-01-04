#pragma once

#include "game/animations/Animations.hpp"
#include "game/enums/GameEndReason.hpp"
#include "misc/DependencyContainer.hpp"
#include <DGM/classes/AppState.hpp>

class [[nodiscard]] AppStateGameOverScreen final : public dgm::AppState
{
public:
    AppStateGameOverScreen(
        dgm::App& app,
        DependencyContainer& dic,
        GameEndReason endReason) noexcept;

public:
    void input() override;

    void update() override;

    void draw() override;

private:
    void buildLayout();

private:
    DependencyContainer& dic;
    GameEndReason endReason;
    AnimationBase fadeInAnimation;
    tgui::Label::Ptr deathReasonLabel;
};
