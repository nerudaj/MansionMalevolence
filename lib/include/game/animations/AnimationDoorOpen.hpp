#pragma once

#include "game/animations/AnimationInterface.hpp"

class [[nodiscard]] AnimationDoorOpen final : public AnimationInterface
{
public:
    AnimationDoorOpen(int link) noexcept : link(link) {}

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
        return DoorOpenedGameEvent(link);
    }

private:
    int link;
};
