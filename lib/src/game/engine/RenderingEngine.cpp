#include "game/engine/RenderingEngine.hpp"
#include "game/definitions/EasingFunctions.hpp"
#include "game/engine/GameRulesEngine.hpp"

const auto WHITE_COLOR = sf::Color { 0xff, 0xf1, 0xe8 };
const auto RED_COLOR = sf::Color { 0xff, 0x00, 0x4d };

RenderingEngine::RenderingEngine(
    dgm::Window& window,
    dgm::ResourceManager& resmgr,
    const Scene& scene,
    const AppSettings& settings,
    const TouchController& touchController) noexcept
    : window(window)
    , scene(scene)
    , settings(settings)
    , touchController(touchController)
    , atlas(1024, 1024)
    , worldCamera(createFullscreenCamera(
          sf::Vector2f(settings.video.resolution), INTERNAL_GAME_RESOLUTION))
    , hudCamera(
          sf::FloatRect { { 0.f, 0.f }, { 1.f, 1.f } },
          sf::Vector2f(settings.video.resolution))
    , text(resmgr.get<sf::Font>("pico-8.ttf"))
    , playbgrLocation(atlas
                          .addTileset(
                              resmgr.get<sf::Texture>("playbgr.png"),
                              resmgr.get<dgm::Clip>("playbgr.png.clip"))
                          .value())
    , cardbgrLocation(atlas
                          .addTileset(
                              resmgr.get<sf::Texture>("cardbgr.png"),
                              resmgr.get<dgm::Clip>("cardbgr.png.clip"))
                          .value())
    , iconsLocation(
          atlas
              .addTileset(
                  resmgr.get<sf::Texture>("zombie_cards_icons.png"),
                  resmgr.get<dgm::Clip>("zombie_cards_icons.png.clip"))
              .value())
    , imagesLocation(
          atlas
              .addTileset(
                  resmgr.get<sf::Texture>("zombie_cards_illustrations.png"),
                  resmgr.get<dgm::Clip>("zombie_cards_illustrations.png.clip"))
              .value())
    , sprite(atlas.getTexture())
{
    resmgr.getMutable<sf::Font>("pico-8.ttf").setSmooth(false);
    text.setFillColor(WHITE_COLOR);
    worldCamera.setPosition(INTERNAL_GAME_RESOLUTION / 2.f);
}

void RenderingEngine::update(const dgm::Time& time)
{
    fpsCounter.update(time.getDeltaTime());
    worldCamera.update(time);
}

void RenderingEngine::draw()
{
    window.setViewFromCamera(worldCamera);

    renderBackground();
    renderHud();
    renderWorld();

    // Restore camera view for menus, etc
    window.setViewFromCamera(hudCamera);

    if (settings.input.showTouchControls)
    {
        renderTouchControls();
    }
}

void RenderingEngine::operator()(const AnimationMainCardToDiscard& a)
{
    const auto transitionOffset =
        (scene.trashBody.getPosition() - a.origin) * Easing::easeInOut(a.perc);
    const auto scale = std::lerp(1.f, 1.f / 3.f, Easing::easeInOut(a.perc));

    renderCard(*scene.mainCard, a.origin + transitionOffset, scale);
}

void RenderingEngine::operator()(const AnimationInventoryCardToDiscard& a)
{
    const auto transitionOffset =
        (scene.trashBody.getPosition() - a.origin) * Easing::easeInOut(a.perc);

    if (scene.mainCard) renderCard(*scene.mainCard, getMainCardOffset());
    renderCard(a.card, a.origin + transitionOffset, 1.f / 3.f);
}

void RenderingEngine::operator()(const AnimationCardTransform& a)
{
    const auto easedF = Easing::easeValley(a.perc);

    const auto preTransformCard = CardBuilder::createCard(a.cardType);
    const auto animationOffset =
        sf::Vector2f { 76.f * 0.5f, -3.f } * (1.f - easedF);

    renderCard(
        a.perc <= 0.5f ? preTransformCard : *scene.mainCard,
        getMainCardOffset() + animationOffset,
        { easedF, 1.f });
}

