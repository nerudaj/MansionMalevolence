#pragma once

#include "game/animations/AnimationInterface.hpp"

class [[nodiscard]] AnimationInvalidOperation final : public AnimationInterface
{
public:
    AnimationInvalidOperation() noexcept
        : AnimationInterface(sf::seconds(0.25f))
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
        return std::nullopt;
    }
};
