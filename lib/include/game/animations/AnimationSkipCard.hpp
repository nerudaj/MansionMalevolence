#pragma once

#include "game/animations/AnimationInterface.hpp"
#include "game/definitions/EasingFunctions.hpp"
#include "game/definitions/Scene.hpp"

class [[nodiscard]] AnimationSkipCard final : public AnimationInterface
{
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
        // return CardSkipEndedGameEvent();
    }
};
