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

void RenderingEngine::operator()(const AnimationCardToDiscard& a)
{
    const auto easedF = Easing::easeValley2(a.perc);

    const auto flipOffset =
        sf::Vector2f { scene.mainCardBody.getSize().x * 0.5f, -3.f }
        * (1.f - easedF);
    const auto travelOffset =
        (scene.trashBody.getPosition() - a.origin) * Easing::easeInOut(a.perc);

    const auto position = a.origin + travelOffset + flipOffset;
    const auto scale =
        sf::Vector2f { easedF, 1.f } * std::lerp(1.f, 1.f / 3.f, a.perc);

    if (!scene.deck.empty())
        renderCard(scene.deck.front(), getMainCardOffset(), { 1.f, 1.f });

    if (a.perc <= 0.25f)
        renderCard(a.card, position, scale);
    else
        renderCardBack(position, scale);
}

void RenderingEngine::operator()(const AnimationCardTransform& a)
{
    const auto easedF = Easing::easeValley(a.perc);

    const auto preTransformCard = CardBuilder::createCard(a.cardType);
    const auto animationOffset =
        sf::Vector2f { 76.f * 0.5f, -3.f } * (1.f - easedF);

    renderCard(
        a.perc <= 0.5f ? preTransformCard : scene.deck.front(),
        getMainCardOffset() + animationOffset,
        { easedF, 1.f });
}

void RenderingEngine::operator()(const AnimationDiscardToDeck& a)
{
    const auto easedF = Easing::easeValley2(a.perc);

    const auto flipOffset =
        sf::Vector2f { scene.mainCardBody.getSize().x * 0.5f, -3.f }
        * (1.f - easedF);
    const auto travelOffset =
        (scene.trashBody.getPosition() - getMainCardOffset())
        * (1.f - Easing::easeInOut(a.perc));

    const auto position = getMainCardOffset() + travelOffset + flipOffset;
    const auto scale =
        sf::Vector2f { easedF, 1.f } * std::lerp(1.f / 3.f, 1.f, a.perc);

    if (a.perc <= 0.25f)
        renderCardBack(position, scale);
    else
        renderCard(scene.cardsToAdd.front(), position, scale);
}

