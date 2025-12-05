#include "game/animations/AnimationDiscardToDeck.hpp"
#include "game/definitions/EasingFunctions.hpp"
#include "game/definitions/Scene.hpp"

void AnimationDiscardToDeck::render(
    const Scene& scene,
    const sf::Vector2f& baseOffset,
    std::function<void(const Card&, const Position&, const Scale&)> renderCard,
    std::function<void(const Position&, const Scale&)> renderCardBack) const
{
    const auto easedF = Easing::easeValley2(getPercFactor());

    const auto flipOffset =
        sf::Vector2f { scene.mainCardBody.getSize().x * 0.5f, -3.f }
        * (1.f - easedF);
    const auto travelOffset = (scene.trashBody.getPosition() - baseOffset)
                              * (1.f - Easing::easeInOut(getPercFactor()));

    const auto position = Position(baseOffset + travelOffset + flipOffset);
    const auto scale = Scale(
        sf::Vector2f { easedF, 1.f }
        * std::lerp(1.f / 3.f, 1.f, getPercFactor()));

    if (getPercFactor() <= 0.25f)
        renderCardBack(position, scale);
    else
        renderCard(scene.cardsToAdd.front(), position, scale);
};
