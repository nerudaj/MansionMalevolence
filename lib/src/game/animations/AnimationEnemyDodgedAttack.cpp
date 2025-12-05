#include "game/animations/AnimationEnemyDodgedAttack.hpp"
#include "game/definitions/EasingFunctions.hpp"
#include "game/engine/RenderingEngine.hpp"

void AnimationEnemyDodgedAttack::render(
    const Scene& scene,
    const sf::Vector2f& baseOffset,
    std::function<void(const Card&, const Position&, const Scale&)> renderCard,
    std::function<void(const Position&, const Scale&)>) const
{
    const auto animationOffset =
        sf::Vector2f { -15.f, 0.f } * Easing::easeOutThenBack(getPercFactor());
    renderCard(
        scene.deck.front(),
        Position(baseOffset + animationOffset),
        Scale({ 1.f, 1.f }));
};
