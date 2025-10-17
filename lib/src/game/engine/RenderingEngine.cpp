#include "game/engine/RenderingEngine.hpp"
#include "game/enums/Icon.hpp"

RenderingEngine::RenderingEngine(
    dgm::ResourceManager& resmgr,
    Scene& scene,
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
    , playbgrLocation(
          atlas.addImage(resmgr.get<sf::Texture>("playbgr.png")).value())
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
    // Render the game world
    window.setViewFromCamera(worldCamera);
    renderWorld(window);

    // Render everything hud-related
    renderHud(window);

    if (settings.input.showTouchControls)
    {
        renderTouchControls(window);
    }

    // Restore camera view for menus, etc
    window.setViewFromCamera(hudCamera);
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

    if (currentAspectRatio > desiredAspectRatio)
    {
        // Narrow desired into wider current
        viewport.size.x = desiredResolution.y / currentResolution.y;
        viewport.position.x = (1.f - viewport.size.x) / 2.f;
    }
    else if (currentAspectRatio < desiredAspectRatio)
    {
        // Wider desired into narrower current
        viewport.size.y = desiredResolution.x / currentResolution.x;
        viewport.position.y = (1.f - viewport.size.y) / 2.f;
    }

    return dgm::Camera(viewport, sf::Vector2f(desiredResolution));
}

void RenderingEngine::renderWorld(dgm::Window& window)
{
    sprite.setTextureRect(atlas.getClip(playbgrLocation).getFrame(0));
    sprite.setPosition({ 0.f, 0.f });
    window.draw(sprite);

    renderCard(window, scene.deck.front(), { 7.f, 35.f });

    for (auto&& [idx, card] : uniranges::enumerate_view(scene.inventory))
    {
        if (!card) continue;

        renderCard(
            window, card.value(), { 8.f + idx * 43.5f, 161.f }, 1.f / 3.f);
    }
}

void RenderingEngine::renderHud(dgm::Window& window)
{
    // Pause button
    auto& iconsClip = atlas.getClip(iconsLocation);
    sprite.setTextureRect(iconsClip.getFrame(std::to_underlying(Icon::Pause)));
    sprite.setPosition({ 3.f, 3.f });
    window.draw(sprite);

    // Skip button
    sprite.setTextureRect(iconsClip.getFrame(std::to_underlying(
        scene.deck.front().bites ? Icon::RedSkip : Icon::GreenSkip)));
    sprite.setPosition({ 96.f, 110.f });
    window.draw(sprite);

    // Trash button
    sprite.setTextureRect(iconsClip.getFrame(std::to_underlying(Icon::Trash)));
    sprite.setPosition({ 54.f, 204.f });
    window.draw(sprite);

    // Hearts
    sprite.setTextureRect(iconsClip.getFrame(std::to_underlying(Icon::Heart)));
    for (auto i = 0; i < scene.hearts; ++i)
    {
        sprite.setPosition({ 31.f + i * 18.f, 3.f });
        window.draw(sprite);
    }

    text.setPosition({ 0.f, 0.f });
    text.setString(fpsCounter.getText());
    window.draw(text);
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
    if (card.ammo > 0)
    {
        sprite.setTextureRect(
            iconsClip.getFrame(std::to_underlying(Icon::Bullet)));

        for (auto i = 0; i < card.ammo; ++i)
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
