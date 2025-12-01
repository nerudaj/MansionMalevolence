#pragma once

#include "game/animations/AnimationInterface.hpp"

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
        const override;

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
