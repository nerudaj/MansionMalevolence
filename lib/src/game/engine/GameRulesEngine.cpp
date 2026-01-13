#include "game/engine/GameRulesEngine.hpp"
#include "game/animations/Animations.hpp"
#include "game/builders/SceneBuilder.hpp"
#include "misc/CoordConverter.hpp"
#include <algorithm>
#include <limits>
#include <random>

void GameRulesEngine::operator()(const CardTakenGameEvent& e)
{
    scene.stats.turnsTaken++;

    if (scene.inventory[e.inventorySlotIdx].has_value())
    {
        auto& inventoryCard = scene.inventory[e.inventorySlotIdx].value();
        combineCards(inventoryCard, scene.deck.front());
    }
    else
    {
        scene.inventory[e.inventorySlotIdx] = scene.deck.front();
    }

    popTopDeckCard();
}

void GameRulesEngine::operator()(const CardSkipStartedGameEvent&)
{
    scene.stats.turnsTaken++;
    scene.activeAnimation = AnimationCardToDiscard(
        scene.deck.front(), scene.mainCardBody.getPosition());
    popTopDeckCard();
    audioEngine.playSound(SoundId::CardShuffle);
}

void GameRulesEngine::operator()(const CardSkipEndedGameEvent& e)
{
    scene.discard.push_back(e.card);
}

void GameRulesEngine::operator()(const BeforeCardSkipGameEvent&)
{
    if (scene.deck.empty()) return;

    const bool hasSilencedPistol = [this]
    {
        for (auto card : scene.inventory)
            if (card.has_value() && card->special & CardSpecial::BoostsEvasion)
                return true;
        return false;
    }();

    const bool isEnemy = scene.deck.front().traits & CardTrait::Enemy;
    const auto special = scene.deck.front().special;
    // clang-format off
    const bool managedToEvade = hasSilencedPistol
        ? scene.chance.rollForSilencedDodge()
        : special & CardSpecial::Blind
            ? scene.chance.rollForBlindDodge()
            : special & CardSpecial::Vigilant
                ? scene.chance.rollForVigilantDodge()
                : scene.chance.rollForDodge();
    // clang-format on

    if (isEnemy && !managedToEvade)
    {
        gameEventQueue.pushEvent<MonsterReactionTriggeredGameEvent>(
            "skipCardAfterReaction"_true);
    }
    else
    {
        gameEventQueue.pushEvent<CardSkipStartedGameEvent>();
    }
}

void GameRulesEngine::operator()(const InventoryCardTrashedGameEvent& e)
{
    auto card = *scene.inventory[e.inventorySlotIdx];
    scene.inventory[e.inventorySlotIdx].reset();

    scene.activeAnimation = AnimationCardToDiscard(
        card, scene.inventoryBodies[e.inventorySlotIdx].getPosition());
}

void GameRulesEngine::operator()(const InventoryCardUsedForHealingGameEvent& e)
{
    auto& card = scene.inventory[e.inventorySlotIdx].value();
    if (!(card.traits & CardTrait::Healing))
    {
        audioEngine.playSound(SoundId::Error);
        return;
    }

    scene.hearts = std::clamp(scene.hearts + card.power, 0, MAX_HEARTS);
    audioEngine.playSound(SoundId::Heal);
    scene.activeAnimation = AnimationHeal(
        card.special & CardSpecial::WinGame, scene.infectionProgress);
    scene.inventory[e.inventorySlotIdx].reset();
}

