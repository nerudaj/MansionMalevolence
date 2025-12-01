#include "game/animations/AnimationTrashMainCard.hpp"
#include "game/definitions/EasingFunctions.hpp"
#include "game/engine/RenderingEngine.hpp"

void AnimationTrashMainCard::render(
    const Scene& scene,
    const sf::Vector2f& baseOffset,
    std::function<void(const Card&, const Position&, const Scale&)> renderCard,
    std::function<void(const Position&, const Scale&)>) const
{
    const auto scale = std::lerp(1.f, 1.f / 10.f, getPercFactor());
    const auto animationOffset =
        (RenderingEngine::getTrashIconOffset() - baseOffset)
        * Easing::easeInOut(getPercFactor());
    renderCard(
        scene.deck.front(),
        Position(baseOffset + animationOffset),
        Scale({ scale, scale }));
};
