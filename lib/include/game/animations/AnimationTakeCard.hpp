#pragma once

#include "game/animations/AnimationInterface.hpp"
#include "game/definitions/EasingFunctions.hpp"
#include "game/engine/RenderingEngine.hpp"

class [[nodiscard]] AnimationTakeCard final : public AnimationInterface
{
public:
    AnimationTakeCard(size_t inventorySlotIdx) noexcept
        : inventorySlotIdx(inventorySlotIdx)
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
        const auto scale = std::lerp(1.f, 1.f / 3.f, getPercFactor());
        const auto animationOffset =
            (RenderingEngine::getNthInventoryCardOffset(inventorySlotIdx)
             - baseOffset)
            * Easing::easeInOut(getPercFactor());
        renderCard(
            scene.deck.front(),
            Position(baseOffset + animationOffset),
            Scale({ scale, scale }));
    };

    std::optional<GameEvent> finalize() const override
    {
        return CardTakenGameEvent(inventorySlotIdx);
    }

private:
    size_t inventorySlotIdx = 0;
};
