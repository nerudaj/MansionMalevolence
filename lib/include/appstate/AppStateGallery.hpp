#pragma once

#include "game/engine/RenderingEngine.hpp"
#include "misc/DependencyContainer.hpp"
#include <DGM/classes/AppState.hpp>

class [[nodiscard]] AppStateGallery final : public dgm::AppState
{
public:
    AppStateGallery(dgm::App& app, DependencyContainer& dic);

public:
    void input() override;

    void update() override;

    void draw() override;

private:
    void buildLayout();

    void onBack();

    void onPageChanged(tgui::Container::Ptr content, size_t idx);

    void updateView();

private:
    DependencyContainer& dic;
    Scene scene;
    RenderingEngine renderer;
    tgui::CanvasSFML::Ptr canvas;
    Card currentCard;
};
