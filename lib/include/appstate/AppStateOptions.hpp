#pragma once

#include "input/InputDetector.hpp"
#include "misc/DependencyContainer.hpp"
#include "settings/AppSettings.hpp"
#include "strings/HwInputToStringMapper.hpp"
#include <DGM/dgm.hpp>

class [[nodiscard]] AppStateOptions final : public dgm::AppState
{
public:
    AppStateOptions(
        dgm::App& app,
        DependencyContainer& dic,
        AppSettings& settings) noexcept;

public:
    void input() override;
    void update() override;
    void draw() override;

private:
    void buildLayout();

    // Rebuilds layout if there is an appropriate change, such as ui scale or
    // resolution change
    void refresh();

    void onBack();

private:
    DependencyContainer& dic;
    AppSettings& settings;
};
