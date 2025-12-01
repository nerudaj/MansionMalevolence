#include "game/animations/AnimationDoorOpen.hpp"
#include "game/builders/CardBuilder.hpp"
#include "game/definitions/EasingFunctions.hpp"
#include "game/definitions/Scene.hpp"

void AnimationDoorOpen::render(
    const Scene& scene,
    const sf::Vector2f& baseOffset,
    std::function<void(const Card&, const Position&, const Scale&)> renderCard,
    std::function<void(const Position&, const Scale&)> renderCardBack) const
{
    const auto easedF = Easing::easeValley(getPercFactor());

    const auto animationOffset =
        (sf::Vector2f { baseOffset.x + 76.f * 0.5f, baseOffset.y - 3.f }
         - baseOffset)
        * (1.f - easedF);

    if (getPercFactor() <= 0.5f)
    {
        renderCard(
            scene.deck.front(),
            Position(baseOffset + animationOffset),
            Scale({ easedF, 1.f }));
    }
    else
    {
        renderCardBack(
            Position(baseOffset + animationOffset), Scale({ easedF, 1.f }));
    }
};
