#pragma once

#include "game/animations/AnimationInterface.hpp"

class [[nodiscard]] AnimationEnemyDodgedAttack final : public AnimationInterface
{
public:
    AnimationEnemyDodgedAttack() : AnimationInterface(sf::seconds(0.3f)) {}

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
