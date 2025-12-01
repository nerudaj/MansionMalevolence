#include "game/engine/GameRulesEngine.hpp"
#include "game/animations/Animations.hpp"
#include "game/builders/SceneBuilder.hpp"
#include <algorithm>
#include <limits>
#include <random>

void GameRulesEngine::operator()(const CardTakenGameEvent& e)
{
    if (scene.inventory[e.inventorySlotIdx].has_value())
    {
        const auto& inventoryCard = scene.inventory[e.inventorySlotIdx].value();
        if (inventoryCard.special == CardSpecial::Combines)
        {
            // TODO: play sound
            scene.inventory[e.inventorySlotIdx] =
                CardBuilder::combineCards(inventoryCard, scene.deck.front());
        }
        else if (
            scene.inventory[e.inventorySlotIdx].value().traits
            & CardTrait::Weapon)
        {
            reloadWeapon(
                *scene.inventory[e.inventorySlotIdx],
                scene.deck.front().quantity);
        }
        else
        {
            throw std::runtime_error(uni::format(
                "CardTakenGameEvent: Trying to mix incoming card {} with "
                "inventory card {} - operation not defined",
                std::to_underlying(scene.deck.front().image),
                std::to_underlying(
                    scene.inventory[e.inventorySlotIdx]->image)));
        }
    }
    else
    {
        scene.inventory[e.inventorySlotIdx] = scene.deck.front();
    }

    popTopDeckCard();
    scene.stats.turnsTaken++;
}

void GameRulesEngine::operator()(const CardSkipStartedGameEvent&)
{
    scene.stats.turnsTaken++;
    scene.activeAnimation = std::make_unique<AnimationSkipCard>();
    audioEngine.playSound(SoundId::CardShuffle);
}

void GameRulesEngine::operator()(const CardSkipEndedGameEvent&)
{
    scene.deck.push_back(scene.deck.front());
    popTopDeckCard();
}

void GameRulesEngine::operator()(const BeforeCardSkipGameEvent&)
{
    const bool isEnemy = scene.deck.front().traits & CardTrait::Enemy;
    const auto special = scene.deck.front().special;
    const bool managedToEvade = rollForSuccess(
        special & CardSpecial::Blind      ? EVADE_CHANCE_BLIND
        : special & CardSpecial::Vigilant ? EVADE_CHANCE_VIGILANT
                                          : EVADE_CHANCE_REGULAR);

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
    audioEngine.playSound(SoundId::CardDestroy);
    scene.inventory[e.inventorySlotIdx].reset();
}

void GameRulesEngine::operator()(const InventoryCardUsedForHealingGameEvent& e)
{
    auto& card = scene.inventory[e.inventorySlotIdx].value();
    if (!(card.traits & CardTrait::Healing))
    {
        // TODO: trigger return animation
        // TODO: play sound
        return;
    }

    scene.hearts = std::clamp(scene.hearts + card.power, 0, MAX_HEARTS);
    // TODO: play healing sound
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
            scene.activeAnimation = std::make_unique<AnimationCardTransform>(
                CardType::CrestDoorEmpty);

            scene.deck.front() =
                CardBuilder::createCard(CardType::CrestDoorWithOneCrest);
        }
        else
        {
            scene.activeAnimation =
                std::make_unique<AnimationDoorOpen>(card.link);
        }
    }
    else if (deckCard.special & CardSpecial::Deposit)
    {
        scene.deck.push_front(*scene.inventory[e.inventorySlotIdx]);
        scene.inventory[e.inventorySlotIdx].reset();
        gameEventQueue.pushEvent<BeforeCardSkipGameEvent>();
    }
    else
    {
        // TODO: trigger return animation
        // TODO: play sound
        return; // skip stats increment
    }

    scene.stats.turnsTaken++;
}

void GameRulesEngine::operator()(const MonsterReactionTriggeredGameEvent& e)
{
    scene.activeAnimation =
        std::make_unique<AnimationEnemyAttack>(e.skipCardAfterReaction);
    audioEngine.playSound(scene.deck.front().specialSound);
}

void GameRulesEngine::operator()(const MonsterReactionFinishedGameEvent& e)
{
    scene.hearts -= scene.deck.front().power;
    // TODO: trigger slash animation
    // TODO: play sound

    scene.lost = scene.hearts <= 0;

    if (e.skipCardAfterReaction)
    {
        gameEventQueue.pushEvent<CardSkipStartedGameEvent>();
    }
}

void GameRulesEngine::operator()(const MonsterShotAtGameEvent& e)
{
    auto& weapon = scene.inventory[e.inventoryWeaponIdx].value();

    audioEngine.playSound(weapon.specialSound);
    --weapon.quantity;
    scene.stats.shotsFired++;

    if (scene.deck.front().special & CardSpecial::Evasive
        && rollForSuccess(EVADE_CHANCE_EVASIVE))
    {
        scene.activeAnimation = std::make_unique<AnimationEnemyDodgedAttack>();
    }
    else
    {
        scene.activeAnimation =
            std::make_unique<AnimationEnemyDamaged>(weapon.power);
    }
}

