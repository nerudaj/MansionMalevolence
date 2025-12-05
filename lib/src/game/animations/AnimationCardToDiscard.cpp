#include "game/animations/AnimationCardToDiscard.hpp"
#include "game/definitions/EasingFunctions.hpp"
#include "game/engine/RenderingEngine.hpp"

void AnimationCardToDiscard::render(
    const Scene& scene,
    const sf::Vector2f& baseOffset,
    std::function<void(const Card&, const Position&, const Scale&)> renderCard,
    std::function<void(const Position&, const Scale&)> renderCardBack) const
{
    const auto easedF = Easing::easeValley2(getPercFactor());

    const auto flipOffset = sf::Vector2f { 76.f * 0.5f, -3.f } * (1.f - easedF);
    const auto travelOffset = (scene.trashBody.getPosition() - origin)
                              * Easing::easeInOut(getPercFactor());

    const auto position = Position(origin + travelOffset + flipOffset);
    const auto scale = Scale(
        sf::Vector2f { easedF, 1.f }
        * std::lerp(1.f, 1.f / 3.f, getPercFactor()));

    if (!scene.deck.empty())
        renderCard(
            scene.deck.front(), Position(baseOffset), Scale({ 1.f, 1.f }));

    if (getPercFactor() <= 0.25f)
        renderCard(card, position, scale);
    else
        renderCardBack(position, scale);
}