void RenderingEngine::operator()(const AnimationDoorOpen& a)
{
    const auto easedF = Easing::easeValley(a.perc);

    const auto animationOffset =
        sf::Vector2f { 76.f * 0.5f, -3.f } * (1.f - easedF);

    if (a.perc <= 0.5f)
    {
        renderCard(
            scene.deck.front(),
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
    renderCard(
        scene.deck.front(),
        getMainCardOffset() + animationOffset,
        sf::Vector2f { 1.f, 1.f });
}

void RenderingEngine::operator()(const AnimationEnemyDamaged& a)
{
    const auto animationOffset =
        sf::Vector2f { 76.f * 0.1f, 76.f * 0.1f } * Easing::easeDamage(a.perc);
    renderCard(
        scene.deck.front(),
        getMainCardOffset() + animationOffset,
        { 1.f, 1.f });
}

void RenderingEngine::operator()(const AnimationEnemyDodgedAttack& a)
{
    const auto animationOffset =
        sf::Vector2f { -15.f, 0.f } * Easing::easeOutThenBack(a.perc);
    renderCard(
        scene.deck.front(),
        getMainCardOffset() + animationOffset,
        { 1.f, 1.f });
}

void RenderingEngine::operator()(const AnimationInvalidOperation& a)
{
    const auto animationOffset =
        sf::Vector2f { 4.f, 4.f } * Easing::easeDamage(a.perc);
    renderCard(
        scene.deck.front(),
        getMainCardOffset() + animationOffset,
        { 1.f, 1.f });
}

void RenderingEngine::operator()(const AnimationNewCardsShufflingIntoDeck& a)
{
    const auto count = scene.cardsToAdd.size();

    auto renderSecondTopDeckCard = [&]
    {
        if (count == 1 && scene.deck.size() > 1)
            renderCard(scene.deck.front(), getMainCardOffset(), { 1.f, 1.f });
        else if (count > 1)
            renderCardBack(getMainCardOffset(), { 1.f, 1.f });
    };

    const auto animationOffset =
        (scene.trashBody.getPosition() - getMainCardOffset())
        * Easing::easeInOut(a.perc);
    const auto scaleFactor = std::lerp(1.f, 1.f / 3.f, a.perc);

    if (a.perc < 0.5f)
    {
        renderSecondTopDeckCard();
        renderCardBack(
            getMainCardOffset() + animationOffset,
            { scaleFactor, scaleFactor });
    }
    else
    {
        renderCardBack(
            getMainCardOffset() + animationOffset,
            { scaleFactor, scaleFactor });
        renderSecondTopDeckCard();
    }
}

void RenderingEngine::operator()(const AnimationReturnInventoryToDeck& a)
{
    const auto animationOffset =
        (RenderingEngine::getTrashIconOffset() - getMainCardOffset())
        * (1.f - Easing::easeInOut(a.perc));

    const float scale = std::lerp(1.f / 3.f, 1.f, a.perc);

    renderCard(a.card, getMainCardOffset() + animationOffset, { scale, scale });

    renderCard(scene.deck.front(), getMainCardOffset(), { 1.f, 1.f });
}

void RenderingEngine::operator()(const AnimationTakeCard& a)
{
    const auto scale = std::lerp(1.f, 1.f / 3.f, a.perc);
    const auto animationOffset =
        (RenderingEngine::getNthInventoryCardOffset(a.inventorySlotIdx)
         - getMainCardOffset())
        * Easing::easeInOut(a.perc);

    if (!scene.deck.empty())
        renderCard(
            scene.deck.front(),
            getMainCardOffset() + animationOffset,
            { scale, scale });
}

void RenderingEngine::operator()(const AnimationTrashMainCard& a)
{
    const auto scale = std::lerp(1.f, 1.f / 10.f, a.perc);
    const auto animationOffset =
        sf::Vector2f { 0.f, -100.f } * Easing::easeInOut(a.perc);
    renderCard(
        scene.deck.front(),
        getMainCardOffset() + animationOffset,
        { scale, scale });
}

void RenderingEngine::operator()(const AnimationHeal& a)
{
    if (!scene.deck.empty())
        renderCard(scene.deck.front(), getMainCardOffset());

    const auto offset = sf::Vector2f { 63.f, 0.f } * a.perc;

    sprite.setTextureRect(atlas.getClip(iconsLocation)
                              .getFrame(std::to_underlying(Icon::HealEffect)));
    sprite.setPosition(sf::Vector2f { 8.f, 150.f } + offset);
    window.draw(sprite);
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
    if (!scene.discard.empty())
    {
        renderCardBack(

            scene.trashBody.getPosition(), sf::Vector2f { 1.f, 1.f } / 3.f);
    }

    if (scene.deck.size() > 1)
        renderCard(*(++scene.deck.begin()), scene.mainCardBody.getPosition());

    renderTopDeckCard();

    for (auto&& [idx, card] : std::ranges::views::enumerate(scene.inventory))
    {
        if (!card) continue;

        renderCard(
            card.value(),
            scene.dragDrop.value_or(DragDrop {}).inventoryIdx.value_or(-1)
                    == static_cast<size_t>(idx)
                ? scene.dragDrop.value_or(DragDrop {}).position
                : getNthInventoryCardOffset(idx),
            1.f / 3.f);
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
    for (auto i = 0; i < scene.hearts; ++i)
    {
        sprite.setPosition(getNthHeartOffset(i));
        window.draw(sprite);
    }

    text.setString(std::to_string(static_cast<int>(
        100.f * scene.stats.turnsTaken / scene.infectionLimit)));
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

void RenderingEngine::renderCard(
    const Card& card, const sf::Vector2f& offset, const sf::Vector2f& scale)
{
    text.setScale(scale);
    sprite.setScale(scale);

    sprite.setTextureRect(atlas.getClip(cardbgrLocation)
                              .getFrame(std::to_underlying(
                                  getAppropriateCardBackgroundType(card))));
    sprite.setPosition(offset);
    window.draw(sprite);

    sprite.setTextureRect(
        atlas.getClip(imagesLocation).getFrame(std::to_underlying(card.image)));
    sprite.setPosition(
        offset + sf::Vector2f { 10.f, 9.f }.componentWiseMul(scale));
    window.draw(sprite);

    auto& iconsClip = atlas.getClip(iconsLocation);
    sprite.setTextureRect(
        iconsClip.getFrame(std::to_underlying(Icon::BulletBig)));

    for (auto i = 0; i < card.quantity && card.traits & CardTrait::Weapon; ++i)
    {
        sprite.setPosition(
            offset + sf::Vector2f { 0.f, 13.f + i * 7.f } * scale.y);
        window.draw(sprite);
    }

    sprite.setTextureRect(iconsClip.getFrame(
        std::to_underlying(getAppropriateDiamondType(card))));

    for (auto i = 0; i < card.power; ++i)
    {
        sprite.setPosition(
            offset
            + sf::Vector2f { 2.f + i * 12.f, 102.f }.componentWiseMul(scale));
        window.draw(sprite);
    }

    text.setCharacterSize(5);
    text.setPosition(
        offset + sf::Vector2f { 10.f, 2.f }.componentWiseMul(scale));
    text.setString(card.name.data());
    window.draw(text);

    for (auto [idx, line] : std::ranges::views::enumerate(card.texts))
    {
        text.setPosition(
            offset
            + sf::Vector2f { 5.f, 76.f + 6.f * idx }.componentWiseMul(scale));
        text.setString(line.data());
        window.draw(text);
    }

    text.setScale({ 1.f, 1.f });
    sprite.setScale({ 1.f, 1.f });
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
    else if (!scene.deck.empty())
    {
        renderCard(scene.deck.front(), getMainCardOffset());
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
    const auto isDragRelevant = dragDrop.inventoryIdx.has_value();
    const auto dragPosition = dragDrop.position;
    const auto&& draggedCard =
        scene.inventory[dragDrop.inventoryIdx.value_or(0)].value_or(
            CardBuilder::createCard(CardType::Empty));

    if (!isDragRelevant) return BackgroundType::Plain;

    if (dgm::Collision::basic(scene.mainCardBody, dragPosition))
    {
        return GameRulesEngine::canCardInteractWithDeck(draggedCard, scene.deck)
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