void RenderingEngine::operator()(const AnimationReturnDiscardToDeck& a)
{
    const auto direction = getDeckOffset() - scene.trashBody.getPosition();
    const auto transitionOffset = direction * a.perc;

    const auto easedF = Easing::easeValley(a.perc);
    const auto flipOffset =
        sf::Vector2f(CARD_SIZE.x / 3.f / 2.f, -6.f) * (1.f - easedF);

    if (a.perc <= 0.5f)
    {
        renderCard(
            scene.cardsToAdd.front(),
            scene.trashBody.getPosition() + transitionOffset + flipOffset,
            sf::Vector2f { easedF, 1.f } / 3.f);
    }
    else
    {
        renderCardBack(
            scene.trashBody.getPosition() + transitionOffset + flipOffset,
            sf::Vector2f { easedF, 1.f } / 3.f);
    }
}

void RenderingEngine::operator()(const AnimationDoorOpen& a)
{
    const auto easedF = Easing::easeValley(a.perc);

    const auto animationOffset =
        sf::Vector2f { 76.f * 0.5f, -3.f } * (1.f - easedF);

    if (a.perc <= 0.5f)
    {
        renderCard(
            *scene.mainCard,
            getMainCardOffset() + animationOffset,
            sf::Vector2f { easedF, 1.f });
    }
    else
    {
        renderCardBack(
            getMainCardOffset() + animationOffset,
            sf::Vector2f { easedF, 1.f });
    }
}

void RenderingEngine::operator()(const AnimationEnemyAttack& a)
{
    if (a.perc >= 0.6f && !worldCamera.isShaking())
    {
        worldCamera.shake(sf::seconds(a.duration.asSeconds() * 0.4f), 5.f);
    }

    const auto animationOffset =
        (RenderingEngine::getInfectionTextOffset()
         - (getMainCardOffset() + scene.mainCardBody.getSize()))
        * Easing::easeAttack(a.perc);
    renderCard(*scene.mainCard, getMainCardOffset() + animationOffset);
}

void RenderingEngine::operator()(const AnimationEnemyDamagedWindup&)
{
    renderCard(*scene.mainCard, getMainCardOffset());
}

void RenderingEngine::operator()(const AnimationEnemyDamaged& a)
{
    const float factor =
        a.elapsed.asSeconds() <= 0.25 ? a.elapsed.asSeconds() / 0.25f : 0.f;

    const auto animationOffset =
        sf::Vector2f { 76.f * 0.1f, 76.f * 0.1f } * Easing::easeDamage(factor);
    renderCard(*scene.mainCard, getMainCardOffset() + animationOffset);

    if (a.perc >= 1.f) return;

    const size_t iconOffset = a.impactIconId == Icon::Explosion1
                                  ? static_cast<size_t>(a.perc * 10 / 2)
                                  : 0u;

    sprite.setTextureRect(
        atlas.getClip(iconsLocation)
            .getFrame(std::to_underlying(a.impactIconId) + iconOffset));
    sprite.setPosition(
        getMainCardOffset() + sf::Vector2f(30.f, 30.f) + animationOffset);
    window.draw(sprite);
}

void RenderingEngine::operator()(const AnimationEnemyDodgedAttack& a)
{
    const auto animationOffset =
        sf::Vector2f { -15.f, 0.f } * Easing::easeOutThenBack(a.perc);
    renderCard(*scene.mainCard, getMainCardOffset() + animationOffset);
}

void RenderingEngine::operator()(const AnimationInvalidOperation& a)
{
    const auto animationOffset =
        sf::Vector2f { 4.f, 4.f } * Easing::easeDamage(a.perc);
    renderCard(*scene.mainCard, getMainCardOffset() + animationOffset);
}

void RenderingEngine::operator()(const AnimationNewCardsShufflingIntoDeck& a)
{
    const auto count = scene.cardsToAdd.size();

    const auto animationOffset =
        (getDeckOffset() - getMainCardOffset()) * Easing::easeInOut(a.perc);
    const auto scaleFactor = std::lerp(1.f, 1.f / 4.f, a.perc);

    if (count > 1) renderCardBack(getMainCardOffset());
    renderCardBack(getMainCardOffset() + animationOffset, scaleFactor);

    // Render deck over the shuffled card
    if (!scene.deck.empty()) renderCardBack(getDeckOffset(), 1.f / 3.f);
}

