#pragma once

#include "game/definitions/Card.hpp"
#include "game/enums/Icon.hpp"
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

struct [[nodiscard]] AnimationMainCardToDiscard final : AnimationBase
{
    sf::Vector2f origin;

    explicit AnimationMainCardToDiscard(const sf::Vector2f& origin)
        : AnimationBase(sf::seconds(0.75f)), origin(origin)
    {
    }
};

struct [[nodiscard]] AnimationInventoryCardToDiscard final : AnimationBase
{
    sf::Vector2f origin;
    Card card;

    AnimationInventoryCardToDiscard(
        const sf::Vector2f& origin, const Card& card)
        : origin(origin), card(card)
    {
    }
};

struct [[nodiscard]] AnimationCardTransform final : AnimationBase
{
    CardType cardType;

    explicit AnimationCardTransform(CardType cardType) : cardType(cardType) {}
};

struct [[nodiscard]] AnimationReturnDiscardToDeck final : AnimationBase
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

struct [[nodiscard]] AnimationEnemyDamagedWindup final : AnimationBase
{
    int damage = 0;
    size_t usedWeaponInventoryIdx = 0;

    AnimationEnemyDamagedWindup(
        int damage, size_t usedWeaponInventoryIdx, sf::Time duration)
        : AnimationBase(duration)
        , damage(damage)
        , usedWeaponInventoryIdx(usedWeaponInventoryIdx)
    {
    }
};

struct [[nodiscard]] AnimationEnemyDamaged final : AnimationBase
{
    int damage = 0;
    size_t usedWeaponInventoryIdx = 0;
    Icon impactIconId;

    AnimationEnemyDamaged(
        int damage,
        size_t usedWeaponInventoryIdx,
        Icon impactIconId,
        sf::Time duration)
        : AnimationBase(sf::seconds(std::max(duration.asSeconds(), 0.25f)))
        , damage(damage)
        , usedWeaponInventoryIdx(usedWeaponInventoryIdx)
        , impactIconId(impactIconId)
    {
    }
};

struct [[nodiscard]] AnimationEnemyDodgedAttack final : AnimationBase
{
    explicit AnimationEnemyDodgedAttack(sf::Time duration)
        : AnimationBase(sf::seconds(std::max(duration.asSeconds(), 0.4f)))
    {
    }
};

struct [[nodiscard]] AnimationInvalidOperation final : AnimationBase
{
    AnimationInvalidOperation() : AnimationBase(sf::seconds(0.25f)) {}
};

struct [[nodiscard]] AnimationOutOfAmmo final : AnimationBase
{
    AnimationOutOfAmmo() : AnimationBase(sf::seconds(0.4f)) {}
};

struct [[nodiscard]] AnimationNewCardsShufflingIntoDeck final : AnimationBase
{
    AnimationNewCardsShufflingIntoDeck() : AnimationBase(sf::seconds(0.3f)) {}
};

struct [[nodiscard]] AnimationTakeCard final : AnimationBase
{
    size_t inventorySlotIdx;
    sf::Vector2f origin;

    AnimationTakeCard(size_t inventorySlotIdx, const sf::Vector2f& origin)
        : inventorySlotIdx(inventorySlotIdx), origin(origin)
    {
    }
};

struct [[nodiscard]] AnimationTrashMainCard final : AnimationBase
{
};

struct [[nodiscard]] AnimationHeal final : AnimationBase
{
    int healAmount = 0;
    bool isVaccineHeal = false;
    int initialInfection = 0;

    explicit AnimationHeal(
        int healAmount, bool isVaccineHeal = false, int initialInfection = 0)
        : AnimationBase(sf::seconds(1.4f))
        , healAmount(healAmount)
        , isVaccineHeal(isVaccineHeal)
        , initialInfection(initialInfection)
    {
    }
};

struct [[nodiscard]] AnimationReturnDraggedMainCard final : AnimationBase
{
    sf::Vector2f origin;

    explicit AnimationReturnDraggedMainCard(const sf::Vector2f& origin)
        : origin(origin)
    {
    }
};

struct [[nodiscard]] AnimationDrawCard final : public AnimationBase
{
};

using Animation = std::variant<
    AnimationMainCardToDiscard,
    AnimationInventoryCardToDiscard,
    AnimationCardTransform,
    AnimationReturnDiscardToDeck,
    AnimationDoorOpen,
    AnimationEnemyAttack,
    AnimationEnemyDamagedWindup,
    AnimationEnemyDamaged,
    AnimationEnemyDodgedAttack,
    AnimationInvalidOperation,
    AnimationNewCardsShufflingIntoDeck,
    AnimationTakeCard,
    AnimationTrashMainCard,
    AnimationHeal,
    AnimationReturnDraggedMainCard,
    AnimationDrawCard,
    AnimationOutOfAmmo>;
