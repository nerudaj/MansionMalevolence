#pragma once

#include "game/animations/AnimationInterface.hpp"

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
        std::function<void(const Position&, const Scale&)>) const override;

    std::optional<GameEvent> finalize() const override
    {
        return CardTakenGameEvent(inventorySlotIdx);
    }

private:
    size_t inventorySlotIdx = 0;
};