void RenderingEngine::operator()(const AnimationTakeCard& a)
{
    const auto scale = std::lerp(1.f, 1.f / 3.f, a.perc);
    const auto animationOffset =
        (getNthInventoryCardOffset(a.inventorySlotIdx) - a.origin)
        * Easing::easeInOut(a.perc);

    renderCard(*scene.mainCard, a.origin + animationOffset, { scale, scale });
}

void RenderingEngine::operator()(const AnimationTrashMainCard& a)
{
    const auto scale = std::lerp(1.f, 1.f / 10.f, a.perc);
    const auto animationOffset =
        sf::Vector2f { 0.f, -100.f } * Easing::easeInOut(a.perc);
    renderCard(
        *scene.mainCard,
        getMainCardOffset() + animationOffset,
        { scale, scale });
}

void RenderingEngine::operator()(const AnimationHeal& a)
{
    if (!scene.deck.empty()) renderCard(*scene.mainCard, getMainCardOffset());

    const auto travelAmount =
        (scene.hearts + a.healAmount) * 63.f / scene.maxHearts;
    const auto offset = sf::Vector2f { travelAmount, 0.f } * a.perc;

    sprite.setTextureRect(atlas.getClip(iconsLocation)
                              .getFrame(std::to_underlying(Icon::HealEffect)));
    sprite.setPosition(sf::Vector2f { 8.f, 150.f } + offset);
    window.draw(sprite);
}

void RenderingEngine::operator()(const AnimationReturnDraggedMainCard& a)
{
    const auto animationOffset =
        (a.origin - getMainCardOffset()) * Easing::easeInOut(1.f - a.perc);
    const auto scale = std::lerp(0.5f, 1.f, Easing::easeInOut(a.perc));

    renderCard(*scene.mainCard, getMainCardOffset() + animationOffset, scale);
}

void RenderingEngine::operator()(const AnimationDrawCard& a)
{
    const auto direction = getMainCardOffset() - getDeckOffset();
    const auto transitionOffset = direction * a.perc;

    const auto cardWidth = std::lerp(CARD_SIZE.x / 3.f, CARD_SIZE.x, a.perc);
    const auto easedF = Easing::easeValley(a.perc);
    const auto flipOffset =
        sf::Vector2f(cardWidth / 2.f, -6.f) * (1.f - easedF);

    const auto scaleDownFactor = std::lerp(3.f, 1.f, a.perc);

    if (a.perc <= 0.5f)
    {
        renderCardBack(
            getDeckOffset() + transitionOffset + flipOffset,
            sf::Vector2f { easedF, 1.f } / scaleDownFactor);
    }
    else
    {
        renderCard(
            scene.deck.front(),
            getDeckOffset() + transitionOffset + flipOffset,
            sf::Vector2f { easedF, 1.f } / scaleDownFactor);
    }
}

dgm::Camera RenderingEngine::createFullscreenCamera(
    const sf::Vector2f& currentResolution,
    const sf::Vector2f& desiredResolution)
{
    auto&& viewport = sf::FloatRect {
        { 0.f, 0.f },
        { 1.f, 1.f },
    };

    const auto&& desiredAspectRatio = desiredResolution.x / desiredResolution.y;
    const auto&& currentAspectRatio = currentResolution.x / currentResolution.y;

    // When black bars are up and down then we need to normalize the desired
    // Y resolution to match the current Y resolution proportionally
    // and get the ratio for the viewport.
    if (desiredAspectRatio > currentAspectRatio)
    { // black bars up and down
        const float m = currentResolution.x / desiredResolution.x;
        viewport.size.y = m * desiredResolution.y / currentResolution.y;
    }
    // Same as above, but for the X resolution
    else
    { // black bars left and right
        const float m = currentResolution.y / desiredResolution.y;
        viewport.size.x = m * desiredResolution.x / currentResolution.x;
    }

    // one of the sizes will be 1, so NOP
    // the other one will be <1, so it will be centered
    viewport.position.x = (1.f - viewport.size.x) / 2.f;
    viewport.position.y = (1.f - viewport.size.y) / 2.f;

    return dgm::Camera(viewport, sf::Vector2f(desiredResolution));
}

