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
        "skip.jpeg", "take.jpeg", "inventory.jpeg", "vaccine.jpeg"
    };
    const std::vector<StringId> labelIds = { StringId::SkipText,
                                             StringId::TakeText,
                                             StringId::InventoryText,
                                             StringId::VaccineText };
    DependencyContainer& dic;
};
