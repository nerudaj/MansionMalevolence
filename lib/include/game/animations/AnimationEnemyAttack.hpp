#pragma once

#include "game/animations/AnimationInterface.hpp"

class [[nodiscard]] AnimationEnemyAttack final : public AnimationInterface
{
public:
    AnimationEnemyAttack(bool skipCardAfterReaction) noexcept
        : skipCardAfterReaction(skipCardAfterReaction)
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
        return MonsterReactionFinishedGameEvent(skipCardAfterReaction);
    }

private:
    bool skipCardAfterReaction = false;
};