void RenderingEngine::renderWorld()
{
    const auto draggedInventoryCardIdx =
        scene.dragDrop.value_or(DragDrop {})
            .inventoryIdx.value_or(std::numeric_limits<size_t>::max());

    for (auto&& [idx, card] : std::ranges::views::enumerate(scene.inventory))
    {
        if (!card) continue;
        if (draggedInventoryCardIdx == static_cast<size_t>(idx)) continue;

        renderCard(card.value(), getNthInventoryCardOffset(idx), 1.f / 3.f);
    }

    if (!scene.deck.empty()) renderCardBack(getDeckOffset(), 1.f / 3.f);
    if (!scene.discard.empty())
        renderCard(
            scene.discard.back(), scene.trashBody.getPosition(), 1.f / 3.f);

    renderTopDeckCard();

    if (draggedInventoryCardIdx != std::numeric_limits<size_t>::max())
    {
        renderCard(
            *scene.inventory[draggedInventoryCardIdx],
            scene.dragDrop.value_or(DragDrop {}).position,
            1 / 3.f);
    }

    renderBoosterChoice();
}

void RenderingEngine::renderBackground()
{
    sprite.setTextureRect(
        atlas.getClip(playbgrLocation)
            .getFrame(std::to_underlying(getAppropriateBackgroundType())));
    sprite.setPosition({ 0.f, 0.f });
    window.draw(sprite);
}

void RenderingEngine::renderHud()
{
    // Pause button
    auto& iconsClip = atlas.getClip(iconsLocation);
    sprite.setTextureRect(iconsClip.getFrame(std::to_underlying(Icon::Pause)));
    sprite.setPosition({ 3.f, 3.f });
    window.draw(sprite);

    // Hearts
    sprite.setTextureRect(iconsClip.getFrame(std::to_underlying(Icon::Heart)));
    const auto isHealAnimationActive =
        scene.activeAnimation
        && std::holds_alternative<AnimationHeal>(*scene.activeAnimation);
    const auto healAnimation =
        isHealAnimationActive ? std::get<AnimationHeal>(*scene.activeAnimation)
                              : AnimationHeal(0);
    int amountOfHeartsToRender =
        scene.hearts
        + static_cast<int>(healAnimation.healAmount * healAnimation.perc);
    for (auto i = 0; i < amountOfHeartsToRender; ++i)
    {
        sprite.setPosition(getNthHeartOffset(i));
        window.draw(sprite);
    }

    text.setString(std::to_string(static_cast<int>(
        100.f * scene.infection.progress / scene.infection.limit)));
    text.setFillColor(RED_COLOR);
    text.setScale({ 2.f, 2.f });
    text.setPosition(getInfectionTextOffset());
    window.draw(text);
    text.setFillColor(WHITE_COLOR);

    /*
    text.set{ 0.f, 0.f });
    text.setString(fpsCounter.getText());
    window.draw(text);
    */

    /*scene.mainCardBody.debugRender(window);
    scene.healthbarBody.debugRender(window);
    scene.trashBody.debugRender(window);
    scene.inventoryBodies[0].debugRender(window);
    scene.inventoryBodies[1].debugRender(window);
    scene.inventoryBodies[2].debugRender(window);*/
}

void RenderingEngine::renderTouchControls()
{
    auto& model = touchController.getTouchModel();
    dgm::Circle thumb({ 0.f, 0.f }, 16.f);

    for (auto& object : model.objects)
    {
        object.touchArea.debugRender(window, sf::Color(128, 128, 128, 128));
        if (object.kind == TouchObjectKind::Joystick)
        {
            thumb.setPosition(object.touchPosition);
            thumb.debugRender(window, sf::Color(128, 128, 128));
        }
    }
}

void RenderingEngine::renderCardBack(
    const sf::Vector2f& offset, const sf::Vector2f& scale)
{
    sprite.setScale(scale);
    sprite.setTextureRect(
        atlas.getClip(cardbgrLocation)
            .getFrame(std::to_underlying(CardBackground::Backside)));
    sprite.setPosition(offset);
    window.draw(sprite);
    sprite.setScale({ 1.f, 1.f });
}

