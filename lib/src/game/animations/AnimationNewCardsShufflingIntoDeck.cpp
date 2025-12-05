#include "game/animations/AnimationNewCardsShufflingIntoDeck.hpp"
#include "game/definitions/EasingFunctions.hpp"
#include "game/definitions/Scene.hpp"

void AnimationNewCardsShufflingIntoDeck::render(
    const Scene& scene,
    const sf::Vector2f& baseOffset,
    std::function<void(const Card&, const Position&, const Scale&)> renderCard,
    std::function<void(const Position&, const Scale&)> renderCardBack) const
{
    const auto count = scene.cardsToAdd.size();

    auto renderSecondTopDeckCard = [&]
    {
        if (count == 1 && scene.deck.size() > 1)
            renderCard(
                scene.deck.front(), Position(baseOffset), Scale({ 1.f, 1.f }));
        else if (count > 1)
            renderCardBack(Position(baseOffset), Scale({ 1.f, 1.f }));
    };

    const auto animationOffset = (scene.trashBody.getPosition() - baseOffset)
                                 * Easing::easeInOut(getPercFactor());
    const auto scaleFactor = std::lerp(1.f, 1.f / 3.f, getPercFactor());

    if (getPercFactor() < 0.5f)
    {
        renderSecondTopDeckCard();
        renderCardBack(
            Position(baseOffset + animationOffset),
            Scale({ scaleFactor, scaleFactor }));
    }
    else
    {
        renderCardBack(
            Position(baseOffset + animationOffset),
            Scale({ scaleFactor, scaleFactor }));
        renderSecondTopDeckCard();
    }
};
