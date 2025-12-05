#include "game/animations/AnimationCardToDiscard.hpp"
#include "game/definitions/EasingFunctions.hpp"
#include "game/engine/RenderingEngine.hpp"

void AnimationCardToDiscard::render(
    const Scene& scene,
    const sf::Vector2f& baseOffset,
    std::function<void(const Card&, const Position&, const Scale&)> renderCard,
    std::function<void(const Position&, const Scale&)> renderCardBack) const
{
    const auto flipEasedF = Easing::easeValley(getPercFactor());
    const auto travelEasedF = Easing::easeInOut(getPercFactor());

    const auto travelAnimationOffset =
        (scene.trashBody.getPosition() - origin) * travelEasedF;
    const auto position = Position(origin + travelAnimationOffset);
    const auto scale = Scale(
        sf::Vector2f { flipEasedF, 1.f }
        * std::lerp(1.f, 1.f / 3.f, getPercFactor()));

    if (getPercFactor() < 0.5f)
    {
        renderCard(card, position, scale);
    }
    else
    {
        renderCardBack(position, scale);
    }
}
