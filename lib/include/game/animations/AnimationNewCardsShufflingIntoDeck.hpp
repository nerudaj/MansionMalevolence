#pragma once

#include "game/animations/AnimationInterface.hpp"

class [[nodiscard]] AnimationNewCardsShufflingIntoDeck final
    : public AnimationInterface
{
public:
    AnimationNewCardsShufflingIntoDeck() noexcept
        : AnimationInterface(sf::seconds(0.3f))
    {
    }

public:
    void render(
        const Scene& scene,
        const sf::Vector2f& baseOffset,
        std::function<void(const Card&, const Position&, const Scale&)>
            renderCard,
        std::function<void(const Position&, const Scale&)> renderCardBack)
        const override;

    std::optional<GameEvent> finalize() const override
    {
        return NewCardShuffledToDiscard();
    }
};
