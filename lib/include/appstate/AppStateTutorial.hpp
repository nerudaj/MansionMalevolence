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

    void updatePage();

private:
    int page = 0;
    const std::vector<std::string> images = { "skip.jpeg",
                                              "take.jpeg",
                                              "inventory.jpeg" };
    const std::vector<StringId> labelIds = { StringId::SkipText,
                                             StringId::TakeText,
                                             StringId::InventoryText };
    DependencyContainer& dic;
};
