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

    void operator()(const AnimationReturnDraggedMainCard& a);

public:
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

    template<class Target>
    void renderCard(
        Target& target,
        const Card& card,
        const sf::Vector2f& offset,
        const sf::Vector2f& scale)
    {
        text.setScale(scale);
        sprite.setScale(scale);

        sprite.setTextureRect(atlas.getClip(cardbgrLocation)
                                  .getFrame(std::to_underlying(
                                      getAppropriateCardBackgroundType(card))));
        sprite.setPosition(offset);
        target.draw(sprite);

        sprite.setTextureRect(atlas.getClip(imagesLocation)
                                  .getFrame(std::to_underlying(card.image)));
        sprite.setPosition(
            offset + sf::Vector2f { 10.f, 9.f }.componentWiseMul(scale));
        target.draw(sprite);

        auto& iconsClip = atlas.getClip(iconsLocation);
        sprite.setTextureRect(
            iconsClip.getFrame(std::to_underlying(Icon::BulletBig)));

        for (auto i = 0; i < card.quantity && card.traits & CardTrait::Weapon;
             ++i)
        {
            sprite.setPosition(
                offset + sf::Vector2f { 0.f, 13.f + i * 7.f } * scale.y);
            target.draw(sprite);
        }

        sprite.setTextureRect(iconsClip.getFrame(
            std::to_underlying(getAppropriateDiamondType(card))));

        for (auto i = 0; i < card.power; ++i)
        {
            sprite.setPosition(
                offset
                + sf::Vector2f { 2.f + i * 12.f, 102.f }.componentWiseMul(
                    scale));
            target.draw(sprite);
        }

        text.setCharacterSize(5);
        text.setPosition(
            offset + sf::Vector2f { 10.f, 2.f }.componentWiseMul(scale));
        text.setString(card.name.data());
        target.draw(text);

        for (auto [idx, line] : std::ranges::views::enumerate(card.texts))
        {
            text.setPosition(
                offset
                + sf::Vector2f { 5.f, 76.f + 6.f * idx }.componentWiseMul(
                    scale));
            text.setString(line.data());
            target.draw(text);
        }

        text.setScale({ 1.f, 1.f });
        sprite.setScale({ 1.f, 1.f });
    }

    void
    renderCard(const Card& card, const sf::Vector2f& offset, float scale = 1.f)
    {
        renderCard(card, offset, { scale, scale });
    }

    void renderCard(
        const Card& card, const sf::Vector2f& offset, const sf::Vector2f& scale)
    {
        renderCard(window, card, offset, scale);
    }

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
