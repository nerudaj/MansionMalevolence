#pragma once

#include "game/animations/AnimationInterface.hpp"
#include "game/definitions/Card.hpp"

class [[nodiscard]] AnimationCardToDiscard final : public AnimationInterface
{
public:
    AnimationCardToDiscard(
        const Card& card, const sf::Vector2f& origin) noexcept
        : AnimationInterface(sf::seconds(0.75f)), card(card), origin(origin)
    {
    }

public:
    void render(
        const Scene& scene,
        const sf::Vector2f& baseOffset,
        std::function<void(const Card&, const Position&, const Scale&)>
            renderCard,
        std::function<void(const Position&, const Scale&)>) const override;

    std::optional<GameEvent> finalize() const override
    {
        return CardSkipEndedGameEvent(card);
    }

private:
    const Card card;
    const sf::Vector2f origin;
};
