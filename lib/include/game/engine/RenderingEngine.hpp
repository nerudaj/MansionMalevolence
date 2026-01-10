#pragma once

#include "game/definitions/Scene.hpp"
#include "game/enums/BackgroundType.hpp"
#include "game/enums/CardBackground.hpp"
#include "game/enums/Icon.hpp"
#include "input/TouchController.hpp"
#include "misc/FpsCounter.hpp"
#include "settings/AppSettings.hpp"
#include <DGM/dgm.hpp>

class [[nodiscard]] RenderingEngine final
{
public:
    RenderingEngine(
        dgm::Window& window,
        dgm::ResourceManager& resmgr,
        const Scene& scene,
        const AppSettings& settings,
        const TouchController& touchController) noexcept;

    RenderingEngine(RenderingEngine&&) = delete;
    RenderingEngine(const RenderingEngine&) = delete;

public:
    void update(const dgm::Time& time);

    void draw();

    [[nodiscard]] const sf::Vector2f& getMainCardOffset() noexcept
    {
        return scene.mainCardBody.getPosition();
    }

    [[nodiscard]] static sf::Vector2f
    getNthInventoryCardOffset(size_t idx) noexcept
    {
        return sf::Vector2f { 8.f + idx * 43.5f, 182.f };
    }

    [[nodiscard]] static sf::Vector2f
    getNthBoosterChoiceOffset(size_t idx) noexcept
    {
        return sf::Vector2f { 8.f + idx * 43.5f, 109.f };
    }

    [[nodiscard]] static sf::Vector2f getTrashIconOffset() noexcept
    {
        return sf::Vector2f { 54.f, 204.f };
    }

    [[nodiscard]] static sf::Vector2f getNthHeartOffset(size_t idx) noexcept
    {
        return { 3.f + idx * 14.f, 150.f };
    }

    [[nodiscard]] static sf::Vector2f getInfectionTextOffset() noexcept
    {
        return { 95.f, 160.f };
    }

public:
    void operator()(const AnimationCardToDiscard& a);

    void operator()(const AnimationCardTransform& a);

    void operator()(const AnimationDiscardToDeck& a);

    void operator()(const AnimationDoorOpen& a);

    void operator()(const AnimationEnemyAttack& a);

    void operator()(const AnimationEnemyDamagedWindup& a);

    void operator()(const AnimationEnemyDamaged& a);

    void operator()(const AnimationEnemyDodgedAttack& a);

    void operator()(const AnimationInvalidOperation& a);

    void operator()(const AnimationNewCardsShufflingIntoDeck& a);

    void operator()(const AnimationReturnInventoryToDeck& a);

    void operator()(const AnimationTakeCard& a);

    void operator()(const AnimationTrashMainCard& a);

    void operator()(const AnimationHeal& a);

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

    void renderWorld();

    void renderBackground();

    void renderHud();

    void renderTouchControls();

    void
    renderCard(const Card& card, const sf::Vector2f& offset, float scale = 1.f)
    {
        renderCard(card, offset, { scale, scale });
    }

    void renderCard(
        const Card& card,
        const sf::Vector2f& offset,
        const sf::Vector2f& scale);

    void renderCardBack(const sf::Vector2f& offset, const sf::Vector2f& scale);

    void renderTopDeckCard();

    void renderBoosterChoice();

    BackgroundType getAppropriateBackgroundType() const;

    CardBackground getAppropriateCardBackgroundType(const Card& card) const;

    Icon getAppropriateDiamondType(const Card& card) const;

private:
    dgm::Window& window;
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