void GameRulesEngine::operator()(const MonsterStaggerEndedGameEvent& e)
{
    auto& deckCard = scene.deck.front();
    deckCard.power -= e.damage;
    if (deckCard.power <= 0)
    {
        // TODO: play die sound
        gameEventQueue.pushEvent<MainCardResolvedGameEvent>();
        scene.stats.enemiesKilled++;
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

    if (dst.traits & CardTrait::Weapon && src.traits & CardTrait::Ammo)
    {
        reloadWeapon(dst, src.quantity);
    }
    else if (
        dst.image == CardImage::GreenHerb || src.image == CardImage::GreenHerb)
    {
        // TODO: play sound
        dst = CardBuilder::createCard(CardType::MixedHerbs);
    }
    else if (
        dst.image == CardImage::MoonCrestLeft
        || src.image == CardImage::MoonCrestLeft)
    {
        // TODO: play sound
        dst = CardBuilder::createCard(CardType::MoonCrest);
    }

    scene.inventory[e.sourceCardInventoryIdx].reset();
}

void GameRulesEngine::operator()(const ZombieDiedGameEvent&)
{
    if (!rollForSuccess(CRIMSON_HEAD_SPAWN_CHANCE)) return;

    scene.deck.push_back(CardBuilder::createCard(CardType::CrimsonHead));
}

void GameRulesEngine::operator()(const MainCardResolvedGameEvent&)
{
    if (scene.deck.front().special & CardSpecial::SpawnCrimsonHead)
    {
        gameEventQueue.pushEvent<ZombieDiedGameEvent>();
    }

    scene.activeAnimation = std::make_unique<AnimationTrashMainCard>();
    audioEngine.playSound(SoundId::CardDestroy);
}

void GameRulesEngine::operator()(const DoorOpenedGameEvent& e)
{
    auto preDeckCount = scene.deck.size();
    if (SceneBuilder::updateScene(scene, e.link) == GameState::Finished)
    {
        scene.won = true;
        return;
    }

    auto postDeckCount = scene.deck.size();
    scene.deck.pop_front();
    gameEventQueue.pushEvent<ShuffleNewCardsIntoDeck>(
        static_cast<int>(postDeckCount - preDeckCount));
}

void GameRulesEngine::operator()(const ShuffleNewCardsIntoDeck& e)
{
    audioEngine.playSound(SoundId::CardShuffle);
    scene.activeAnimation =
        std::make_unique<AnimationNewCardsShufflingIntoDeck>(e.cardCount);
}

void GameRulesEngine::update(const dgm::Time& time)
{
    updateActiveAnimation(time);
    if (scene.activeAnimation) return;

    scene.usableInventorySlot = getUsableInventorySlot(scene.deck.front());
    scene.canTakeCard = scene.deck.front().traits & CardTrait::Pickable
                        && scene.usableInventorySlot;
    scene.canSafelySkipCard = !(scene.deck.front().traits & CardTrait::Enemy);

    if (scene.preventInteractions) return;

    if (input.isTakeButtonPressed())
    {
        handleTake();
    }
    else if (input.isSkipButtonPressed())
    {
        handleSkip();
    }
    else if (auto pos = input.getDragPosition(); pos != sf::Vector2f {})
    {
        handleDragStartedOrMoved(pos);
    }
    else if (scene.dragDrop.has_value())
    {
        handleDragEnded();
    }
}

void GameRulesEngine::handleTake()
{
    scene.preventInteractions = true;
    if (scene.canTakeCard)
    {
        scene.activeAnimation =
            std::make_unique<AnimationTakeCard>(*scene.usableInventorySlot);
    }
    else
    {
        scene.activeAnimation = std::make_unique<AnimationInvalidOperation>();
    }
}

void GameRulesEngine::handleSkip()
{
    scene.preventInteractions = true;
    gameEventQueue.pushEvent<BeforeCardSkipGameEvent>();
}

void GameRulesEngine::handleDragStartedOrMoved(sf::Vector2f& pos)
{
    const auto worldPos = screenToWorld(pos);
    if (!scene.dragDrop.has_value())
    { // started
        auto&& inventoryIdx = findCollidingInventoryIdx(worldPos);
        auto&& traits = inventoryIdx
                            ? scene.inventory[*inventoryIdx].value().traits
                            : CardTrait::None;
        scene.dragDrop = DragDrop {
            .inventoryIdx = std::move(inventoryIdx),
            .canTrashCard = !(
                traits & CardTrait::KeyItem || traits & CardTrait::KeyItemPart),
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
    ScopeGuard(Callable&& callable) : callable(std::forward<Callable>(callable))
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
        if (scene.dragDrop->canTrashCard)
        {
            gameEventQueue.pushEvent<InventoryCardTrashedGameEvent>(
                inventoryIdx);
        }
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

    if (scene.activeAnimation->update(time)
        == dgm::Animation::PlaybackStatus::Finished)
    {
        auto event = scene.activeAnimation->finalize();
        if (event) gameEventQueue.pushEvent(std::move(*event));

        // Clean inputs pressed during animation
        std::ignore = input.isSkipButtonPressed();
        std::ignore = input.isTakeButtonPressed();
        scene.preventInteractions = false;

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
                  && deck.front().link == a.link
           || deck.front().special & CardSpecial::Deposit;
}

sf::Vector2f GameRulesEngine::screenToWorld(const sf::Vector2f& pos)
{
    // precondition check
    if (settings.resolution.x > settings.resolution.y)
        throw std::runtime_error(
            "Playing on landscape resolution is unsupported");

    // NOTE: I assume this game will always be played on a portrait aspect ratio
    // therefore the viewport can be offset on Y axis, but not on X axis.
    const float scale = settings.resolution.x / INTERNAL_GAME_RESOLUTION.x;
    const float yoffsetRelative =
        (1.f - (INTERNAL_GAME_RESOLUTION.y * scale / settings.resolution.y))
        / 2.f;

    return (pos - sf::Vector2f { 0.f, settings.resolution.y * yoffsetRelative })
           / scale;
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
    scene.preventInteractions = false;
}

bool GameRulesEngine::rollForSuccess(float chance)
{
    const auto roll = std::random_device {}();
    return chance >= (roll % 100) / static_cast<float>(100);
}
