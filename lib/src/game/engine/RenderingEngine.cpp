#include "game/engine/RenderingEngine.hpp"

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
    , text(resmgr.get<sf::Font>("ChunkFive-Regular.ttf"))
    , playbgrLocation(atlas.addImage(resmgr.get<sf::Texture>("playbgr.png")).value())
    , background(atlas.getTexture())
{
    background.setTextureRect(atlas.getClip(playbgrLocation).getFrame(0));
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
    window.setViewFromCamera(hudCamera);
    renderHud(window);

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
    window.draw(background);
}

void RenderingEngine::renderHud(dgm::Window& window)
{
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
