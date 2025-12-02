#pragma once

#include "game/animations/AnimationInterface.hpp"
#include "game/definitions/Card.hpp"

class [[nodiscard]] AnimationReturnInventoryToDeck final
    : public AnimationInterface
{
public:
    AnimationReturnInventoryToDeck(Card card) noexcept : card(card) {}

public:
    void render(
        const Scene& scene,
        const sf::Vector2f& baseOffset,
        std::function<void(const Card&, const Position&, const Scale&)>
            renderCard,
        std::function<void(const Position&, const Scale&)>) const override;

    std::optional<GameEvent> finalize() const override
    {
        return std::nullopt;
    }

private:
    Card card;
};