void GameRulesEngine::operator()(const InventoryCardUsedOnMainCardGameEvent& e)
{
    auto& card = scene.inventory[e.inventorySlotIdx].value();
    auto& deckCard = scene.deck.front();
    if (card.traits & CardTrait::Weapon && deckCard.traits & CardTrait::Enemy)
    {
        if (card.quantity != 0)
        {
            gameEventQueue.pushEvent<MonsterShotAtGameEvent>(
                e.inventorySlotIdx);
        }
    }
    else if (
        card.traits & CardTrait::KeyItem
        && deckCard.traits & CardTrait::KeyTarget && card.link == deckCard.link)
    {
        scene.inventory[e.inventorySlotIdx].reset();

        if (card.link == SPECIAL_CREST_DOOR
            && deckCard.image == CardImage::CrestDoorEmpty)
        {
            transformTopCard(
                CardType::CrestDoorEmpty, CardType::CrestDoorWithOneCrest);
        }
        else if (card.link == SPECIAL_RED_JEWEL)
        {
            transformTopCard(CardType::RedJewelBox, CardType::RocketLauncher);
        }
        else if (card.link == SPECIAL_YELLOW_JEWEL)
        {
            transformTopCard(
                CardType::YellowJewelBox, CardType::MoonCrestRight);
        }
        else if (card.link == SPECIAL_LOCKER_KEY)
        {
            audioEngine.playSound(scene.deck.front().specialSound);
            transformTopCard(
                CardType::LockedWeaponLocker, CardType::UnlockedWeaponLocker);
        }
        else
        {
            audioEngine.playSound(scene.deck.front().specialSound);
            scene.activeAnimation = AnimationDoorOpen(card.link);
        }
    }
    else
    {
        audioEngine.playSound(SoundId::Error);
        scene.activeAnimation = AnimationInvalidOperation();
        return; // skip stats increment
    }

    scene.stats.turnsTaken++;
}

void GameRulesEngine::operator()(const MonsterReactionTriggeredGameEvent& e)
{
    scene.activeAnimation = AnimationEnemyAttack(e.skipCardAfterReaction);
    audioEngine.playSound(scene.deck.front().specialSound);
}

void GameRulesEngine::operator()(const MonsterReactionFinishedGameEvent& e)
{
    scene.hearts -= scene.deck.front().power;
    scene.stats.damageTaken += scene.deck.front().power;
    if (scene.hearts <= 0) return;

    if (e.skipCardAfterReaction)
    {
        gameEventQueue.pushEvent<CardSkipStartedGameEvent>();
    }
}

void GameRulesEngine::operator()(const MonsterShotAtGameEvent& e)
{
    auto& weapon = scene.inventory[e.inventoryWeaponIdx].value();

    const auto soundDuration =
        weapon.image == CardImage::RocketLauncher
            ? audioEngine.playSound(SoundId::LauncherFire)
            : sf::seconds(0.001f);
    --weapon.quantity;
    scene.stats.shotsFired++;

    const bool canEvade = scene.deck.front().special & CardSpecial::Evasive
                          && !(weapon.special & CardSpecial::NegatesEvasive);
    if (canEvade && scene.chance.rollForEvasion())
    {
        if (weapon.image != CardImage::RocketLauncher)
            std::ignore = audioEngine.playSound(weapon.specialSound);
        scene.activeAnimation = AnimationEnemyDodgedAttack(soundDuration);
    }
    else
    {
        const int extraDamage =
            weapon.special & CardSpecial::CritChance
                ? static_cast<int>(scene.chance.rollForCrit())
                : 0;
        scene.activeAnimation = AnimationEnemyDamagedWindup(
            weapon.power + extraDamage, e.inventoryWeaponIdx, soundDuration);
    }
}

void GameRulesEngine::operator()(const MonsterStaggerEndedGameEvent& e)
{
    auto& deckCard = scene.deck.front();
    deckCard.power -= e.damage;
    if (deckCard.power <= 0)
    {
        gameEventQueue.pushEvent<MainCardResolvedGameEvent>();
        scene.stats.enemiesKilled++;

        assert(scene.inventory[e.usedWeaponInventoryIdx].has_value());
        auto& weapon = scene.inventory[e.usedWeaponInventoryIdx].value();
        if (weapon.special & CardSpecial::RefillAmmoOnKill)
        {
            weapon.quantity++;
        }
    }
    else if (deckCard.special & CardSpecial::Retaliate)
    {
        gameEventQueue.pushEvent<MonsterReactionTriggeredGameEvent>(
            "skipCardAfterReaction"_false);
    }
}

void GameRulesEngine::operator()(const MainCardTrashedGameEvent&)
{
    popTopDeckCard();
}

