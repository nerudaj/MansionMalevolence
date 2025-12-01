#pragma once

#include "game/animations/AnimationInterface.hpp"
#include "game/enums/CardType.hpp"

class [[nodiscard]] AnimationCardTransform final : public AnimationInterface
{
public:
    AnimationCardTransform(CardType cardType) noexcept : cardType(cardType) {}

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
    CardType cardType;
};
