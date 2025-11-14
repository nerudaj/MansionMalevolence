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

struct [[nodiscard]] MonsterReactionFinishedGameEvent final
{
    bool skipCardAfterReaction;

    constexpr explicit MonsterReactionFinishedGameEvent(
        bool skipCardAfterReaction) noexcept
        : skipCardAfterReaction(skipCardAfterReaction) {};
};

struct [[nodiscard]] MainCardTrashedGameEvent final
{
    MainCardTrashedGameEvent() = default;
};

struct [[nodiscard]] CardUsedOnAnotherInventoryCardGameEvent final
{
    size_t sourceCardInventoryIdx;
    size_t destinationCardInventoryIdx;

    constexpr CardUsedOnAnotherInventoryCardGameEvent(
        size_t sourceCardInventoryIdx,
        size_t destinationCardInventoryIdx) noexcept
        : sourceCardInventoryIdx(sourceCardInventoryIdx)
        , destinationCardInventoryIdx(destinationCardInventoryIdx)
    {
    }
};

using GameEvent = std::variant<
    CardTakenGameEvent,
    CardSkippedGameEvent,
    InventoryCardTrashedGameEvent,
    InventoryCardUsedForHealingGameEvent,
    InventoryCardUsedOnMainCardGameEvent,
    MonsterReactionTriggeredGameEvent,
    MonsterReactionFinishedGameEvent,
    MainCardTrashedGameEvent,
    CardUsedOnAnotherInventoryCardGameEvent>;