void GameRulesEngine::operator()(
    const CardUsedOnAnotherInventoryCardGameEvent& e)
{
    if (!scene.inventory[e.sourceCardInventoryIdx].has_value()
        || !scene.inventory[e.destinationCardInventoryIdx].has_value())
    {
        throw std::runtime_error(
            "CardUsedOnAnotherInventoryCardGameEvent: One of the two inventory "
            "slots is empty");
    }

    const auto& src = scene.inventory[e.sourceCardInventoryIdx].value();
    auto& dst = scene.inventory[e.destinationCardInventoryIdx].value();

    combineCards(dst, src);

    scene.inventory[e.sourceCardInventoryIdx].reset();
}

void GameRulesEngine::operator()(const ZombieDiedGameEvent&)
{
    if (!scene.chance.rollForCrimsonHead()) return;

    scene.discard.push_back(CardBuilder::createCard(CardType::CrimsonHead));
}

void GameRulesEngine::operator()(const MainCardResolvedGameEvent&)
{
    if (scene.deck.front().special & CardSpecial::SpawnCrimsonHead)
    {
        gameEventQueue.pushEvent<ZombieDiedGameEvent>();
    }

    scene.activeAnimation = AnimationTrashMainCard();
    audioEngine.playSound(SoundId::CardDestroy);
}

void GameRulesEngine::operator()(const DoorOpenedGameEvent& e)
{
    scene.cardsToAdd = scene.builder->generateRoomDeck(e.link);
    popTopDeckCard();
    shuffleNewCardIntoDeck();
}

void GameRulesEngine::operator()(const NewCardShuffledToDiscard&)
{
    scene.discard.push_back(scene.cardsToAdd.front());
    scene.cardsToAdd.pop_front();
    shuffleNewCardIntoDeck();
}

void GameRulesEngine::operator()(const DiscardReturnedToDeckGameEvent&)
{
    scene.deck = scene.cardsToAdd;
    scene.cardsToAdd.clear();
}

void GameRulesEngine::operator()(const AttackWindupAnimationEndedGameEvent& e)
{
    assert(scene.inventory[e.usedWeaponInventoryIdx]);
    auto& weapon = scene.inventory[e.usedWeaponInventoryIdx].value();
    const auto soundDuration = audioEngine.playSound(weapon.specialSound);

    const auto icon =
        weapon.image == CardImage::Pistol           ? Icon::BulletHole
        : weapon.image == CardImage::SilencedPistol ? Icon::BulletHole
        : weapon.image == CardImage::Crossbow       ? Icon::Dart
        : weapon.image == CardImage::Shotgun        ? Icon::ThreeBulletHole
                                                    : Icon::Explosion1;

    scene.activeAnimation = AnimationEnemyDamaged(
        e.damage, e.usedWeaponInventoryIdx, icon, soundDuration);
}

void GameRulesEngine::update(const dgm::Time& time)
{
    scene.infectionProgress = scene.stats.turnsTaken;

    if (scene.activeAnimation)
    {
        updateActiveAnimation(time);
        return;
    }

    if (scene.deck.empty())
    {
        if (!scene.discard.empty())
        {
            scene.activeAnimation = AnimationDiscardToDeck();
            scene.cardsToAdd = scene.discard;
            scene.discard.clear();
            return;
        }
    }

    if (scene.deck.empty())
    {
        scene.usableInventorySlot = std::nullopt;
        scene.canSafelySkipCard = false;
        scene.canTakeCard = false;
    }
    else
    {
        scene.usableInventorySlot = getUsableInventorySlot(scene.deck.front());
        scene.canTakeCard =
            (scene.deck.front().traits & CardTrait::Pickable
             && scene.usableInventorySlot)
            || scene.deck.front().special & CardSpecial::BoosterPack;
        scene.canSafelySkipCard =
            !(scene.deck.front().traits & CardTrait::Enemy);
    }

    if (scene.boosterChoice)
    {
        const auto clickPos = screenToWorld(input.getDragPosition());
        for (auto&& [idx, body] :
             std::ranges::views::enumerate(scene.choiceBodies))
        {
            if (dgm::Collision::basic(body, clickPos))
            {
                transformTopCard(
                    CardType::Crate, scene.boosterChoice.value()[idx]);
                scene.boosterChoice.reset();
            }
        }
    }
    else
    {
        if (auto pos = input.getDragPosition(); pos != sf::Vector2f {})
        {
            handleDragStartedOrMoved(pos);
        }
        else if (scene.dragDrop.has_value())
        {
            handleDragEnded();
        }
    }
}

