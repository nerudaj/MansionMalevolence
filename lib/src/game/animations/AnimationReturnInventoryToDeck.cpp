#include "game/animations/AnimationReturnInventoryToDeck.hpp"
#include "game/definitions/EasingFunctions.hpp"
#include "game/engine/RenderingEngine.hpp"

void AnimationReturnInventoryToDeck::render(
    const Scene& scene,
    const sf::Vector2f& baseOffset,
    std::function<void(const Card&, const Position&, const Scale&)> renderCard,
    std::function<void(const Position&, const Scale&)>) const
{
    const auto animationOffset =
        (RenderingEngine::getTrashIconOffset() - baseOffset)
        * (1.f - Easing::easeInOut(getPercFactor()));

    const float scale = std::lerp(1.f / 3.f, 1.f, getPercFactor());

    renderCard(
        card, Position(baseOffset + animationOffset), Scale({ scale, scale }));

    renderCard(scene.deck.front(), Position(baseOffset), Scale({ 1.f, 1.f }));
}
