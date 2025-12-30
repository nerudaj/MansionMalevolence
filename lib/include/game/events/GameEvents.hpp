#pragma once

#include "game/definitions/Card.hpp"
#include <variant>

struct [[nodiscard]] CardTakenGameEvent final
{
    size_t inventorySlotIdx;

    constexpr explicit CardTakenGameEvent(size_t idx) noexcept
        : inventorySlotIdx(idx)
    {
    }
};

struct [[nodiscard]] BeforeCardSkipGameEvent final
{
};

struct [[nodiscard]] CardSkipStartedGameEvent final
{
};

struct [[nodiscard]] CardSkipEndedGameEvent final
{
    Card card;

    explicit CardSkipEndedGameEvent(Card card) : card(card) {}
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

    explicit InventoryCardUsedForHealingGameEvent(size_t idx)
        : inventorySlotIdx(idx)
    {
    }
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

struct [[nodiscard]] MonsterShotAtGameEvent final
{
    size_t inventoryWeaponIdx;

    constexpr explicit MonsterShotAtGameEvent(
        size_t inventoryWeaponIdx) noexcept
        : inventoryWeaponIdx(inventoryWeaponIdx)
    {
    }
};

struct [[nodiscard]] MonsterStaggerEndedGameEvent final
{
    int damage;
    size_t usedWeaponInventoryIdx;

    constexpr explicit MonsterStaggerEndedGameEvent(
        int damage, size_t usedWeaponInventoryIdx) noexcept
        : damage(damage), usedWeaponInventoryIdx(usedWeaponInventoryIdx)
    {
    }
};

struct [[nodiscard]] MainCardTrashedGameEvent final
{
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

struct [[nodiscard]] ZombieDiedGameEvent final
{
};

struct [[nodiscard]] MainCardResolvedGameEvent final
{
};

struct [[nodiscard]] DoorOpenedGameEvent final
{
    int link;

    explicit DoorOpenedGameEvent(int link) noexcept : link(link) {}
};

struct [[nodiscard]] NewCardShuffledToDiscard final
{
};

struct [[nodiscard]] DiscardReturnedToDeckGameEvent
{
};

using GameEvent = std::variant<
    CardTakenGameEvent,
    BeforeCardSkipGameEvent,
    CardSkipStartedGameEvent,
    CardSkipEndedGameEvent,
    InventoryCardTrashedGameEvent,
    InventoryCardUsedForHealingGameEvent,
    InventoryCardUsedOnMainCardGameEvent,
    MonsterReactionTriggeredGameEvent,
    MonsterReactionFinishedGameEvent,
    MonsterShotAtGameEvent,
    MonsterStaggerEndedGameEvent,
    MainCardTrashedGameEvent,
    CardUsedOnAnotherInventoryCardGameEvent,
    ZombieDiedGameEvent,
    MainCardResolvedGameEvent,
    DoorOpenedGameEvent,
    NewCardShuffledToDiscard,
    DiscardReturnedToDeckGameEvent>;
