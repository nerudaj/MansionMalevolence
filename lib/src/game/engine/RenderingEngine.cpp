#include "game/engine/RenderingEngine.hpp"
#include "game/engine/GameRulesEngine.hpp"
#include "game/enums/BackgroundType.hpp"
#include "game/enums/Icon.hpp"

RenderingEngine::RenderingEngine(
    dgm::ResourceManager& resmgr,
    const Scene& scene,
    const AppSettings& settings,
    const TouchController& touchController) noexcept
    : scene(scene)
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
    , cardbgrLocation(
          atlas.addImage(resmgr.get<sf::Texture>("cardbgr.png")).value())
    , iconsLocation(atlas
                        .addTileset(
                            resmgr.get<sf::Texture>("icons.png"),
                            resmgr.get<dgm::Clip>("icons.png.clip"))
                        .value())
    , imagesLocation(atlas
                         .addTileset(
                             resmgr.get<sf::Texture>("illustrations.png"),
                             resmgr.get<dgm::Clip>("illustrations.png.clip"))
                         .value())
    , sprite(atlas.getTexture())
{
    resmgr.getMutable<sf::Font>("pico-8.ttf").setSmooth(false);
    text.setFillColor(sf::Color { 0xff, 0xf1, 0xe8 });
}

void RenderingEngine::update(const dgm::Time& time)
{
    fpsCounter.update(time.getDeltaTime());
}

