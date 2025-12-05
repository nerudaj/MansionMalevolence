#pragma once

#include "game/animations/AnimationInterface.hpp"
#include "game/definitions/Card.hpp"

class [[nodiscard]] AnimationCardToDiscard final : public AnimationInterface
{
public:
    AnimationCardToDiscard(
        Card card,
        const sf::Vector2f& origin,
        bool emitGameEvent = false) noexcept
        : card(card), origin(origin), emitGameEvent(emitGameEvent)
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
        if (emitGameEvent) return CardSkipEndedGameEvent();
        return std::nullopt;
    }

private:
    const Card card;
    const sf::Vector2f origin;
    const bool emitGameEvent;
};
