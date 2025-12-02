#pragma once

#include "misc/DependencyContainer.hpp"
#include <DGM/classes/AppState.hpp>

class [[nodiscard]] AppStateLevelSelect final : public dgm::AppState
{
public:
    AppStateLevelSelect(
        dgm::App& app, DependencyContainer& dic, AppSettings& settings);

public:
    void input() override;

    void update() override;

    void draw() override;

private:
    void restoreFocusImpl(const std::string& msg) override;

    void buildLayout();

private:
    DependencyContainer& dic;
    AppSettings& settings;
};
