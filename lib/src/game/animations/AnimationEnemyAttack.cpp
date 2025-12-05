#include "game/animations/AnimationEnemyAttack.hpp"
#include "game/definitions/EasingFunctions.hpp"
#include "game/engine/RenderingEngine.hpp"

void AnimationEnemyAttack::render(
    const Scene& scene,
    const sf::Vector2f& baseOffset,
    std::function<void(const Card&, const Position&, const Scale&)> renderCard,
    std::function<void(const Position&, const Scale&)>) const
{
    const auto animationOffset = (RenderingEngine::getInfectionTextOffset()
                                  - (baseOffset + scene.mainCardBody.getSize()))
                                 * Easing::easeAttack(getPercFactor());
    renderCard(
        scene.deck.front(),
        Position(baseOffset + animationOffset),
        Scale({ 1.f, 1.f }));
};
