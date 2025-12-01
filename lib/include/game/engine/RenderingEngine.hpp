#pragma once

#include "game/definitions/Scene.hpp"
#include "game/enums/BackgroundType.hpp"
#include "input/TouchController.hpp"
#include "misc/FpsCounter.hpp"
#include "settings/AppSettings.hpp"
#include <DGM/dgm.hpp>

class [[nodiscard]] RenderingEngine final
{
public:
    RenderingEngine(
        dgm::ResourceManager& resmgr,
        const Scene& scene,
        const AppSettings& settings,
        const TouchController& touchController) noexcept;

    RenderingEngine(RenderingEngine&&) = delete;
    RenderingEngine(const RenderingEngine&) = delete;

public:
    void update(const dgm::Time& time);

    void draw(dgm::Window& window);

    [[nodiscard]] static sf::Vector2f getDeckCardOffset() noexcept
    {
        return sf::Vector2f { 7.f, 35.f };
    }

    [[nodiscard]] static sf::Vector2f
    getNthInventoryCardOffset(size_t idx) noexcept
    {
        return sf::Vector2f { 8.f + idx * 43.5f, 161.f };
    }

    [[nodiscard]] static sf::Vector2f getTrashIconOffset() noexcept
    {
        return sf::Vector2f { 54.f, 204.f };
    }

    [[nodiscard]] static sf::Vector2f getNthHeartOffset(size_t idx) noexcept
    {
        return { 31.f + idx * 18.f, 3.f };
    }

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

    void renderBackground(dgm::Window& window);

    void renderHud(dgm::Window& window);

    void renderTouchControls(dgm::Window& window);

    void renderCard(
        dgm::Window& window,
        const Card& card,
        const sf::Vector2f& offset,
        float scale = 1.f)
    {
        renderCard(window, card, offset, { scale, scale });
    }

    void renderCard(
        dgm::Window& window,
        const Card& card,
        const sf::Vector2f& offset,
        const sf::Vector2f& scale);

    void renderCardBack(
        dgm::Window& window,
        const sf::Vector2f& offset,
        const sf::Vector2f& scale);

    void renderTopDeckCard(dgm::Window& window);

    BackgroundType getAppropriateBackgroundType() const;

private:
    const Scene& scene;
    const AppSettings& settings;
    const TouchController& touchController;
    dgm::TextureAtlas atlas;
    dgm::Camera worldCamera;
    dgm::Camera hudCamera;
    FpsCounter fpsCounter;
    sf::Text text;

    dgm::TextureAtlas::ResourceLocation<dgm::Clip> playbgrLocation;
    dgm::TextureAtlas::ResourceLocation<dgm::Clip> cardbgrLocation;
    dgm::TextureAtlas::ResourceLocation<dgm::Clip> iconsLocation;
    dgm::TextureAtlas::ResourceLocation<dgm::Clip> imagesLocation;
    sf::Sprite sprite;
};