void GameRulesEngine::handleTake()
{
    if (scene.canTakeCard)
    {
        assert(!scene.deck.empty());
        if (scene.deck.front().special & CardSpecial::BoosterPack)
        {
            scene.boosterChoice =
                SceneBuilder::generateBooster(scene.deck.front().image);
            audioEngine.playSound(scene.deck.front().specialSound);
        }
        else
        {
            scene.activeAnimation =
                AnimationTakeCard(*scene.usableInventorySlot);
        }
    }
    else
    {
        audioEngine.playSound(SoundId::Error);
        scene.activeAnimation = AnimationInvalidOperation();
    }
}

void GameRulesEngine::handleSkip()
{
    gameEventQueue.pushEvent<BeforeCardSkipGameEvent>();
}

void GameRulesEngine::handleDragStartedOrMoved(sf::Vector2f& pos)
{
    const auto worldPos = screenToWorld(pos);
    if (!scene.dragDrop.has_value())
    { // started
        auto&& inventoryIdx = findCollidingInventoryIdx(worldPos);
        scene.dragDrop = DragDrop {
            .inventoryIdx = std::move(inventoryIdx),
            .draggingMainCard =
                dgm::Collision::basic(scene.mainCardBody, worldPos),
            .position = worldPos,
            .initialPosition = worldPos,
        };
    }
    else
    { // moved
        scene.dragDrop->position = worldPos;
    }
}

template<class Callable>
class ScopeGuard
{
public:
    explicit ScopeGuard(Callable&& callable)
        : callable(std::forward<Callable>(callable))
    {
    }

    ~ScopeGuard()
    {
        callable();
    }

private:
    Callable&& callable;
};

void GameRulesEngine::handleDragEnded()
{
    auto&& guard = ScopeGuard([this] { scene.dragDrop.reset(); });

    if (scene.dragDrop->draggingMainCard)
    {
        const auto dir =
            scene.dragDrop->position - scene.dragDrop->initialPosition;

        const bool swipedDown = dir.y > 0.f && dir.y > std::abs(dir.x);
        const bool swipedRight = dir.x > 0.f && dir.x > std::abs(dir.y);
        if (swipedDown)
            handleTake();
        else if (swipedRight)
            handleSkip();

        return;
    }

    if (!scene.dragDrop->inventoryIdx.has_value())
    {
        return;
    }

    auto inventoryIdx = *scene.dragDrop->inventoryIdx;
    const auto& dragPos = scene.dragDrop->position;
    if (dgm::Collision::basic(scene.mainCardBody, dragPos))
    {
        gameEventQueue.pushEvent<InventoryCardUsedOnMainCardGameEvent>(
            inventoryIdx);
    }
    else if (dgm::Collision::basic(scene.healthbarBody, dragPos))
    {
        gameEventQueue.pushEvent<InventoryCardUsedForHealingGameEvent>(
            inventoryIdx);
    }
    else if (dgm::Collision::basic(scene.trashBody, dragPos))
    {
        gameEventQueue.pushEvent<InventoryCardTrashedGameEvent>(inventoryIdx);
    }
    else
    {
        for (auto&& [idx, inventoryBody] :
             std::views::enumerate(scene.inventoryBodies))
        {
            if (!scene.inventory[idx].has_value()
                || static_cast<size_t>(idx) == inventoryIdx)
                continue;

            if (dgm::Collision::basic(inventoryBody, dragPos)
                && canInventoryCardCombineWithIncoming(
                    *scene.inventory[idx], *scene.inventory[inventoryIdx]))
            {
                gameEventQueue
                    .pushEvent<CardUsedOnAnotherInventoryCardGameEvent>(
                        inventoryIdx, static_cast<size_t>(idx));
            }
        }
    }
}

