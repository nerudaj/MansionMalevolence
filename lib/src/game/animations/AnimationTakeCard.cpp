#include "game/animations/AnimationTakeCard.hpp"
#include "game/definitions/EasingFunctions.hpp"
#include "game/engine/RenderingEngine.hpp"

void AnimationTakeCard::render(
    const Scene& scene,
    const sf::Vector2f& baseOffset,
    std::function<void(const Card&, const Position&, const Scale&)> renderCard,
    std::function<void(const Position&, const Scale&)>) const
{
    const auto scale = std::lerp(1.f, 1.f / 3.f, getPercFactor());
    const auto animationOffset =
        (RenderingEngine::getNthInventoryCardOffset(inventorySlotIdx)
         - baseOffset)
        * Easing::easeInOut(getPercFactor());

    if (!scene.deck.empty())
        renderCard(
            scene.deck.front(),
            Position(baseOffset + animationOffset),
            Scale({ scale, scale }));
};
