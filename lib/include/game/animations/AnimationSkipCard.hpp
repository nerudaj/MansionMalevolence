#pragma once

#include "game/animations/AnimationInterface.hpp"
#include "game/definitions/EasingFunctions.hpp"
#include "game/definitions/Scene.hpp"

class [[nodiscard]] AnimationSkipCard final : public AnimationInterface
{
public:
    void render(
        const Scene& scene,
        const sf::Vector2f& baseOffset,
        std::function<void(const Card&, const Position&, const Scale&)>
            renderCard,
        std::function<void(const Position&, const Scale&)>) const override
    {
        const auto scale = Scale({ 1.f, 1.f });

        auto renderSecondTopDeckCard = [&]
        {
            if (scene.deck.size() > 1)
                renderCard(
                    *(++scene.deck.begin()), Position(baseOffset), scale);
        };

        const auto animationOffset =
            (sf::Vector2f { baseOffset.x + 76.f * 1.5f, baseOffset.y }
             - baseOffset)
            * Easing::easeOutThenBack(getPercFactor());

        if (getPercFactor() < 0.5f)
        {
            renderSecondTopDeckCard();
            renderCard(
                scene.deck.front(),
                Position(baseOffset + animationOffset),
                scale);
        }
        else
        {
            renderCard(
                scene.deck.front(),
                Position(baseOffset + animationOffset),
                scale);
            renderSecondTopDeckCard();
        }
    };

    std::optional<GameEvent> finalize() const override
    {
        return CardSkipEndedGameEvent();
    }
};