void GameRulesEngine::updateActiveAnimation(const dgm::Time& time)
{
    if (!scene.activeAnimation) return;

    const auto status = std::visit(
        overloads { [&time, this](AnimationHeal& a)
                    { return updateHealAnimation(a, time); },
                    [&time](auto& a) { return updateAnimation(a, time); } },
        *scene.activeAnimation);

    if (status == dgm::Animation::PlaybackStatus::Finished)
    {
        auto event = getEventAfterAnimationEnded(*scene.activeAnimation);
        if (event) gameEventQueue.pushEvent(std::move(*event));

        scene.activeAnimation.reset();
    }
}

std::optional<size_t>
GameRulesEngine::getUsableInventorySlot(const Card& card) const
{
    // Then empty slots
    for (auto&& [idx, slot] : std::ranges::views::enumerate(scene.inventory))
    {
        if (!slot) return idx;
    }

    // Check combinable cards
    for (auto&& [idx, slot] : std::ranges::views::enumerate(scene.inventory))
    {
        if (slot && canInventoryCardCombineWithIncoming(*slot, card))
            return idx;
    }

    return std::nullopt;
}

bool GameRulesEngine::canInventoryCardCombineWithIncoming(
    const Card& inventoryCard, const Card& incomingCard)
{
    return inventoryCard.traits & CardTrait::Weapon
               && incomingCard.traits & CardTrait::Ammo
               && inventoryCard.quantity < MAX_AMMO
           || inventoryCard.special & CardSpecial::Combines
                  && incomingCard.special & CardSpecial::Combines
                  && inventoryCard.link == incomingCard.link;
}

bool GameRulesEngine::canCardInteractWithDeck(
    const Card& a, const std::list<Card>& deck)
{
    const auto deckTraits = deck.front().traits;
    return deckTraits & CardTrait::Enemy && a.traits & CardTrait::Weapon
           || deckTraits & CardTrait::KeyTarget && a.traits & CardTrait::KeyItem
                  && deck.front().link == a.link;
}

bool GameRulesEngine::gameEnded() const noexcept
{
    return !scene.activeAnimation
           && (gameWon() || scene.hearts <= 0
               || scene.stats.turnsTaken >= scene.infectionLimit);
}

GameEndReason GameRulesEngine::getGameEnding() const noexcept
{
    if (gameWon())
        return GameEndReason::Won;
    else if (scene.stats.turnsTaken >= scene.infectionLimit)
        return GameEndReason::InfectionMax;
    else if (scene.deck.front().image == CardImage::Zombie)
        return GameEndReason::ZombieBite;
    else if (scene.deck.front().image == CardImage::Cerberus)
        return GameEndReason::CerberusBark;
    else if (scene.deck.front().image == CardImage::CrimsonHead)
        return GameEndReason::CrimsonHeadScreech;
    else if (scene.deck.front().image == CardImage::Licker)
        return GameEndReason::LickerLick;
    else if (scene.deck.front().image == CardImage::Tyrant)
        return GameEndReason::TyrantScratch;

    assert(false);
    return {};
}

bool GameRulesEngine::gameWon() const noexcept
{
    return scene.infectionProgress == -1;
}

