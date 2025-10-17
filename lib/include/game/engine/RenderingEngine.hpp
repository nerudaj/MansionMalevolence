#pragma once

#include "game/Scene.hpp"
#include "input/TouchController.hpp"
#include "misc/FpsCounter.hpp"
#include "settings/AppSettings.hpp"
#include <DGM/dgm.hpp>

class [[nodiscard]] RenderingEngine final
{
public:
    RenderingEngine(
        dgm::ResourceManager& resmgr,
        Scene& scene,
        const AppSettings& settings,
        const TouchController& touchController) noexcept;

    RenderingEngine(RenderingEngine&&) = delete;
    RenderingEngine(const RenderingEngine&) = delete;

public:
    void update(const dgm::Time& time);

    void draw(dgm::Window& window);

private:
    /**
     * \brief Create fullscreen camera with a fixed resolution and aspect ratio
     *
     * When porting games to multiple systems / resolutions / aspect ratio, the
     * renderer can show less / more than you need to. This function allows you
     * to run the game at some fixed internal resolution, inserting black bars
     * if the aspect ratio of the current device mismatches your desired aspect
     * ratio.
     */
    static dgm::Camera createFullscreenCamera(
        const sf::Vector2f& currentResolution,
        const sf::Vector2f& desiredResolution);

    void renderWorld(dgm::Window& window);

    void renderHud(dgm::Window& window);

    void renderTouchControls(dgm::Window& window);

private:
    const static inline auto INTERNAL_GAME_RESOLUTION =
        sf::Vector2f { 1280.f, 720.f };

    Scene& scene;
    const AppSettings& settings;
    const TouchController& touchController;
    dgm::Camera worldCamera;
    dgm::Camera hudCamera;

    FpsCounter fpsCounter;
    sf::Text text;

    sf::Sprite sprite;
    sf::RectangleShape ground;
};
