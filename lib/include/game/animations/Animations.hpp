#pragma once

#include "game/definitions/Card.hpp"
#include <DGM/classes/Animation.hpp>
#include <DGM/classes/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <variant>

struct [[nodiscard]] AnimationBase
{
    sf::Time elapsed = sf::Time::Zero;
    sf::Time duration = sf::Time::Zero;
    float perc = 0.f;

    explicit AnimationBase(sf::Time duration = sf::seconds(0.5f))
        : duration(duration)
    {
    }
};

static inline dgm::Animation::PlaybackStatus
updateAnimation(AnimationBase& animation, const dgm::Time& time)
{
    animation.elapsed += time.getElapsed();
    animation.perc = animation.elapsed / animation.duration;
    return animation.elapsed > animation.duration
               ? dgm::Animation::PlaybackStatus::Finished
               : dgm::Animation::PlaybackStatus::Playing;
}

struct [[nodiscard]] AnimationCardToDiscard final : AnimationBase
{
    Card card;
    sf::Vector2f origin;

    AnimationCardToDiscard(const Card& card, const sf::Vector2f& origin)
        : AnimationBase(sf::seconds(0.75f)), card(card), origin(origin)
    {
    }
};

struct [[nodiscard]] AnimationCardTransform final : AnimationBase
{
    CardType cardType;

    explicit AnimationCardTransform(CardType cardType) : cardType(cardType) {}
};

struct [[nodiscard]] AnimationDiscardToDeck final : AnimationBase
{
};

struct [[nodiscard]] AnimationDoorOpen final : AnimationBase
{
    int link;

    explicit AnimationDoorOpen(int link) : link(link) {}
};

struct [[nodiscard]] AnimationEnemyAttack final : AnimationBase
{
    bool skipCardAfterReaction;

    explicit AnimationEnemyAttack(bool skipCardAfterReaction)
        : skipCardAfterReaction(skipCardAfterReaction)
    {
    }
};

struct [[nodiscard]] AnimationEnemyDamaged final : AnimationBase
{
    int damage = 0;
    size_t usedWeaponInventoryIdx = false;

    AnimationEnemyDamaged(
        int damage, size_t usedWeaponInventoryIdx, sf::Time duration)
        : AnimationBase(sf::seconds(std::max(duration.asSeconds(), 0.25f)))
        , damage(damage)
        , usedWeaponInventoryIdx(usedWeaponInventoryIdx)
    {
    }
};

struct [[nodiscard]] AnimationEnemyDodgedAttack final : AnimationBase
{
    explicit AnimationEnemyDodgedAttack(sf::Time duration)
        : AnimationBase(sf::seconds(std::max(duration.asSeconds(), 0.3f)))
    {
    }
};

struct [[nodiscard]] AnimationInvalidOperation final : AnimationBase
{
    AnimationInvalidOperation() : AnimationBase(sf::seconds(0.25f)) {}
};

struct [[nodiscard]] AnimationNewCardsShufflingIntoDeck final : AnimationBase
{
    AnimationNewCardsShufflingIntoDeck() : AnimationBase(sf::seconds(0.3f)) {}
};

struct [[nodiscard]] AnimationReturnInventoryToDeck final : AnimationBase
{
    Card card;

    explicit AnimationReturnInventoryToDeck(Card card) : card(card) {}
};

struct [[nodiscard]] AnimationTakeCard final : AnimationBase
{
    size_t inventorySlotIdx;

    explicit AnimationTakeCard(size_t inventorySlotIdx)
        : inventorySlotIdx(inventorySlotIdx)
    {
    }
};

struct [[nodiscard]] AnimationTrashMainCard final : AnimationBase
{
};

struct [[nodiscard]] AnimationHeal final : AnimationBase
{
    AnimationHeal() : AnimationBase(sf::seconds(1.4f)) {}
};

using Animation = std::variant<
    AnimationCardToDiscard,
    AnimationCardTransform,
    AnimationDiscardToDeck,
    AnimationDoorOpen,
    AnimationEnemyAttack,
    AnimationEnemyDamaged,
    AnimationEnemyDodgedAttack,
    AnimationInvalidOperation,
    AnimationNewCardsShufflingIntoDeck,
    AnimationReturnInventoryToDeck,
    AnimationTakeCard,
    AnimationTrashMainCard,
    AnimationHeal>;
