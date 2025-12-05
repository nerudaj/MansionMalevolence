#include "game/animations/AnimationInvalidOperation.hpp"
#include "game/definitions/EasingFunctions.hpp"
#include "game/engine/RenderingEngine.hpp"

void AnimationInvalidOperation::render(
    const Scene& scene,
    const sf::Vector2f& baseOffset,
    std::function<void(const Card&, const Position&, const Scale&)> renderCard,
    std::function<void(const Position&, const Scale&)>) const
{
    const auto animationOffset =
        sf::Vector2f { 76.f * 0.1f, 0.f }
        - baseOffset * Easing::easeDamage(getPercFactor());
    renderCard(
        scene.deck.front(),
        Position(baseOffset + animationOffset),
        Scale({ 1.f, 1.f }));
};