std::optional<GameEvent>
GameRulesEngine::getEventAfterAnimationEnded(const Animation& animation)
{
    return std::visit(
        overloads {
            [](const AnimationCardToDiscard& a) -> std::optional<GameEvent>
            { return CardSkipEndedGameEvent(a.card); },
            [](const AnimationCardTransform&) -> std::optional<GameEvent>
            { return std::nullopt; },
            [](const AnimationDiscardToDeck&) -> std::optional<GameEvent>
            { return DiscardReturnedToDeckGameEvent(); },
            [](const AnimationDoorOpen& a) -> std::optional<GameEvent>
            { return DoorOpenedGameEvent(a.link); },
            [](const AnimationEnemyAttack& a) -> std::optional<GameEvent>
            {
                return MonsterReactionFinishedGameEvent(
                    a.skipCardAfterReaction);
            },
            [](const AnimationEnemyDamagedWindup& a) -> std::optional<GameEvent>
            {
                return AttackWindupAnimationEndedGameEvent(
                    a.damage, a.usedWeaponInventoryIdx);
            },
            [](const AnimationEnemyDamaged& a) -> std::optional<GameEvent>
            {
                return MonsterStaggerEndedGameEvent(
                    a.damage, a.usedWeaponInventoryIdx);
            },
            [](const AnimationEnemyDodgedAttack&) -> std::optional<GameEvent>
            { return std::nullopt; },
            [](const AnimationInvalidOperation&) -> std::optional<GameEvent>
            { return std::nullopt; },
            [](const AnimationNewCardsShufflingIntoDeck&)
                -> std::optional<GameEvent>
            { return NewCardShuffledToDiscard(); },
            [](const AnimationReturnInventoryToDeck&)
                -> std::optional<GameEvent> { return std::nullopt; },
            [](const AnimationTakeCard& a) -> std::optional<GameEvent>
            { return CardTakenGameEvent(a.inventorySlotIdx); },
            [](const AnimationTrashMainCard&) -> std::optional<GameEvent>
            { return MainCardTrashedGameEvent(); },
            [](const AnimationHeal&) -> std::optional<GameEvent>
            { return std::nullopt; } },
        animation);
}

dgm::Animation::PlaybackStatus
GameRulesEngine::updateHealAnimation(AnimationHeal& a, const dgm::Time& time)
{
    auto result = updateAnimation(a, time);
    if (a.isVaccineHeal)
    {
        scene.infectionProgress =
            static_cast<int>(std::lerp(a.initialInfection, -1, a.perc));
    }

    return result;
}

sf::Vector2f GameRulesEngine::screenToWorld(const sf::Vector2f& pos)
{
    return CoordConverter::screenToWorld(
        pos, INTERNAL_GAME_RESOLUTION_U, settings.resolution);
}

std::optional<size_t>
GameRulesEngine::findCollidingInventoryIdx(const sf::Vector2f& pointerPos)
{
    for (auto&& [idx, body] : std::views::enumerate(scene.inventoryBodies))
    {
        if (scene.inventory[idx].has_value()
            && dgm::Collision::basic(body, pointerPos))
            return idx;
    }

    return std::nullopt;
}

void GameRulesEngine::reloadWeapon(Card& weapon, int quantity)
{
    audioEngine.playSound(SoundId::WeaponReload);
    weapon.quantity = std::clamp(weapon.quantity + quantity, 0, MAX_AMMO);
}

void GameRulesEngine::popTopDeckCard()
{
    scene.deck.pop_front();
}

void GameRulesEngine::transformTopCard(CardType from, CardType to)
{
    scene.activeAnimation = AnimationCardTransform(from);

    scene.deck.front() = CardBuilder::createCard(to);
}

void GameRulesEngine::shuffleNewCardIntoDeck()
{
    if (scene.cardsToAdd.empty()) return;

    audioEngine.playSound(SoundId::CardShuffle);
    scene.activeAnimation = AnimationNewCardsShufflingIntoDeck();
}

void GameRulesEngine::combineCards(Card& inventoryCard, const Card& incoming)
{
    if (inventoryCard.special & CardSpecial::Combines
        && incoming.special & CardSpecial::Combines)
    {
        audioEngine.playSound(incoming.specialSound);
        inventoryCard = CardBuilder::combineCards(inventoryCard, incoming);
    }
    else if (
        inventoryCard.traits & CardTrait::Weapon
        && incoming.traits & CardTrait::Ammo)
    {
        reloadWeapon(inventoryCard, incoming.quantity);
    }
    else
    {
        throw std::runtime_error(uni::format(
            "GameRulesEngine::combineCards: Trying to mix incoming card {} "
            "with "
            "inventory card {} - operation not defined",
            std::to_underlying(incoming.image),
            std::to_underlying(inventoryCard.image)));
    }
}
