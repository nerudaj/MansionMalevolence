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

using GameEvent = std::variant<CardTakenGameEvent, CardSkippedGameEvent>;
