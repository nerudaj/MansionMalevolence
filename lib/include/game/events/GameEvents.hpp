#pragma once

#include <variant>

struct [[nodiscard]] CardTakenGameEvent final
{
    size_t inventorySlotIdx;

    CardTakenGameEvent(size_t idx) : inventorySlotIdx(idx) {}
};

struct [[nodiscard]] CardSkippedGameEvent final
{
    CardSkippedGameEvent() = default;
};

struct [[nodiscard]] InventoryCardTrashedGameEvent final
{
    size_t inventorySlotIdx;

    InventoryCardTrashedGameEvent(size_t idx) : inventorySlotIdx(idx) {}
};

using GameEvent = std::variant<
    CardTakenGameEvent,
    CardSkippedGameEvent,
    InventoryCardTrashedGameEvent>;
