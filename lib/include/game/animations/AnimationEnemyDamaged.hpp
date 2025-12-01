#pragma once

#include "game/animations/AnimationInterface.hpp"
#include "game/definitions/EasingFunctions.hpp"
#include "game/engine/RenderingEngine.hpp"

class [[nodiscard]] AnimationEnemyDamaged final : public AnimationInterface
{
public:
    AnimationEnemyDamaged(int damage) noexcept
        : AnimationInterface(sf::seconds(0.25f)), damage(damage)
    {
    }

public:
    void render(
        const Scene& scene,
        const sf::Vector2f& baseOffset,
        std::function<void(const Card&, const Position&, const Scale&)>
            renderCard,
        std::function<void(const Position&, const Scale&)>) const override
    {
        const auto animationOffset =
            (sf::Vector2f { baseOffset.x + 76.f * 0.1f,
                            baseOffset.y + 76.f * 0.1f }
             - baseOffset)
            * Easing::easeDamage(getPercFactor());
        renderCard(
            scene.deck.front(),
            Position(baseOffset + animationOffset),
            Scale({ 1.f, 1.f }));
    };

    std::optional<GameEvent> finalize() const override
    {
        return MonsterStaggerEndedGameEvent(damage);
    }

private:
    int damage = 0;
};
