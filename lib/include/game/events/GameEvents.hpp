#pragma once

#include <variant>

struct [[nodiscard]] CardTakenGameEvent final
{
    size_t inventorySlotIdx;

    constexpr explicit CardTakenGameEvent(size_t idx) noexcept
        : inventorySlotIdx(idx)
    {
    }
};

struct [[nodiscard]] CardSkippedGameEvent final
{
    CardSkippedGameEvent() = default;
};

struct [[nodiscard]] InventoryCardTrashedGameEvent final
{
    size_t inventorySlotIdx;

    constexpr explicit InventoryCardTrashedGameEvent(size_t idx) noexcept
        : inventorySlotIdx(idx)
    {
    }
};

struct [[nodiscard]] InventoryCardUsedForHealingGameEvent final
{
    size_t inventorySlotIdx;

    InventoryCardUsedForHealingGameEvent(size_t idx) : inventorySlotIdx(idx) {}
};

struct [[nodiscard]] InventoryCardUsedOnMainCardGameEvent final
{
    size_t inventorySlotIdx;

    constexpr explicit InventoryCardUsedOnMainCardGameEvent(size_t idx) noexcept
        : inventorySlotIdx(idx)
    {
    }
};

struct [[nodiscard]] MonsterReactionTriggeredGameEvent final
{
    bool skipCardAfterReaction;

    constexpr explicit MonsterReactionTriggeredGameEvent(
        bool skipCardAfterReaction) noexcept
        : skipCardAfterReaction(skipCardAfterReaction) {};
};

struct [[nodiscard]] MainCardTrashedGameEvent final
{
    MainCardTrashedGameEvent() = default;
};

using GameEvent = std::variant<
    CardTakenGameEvent,
    CardSkippedGameEvent,
    InventoryCardTrashedGameEvent,
    InventoryCardUsedForHealingGameEvent,
    InventoryCardUsedOnMainCardGameEvent,
    MonsterReactionTriggeredGameEvent,
    MainCardTrashedGameEvent>;