void RenderingEngine::renderTopDeckCard()
{
    if (scene.activeAnimation)
    {
        std::visit(*this, *scene.activeAnimation);
    }
    else if (scene.mainCard)
    {
        const auto isDraggingMainCard =
            scene.dragDrop.value_or(DragDrop {}).draggingMainCard;

        if (isDraggingMainCard && !scene.deck.empty())
            renderCardBack(getMainCardOffset());

        const auto offset =
            isDraggingMainCard ? scene.dragDrop->position : getMainCardOffset();
        renderCard(*scene.mainCard, offset, isDraggingMainCard ? 0.5f : 1.f);
    }
}

void RenderingEngine::renderBoosterChoice()
{
    if (!scene.boosterChoice) return;

    sprite.setTextureRect(
        atlas.getClip(playbgrLocation)
            .getFrame(std::to_underlying(BackgroundType::PickOne)));
    sprite.setPosition({ 0.f, 0.f });
    window.draw(sprite);

    for (auto&& [idx, cardType] :
         std::ranges::views::enumerate(scene.boosterChoice.value()))
    {
        renderCard(
            CardBuilder::createCard(cardType),
            getNthBoosterChoiceOffset(idx),
            1.f / 3.f);
    }
}

BackgroundType RenderingEngine::getAppropriateBackgroundType() const
{
    const auto dragDrop = scene.dragDrop.value_or(DragDrop {});
    const auto dragPosition = dragDrop.position;
    const auto&& draggedCard =
        scene.inventory[dragDrop.inventoryIdx.value_or(0)].value_or(
            CardBuilder::createCard(CardType::Empty));

    if (scene.dragDrop && !scene.dragDrop->inventoryIdx
        && !scene.dragDrop->draggingMainCard)
        return BackgroundType::Plain;

    if (dragDrop.draggingMainCard)
    {
        if (dgm::Collision::basic(scene.trashBody, dragPosition))
        {
            return BackgroundType::Trash;
        }
        else if (dgm::Collision::basic(scene.wholeInventoryBody, dragPosition))
        {
            return scene.canTakeCard ? BackgroundType::InventoryWhole
                                     : BackgroundType::InventoryWholeGreyed;
        }

        return BackgroundType::Plain;
    }

    if (dgm::Collision::basic(scene.mainCardBody, dragPosition))
    {
        assert(scene.mainCard);
        return GameRulesEngine::canCardInteractWithMainCard(
                   draggedCard, *scene.mainCard)
                   ? BackgroundType::MainCard
                   : BackgroundType::MainCardGreyed;
    }
    else if (dgm::Collision::basic(scene.healthbarBody, dragPosition))
    {
        return draggedCard.traits & CardTrait::Healing
                   ? BackgroundType::HealthBar
                   : BackgroundType::HealthBarGreyed;
    }
    else if (dgm::Collision::basic(scene.trashBody, dragPosition))
    {
        return BackgroundType::Trash;
    }

    for (auto&& [idx, inventoryBody] :
         std::views::enumerate(scene.inventoryBodies))
    {
        const bool isIdxOfDraggedItem = static_cast<size_t>(idx)
                                        == dragDrop.inventoryIdx.value_or(
                                            std::numeric_limits<size_t>::max());
        if (!scene.inventory[idx].has_value() || isIdxOfDraggedItem) continue;

        if (dgm::Collision::basic(inventoryBody, dragPosition))
        {
            return static_cast<BackgroundType>(
                GameRulesEngine::canInventoryCardCombineWithIncoming(
                    scene.inventory[idx].value(), draggedCard)
                    ? std::to_underlying(BackgroundType::Inventory1) + 2 * idx
                    : std::to_underlying(BackgroundType::Inventory1Greyed)
                          + 2 * idx);
        }
    }

    return BackgroundType::Plain;
}

CardBackground
RenderingEngine::getAppropriateCardBackgroundType(const Card& card) const
{
    if (card.power == 0)
        return CardBackground::NoAmmoNoDiamonds;
    else if (card.traits & CardTrait::Weapon)
        return CardBackground::AmmoAndDiamonds;
    return CardBackground::NoAmmoWithDiamonds;
}

Icon RenderingEngine::getAppropriateDiamondType(const Card& card) const
{
    if (card.traits & CardTrait::Enemy)
        return Icon::RedDiamond;
    else if (card.traits & CardTrait::Healing)
        return Icon::GreenDiamond;
    else if (card.traits & CardTrait::Weapon)
        return Icon::YellowDiamond;
    return Icon::Diamond;
}
