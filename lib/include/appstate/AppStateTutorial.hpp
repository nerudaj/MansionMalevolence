#pragma once

#include "misc/DependencyContainer.hpp"
#include <DGM/classes/AppState.hpp>

class [[nodiscard]] AppStateTutorial : public dgm::AppState
{
public:
    AppStateTutorial(dgm::App& app, DependencyContainer& dic);

public:
    void input() override;

    void update() override;

    void draw() override;

private:
    void buildLayout();

    void onPageChanged(tgui::Container::Ptr content, size_t pageIdx);

private:
    const std::vector<std::string> images = {
        "skip.png",    "take.png",     "attack.png", "combine.png", "heal.png",
        "discard.png", "diamonds.png", "ammo.png",   "vaccine.png",
    };
    const std::vector<StringId> labelIds = {
        StringId::SkipText,     StringId::TakeText, StringId::AttackText,
        StringId::CombineText,  StringId::HealText, StringId::DiscardText,
        StringId::DiamondsText, StringId::AmmoText, StringId::VaccineText,
    };
    DependencyContainer& dic;
};