void RenderingEngine::draw(dgm::Window& window)
{
    window.setViewFromCamera(worldCamera);

    renderBackground(window);
    renderHud(window);
    renderWorld(window);

    // Restore camera view for menus, etc
    window.setViewFromCamera(hudCamera);

    if (settings.input.showTouchControls)
    {
        renderTouchControls(window);
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

void RenderingEngine::renderWorld(dgm::Window& window)
{
    renderTopDeckCard(window);

    for (auto&& [idx, card] : std::ranges::views::enumerate(scene.inventory))
    {
        if (!card) continue;

        renderCard(
            window,
            card.value(),
            scene.dragDrop.value_or(DragDrop {}).inventoryIdx.value_or(-1)
                    == static_cast<size_t>(idx)
                ? scene.dragDrop.value_or(DragDrop {}).position
                : getNthInventoryCardOffset(idx),
            1.f / 3.f);
    }
}

void RenderingEngine::renderBackground(dgm::Window& window)
{
    sprite.setTextureRect(
        atlas.getClip(playbgrLocation)
            .getFrame(std::to_underlying(getAppropriateBackgroundType())));
    sprite.setPosition({ 0.f, 0.f });
    window.draw(sprite);
}

void RenderingEngine::renderHud(dgm::Window& window)
{
    // Pause button
    auto& iconsClip = atlas.getClip(iconsLocation);
    sprite.setTextureRect(iconsClip.getFrame(std::to_underlying(Icon::Pause)));
    sprite.setPosition({ 3.f, 3.f });
    window.draw(sprite);

    // Take button
    sprite.setTextureRect(iconsClip.getFrame(std::to_underlying(
        scene.deck.front().traits & CardTrait::Pickable ? Icon::GreenTake
                                                        : Icon::RedTake)));
    sprite.setPosition({ 96.f, 48.f });
    window.draw(sprite);

    // Skip button
    sprite.setTextureRect(iconsClip.getFrame(std::to_underlying(
        scene.deck.front().traits & CardTrait::Enemy ? Icon::RedSkip
                                                     : Icon::GreenSkip)));
    sprite.setPosition({ 96.f, 110.f });
    window.draw(sprite);

    // Trash button
    sprite.setTextureRect(iconsClip.getFrame(std::to_underlying(Icon::Trash)));
    sprite.setPosition(getTrashIconOffset());
    window.draw(sprite);

    // Hearts
    sprite.setTextureRect(iconsClip.getFrame(std::to_underlying(Icon::Heart)));
    for (auto i = 0; i < scene.hearts; ++i)
    {
        sprite.setPosition(getNthHeartOffset(i));
        window.draw(sprite);
    }

    text.setPosition({ 0.f, 0.f });
    text.setString(fpsCounter.getText());
    window.draw(text);

    /*scene.mainCardBody.debugRender(window);
    scene.healthbarBody.debugRender(window);
    scene.trashBody.debugRender(window);
    scene.inventoryBodies[0].debugRender(window);
    scene.inventoryBodies[1].debugRender(window);
    scene.inventoryBodies[2].debugRender(window);*/
}

void RenderingEngine::renderTouchControls(dgm::Window& window)
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
    dgm::Window& window,
    const Card& card,
    const sf::Vector2f& offset,
    float scale)
{
    text.setScale({ scale, scale });
    sprite.setScale({ scale, scale });

    sprite.setTextureRect(atlas.getClip(cardbgrLocation).getFrame(0));
    sprite.setPosition(offset);
    window.draw(sprite);

    sprite.setTextureRect(
        atlas.getClip(imagesLocation).getFrame(std::to_underlying(card.image)));
    sprite.setPosition(offset + sf::Vector2f { 10.f, 9.f } * scale);
    window.draw(sprite);

    auto& iconsClip = atlas.getClip(iconsLocation);
    if (card.quantity > 0 && card.traits & CardTrait::Weapon)
    {
        sprite.setTextureRect(
            iconsClip.getFrame(std::to_underlying(Icon::Bullet)));

        for (auto i = 0; i < card.quantity; ++i)
        {
            sprite.setPosition(
                offset + sf::Vector2f { 0.f, 13.f + i * 6.f } * scale);
            window.draw(sprite);
        }
    }

    if (card.power > 0)
    {
        sprite.setTextureRect(
            iconsClip.getFrame(std::to_underlying(Icon::Diamond)));

        for (auto i = 0; i < card.power; ++i)
        {
            sprite.setPosition(
                offset + sf::Vector2f { 2.f + i * 12.f, 102.f } * scale);
            window.draw(sprite);
        }
    }

    text.setCharacterSize(5);
    text.setPosition(offset + sf::Vector2f { 10.f, 2.f } * scale);
    text.setString(card.name.data());
    window.draw(text);

    // Text description
    text.setPosition(offset + sf::Vector2f { 4.f, 77.f } * scale);
    text.setString(card.text1.data());
    window.draw(text);

    text.setPosition(offset + sf::Vector2f { 4.f, 83.f } * scale);
    text.setString(card.text2.data());
    window.draw(text);

    text.setPosition(offset + sf::Vector2f { 4.f, 89.f } * scale);
    text.setString(card.text3.data());
    window.draw(text);

    text.setPosition(offset + sf::Vector2f { 4.f, 95.f } * scale);
    text.setString(card.text4.data());
    window.draw(text);

    text.setScale({ 1.f, 1.f });
    sprite.setScale({ 1.f, 1.f });
}

static float easeInOut(float x)
{
    return x < 0.5f ? 4 * std::pow(x, 3.f) : 1 - std::pow(-2 * x + 2, 3) / 2;
}

static float easeOutThenBack(float x)
{
    return -4.f * std::pow(x - 0.5f, 2.f) + 1;
}

static float easeAttack(float x)
{
    // note: wolframalpha.com
    // prompt: interpolating polynomial | {point1} ... {pointN}
    if (x < 0.4f)
        return 12.5f * x * x - 5.f * x;
    else if (x < 0.6f)
        return 5.f * (x - 0.4f);
    else if (x < 0.7f)
        return 1.f;
    return 193.333f * std::pow(x, 3.f) - 491.f * x * x + 407.967f * x - 110.3f;
}

static float easeDamage(float x)
{
    return std::sin(x * 10.f);
}

void RenderingEngine::renderTopDeckCard(dgm::Window& window)
{
    const auto deckOffset = getDeckCardOffset();

    if (scene.activeAnimation)
    {
        const float f =
            scene.activeAnimation->elapsed / scene.activeAnimation->duration;

        if (scene.activeAnimation->kind == AnimationKind::TakeCard)
        {
            const auto animationOffset =
                (getNthInventoryCardOffset(scene.activeAnimation->data)
                 - deckOffset)
                * easeInOut(f);

            renderCard(
                window,
                scene.deck.front(),
                deckOffset + animationOffset,
                std::lerp(1.f, 1.f / 3.f, f));
        }
        else if (scene.activeAnimation->kind == AnimationKind::SkipCard)
        {
            const auto animationOffset =
                (sf::Vector2f { deckOffset.x + 76.f * 1.5f, deckOffset.y }
                 - deckOffset)
                * easeOutThenBack(f);

            if (f < 0.5f)
            {
                renderSecondTopDeckCard(window);
                renderCard(
                    window, scene.deck.front(), deckOffset + animationOffset);
            }
            else
            {
                renderCard(
                    window, scene.deck.front(), deckOffset + animationOffset);
                renderSecondTopDeckCard(window);
            }
        }
        else if (scene.activeAnimation->kind == AnimationKind::TrashMainCard)
        {
            const auto animationOffset =
                (getTrashIconOffset() - deckOffset) * easeInOut(f);
            renderCard(
                window,
                scene.deck.front(),
                deckOffset + animationOffset,
                std::lerp(1.f, 1.f / 10.f, f));
        }
        else if (scene.activeAnimation->kind == AnimationKind::EnemyAttack)
        {
            const auto animationOffset =
                (getNthHeartOffset(0) - deckOffset) * easeAttack(f);
            renderCard(
                window, scene.deck.front(), deckOffset + animationOffset);
        }
        else if (scene.activeAnimation->kind == AnimationKind::EnemyDamaged)
        {
            const auto animationOffset =
                (sf::Vector2f { deckOffset.x + 76.f * 0.1f,
                                deckOffset.y + 76.f * 0.1f }
                 - deckOffset)
                * easeDamage(f);
            renderCard(
                window, scene.deck.front(), deckOffset + animationOffset);
        }
        else
        {
            renderCard(window, scene.deck.front(), deckOffset);
        }
    }
    else
    {
        renderCard(window, scene.deck.front(), deckOffset);
    }
}

void RenderingEngine::renderSecondTopDeckCard(dgm::Window& window)
{
    if (scene.deck.size() > 1)
        renderCard(window, *(++scene.deck.begin()), getDeckCardOffset());
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
