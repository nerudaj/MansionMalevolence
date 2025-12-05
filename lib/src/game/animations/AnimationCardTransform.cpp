#include "game/animations/AnimationCardTransform.hpp"
#include "game/builders/CardBuilder.hpp"
#include "game/definitions/EasingFunctions.hpp"
#include "game/definitions/Scene.hpp"

void AnimationCardTransform::render(
    const Scene& scene,
    const sf::Vector2f& baseOffset,
    std::function<void(const Card&, const Position&, const Scale&)> renderCard,
    std::function<void(const Position&, const Scale&)>) const
{
    const auto easedF = Easing::easeValley(getPercFactor());

    const auto preTransformCard = CardBuilder::createCard(cardType);
    const auto animationOffset =
        sf::Vector2f { 76.f * 0.5f, -3.f } * (1.f - easedF);

    renderCard(
        getPercFactor() <= 0.5f ? preTransformCard : scene.deck.front(),
        Position(baseOffset + animationOffset),
        Scale({ easedF, 1.f }));
};
