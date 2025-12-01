#pragma once

#include "game/animations/AnimationInterface.hpp"
#include "game/definitions/EasingFunctions.hpp"
#include "game/definitions/Scene.hpp"

class [[nodiscard]] AnimationNewCardsShufflingIntoDeck final
    : public AnimationInterface
{
public:
    AnimationNewCardsShufflingIntoDeck(int count) noexcept
        : AnimationInterface(sf::seconds(0.3f)), count(count)
    {
    }

public:
    void render(
        const Scene& scene,
        const sf::Vector2f& baseOffset,
        std::function<void(const Card&, const Position&, const Scale&)>
            renderCard,
        std::function<void(const Position&, const Scale&)> renderCardBack)
        const override
    {
        auto renderSecondTopDeckCard = [&]
        {
            if (count == 1 && scene.deck.size() > 1)
                renderCard(
                    *(++scene.deck.begin()),
                    Position(baseOffset),
                    Scale({ 1.f, 1.f }));
            else if (count > 1)
                renderCardBack(Position(baseOffset), Scale({ 1.f, 1.f }));
        };

        const auto animationOffset =
            (sf::Vector2f { baseOffset.x + 76.f * 1.5f, baseOffset.y }
             - baseOffset)
            * Easing::easeOutThenBack(getPercFactor());
        const auto scaleFactor = std::lerp(1.f, 0.8f, getPercFactor());

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

    std::optional<GameEvent> finalize() const override
    {
        if (count > 1)
            return ShuffleNewCardsIntoDeck(count - 1);
        else
            return std::nullopt;
    }

private:
    int count;
};
