#include "game/engine/GameRulesEngine.hpp"
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
}

void GameRulesEngine::operator()(const CardSkipStartedGameEvent& e)
{
    scene.activeAnimation = Animation {
        .kind = AnimationKind::SkipCard,
    };
}

void GameRulesEngine::operator()(const CardSkipEndedGameEvent&)
{
    scene.deck.push_back(scene.deck.front());
    popTopDeckCard();
}

void GameRulesEngine::operator()(const BeforeCardSkipGameEvent& e)
{
    const bool isEnemy = scene.deck.front().traits & CardTrait::Enemy;
    const auto special = scene.deck.front().special;
    const bool managedToEvade = rollForSuccess(
        special == CardSpecial::Blind      ? EVADE_CHANCE_BLIND
        : special == CardSpecial::Vigilant ? EVADE_CHANCE_VIGILANT
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
    // TODO: play sound
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

        if (card.link == SPECIAL_SHIELD_KEYDOOR
            || card.link == SPECIAL_DIAMOND_KEYDOOR)
        {
            SceneBuilder::spawnCardsAfterFirstKeyTarget(scene);
        }
        else if (
            card.link == SPECIAL_CREST_DOOR
            && deckCard.image == CardImage::CrestDoorEmpty)
        {
            scene.deck.front() =
                CardBuilder::createCard(CardType::CrestDoorWithOneCrest);
            return;
        }
        else if (
            card.link == SPECIAL_CREST_DOOR
            && deckCard.image == CardImage::CrestDoorWithOneCrest)
        {
            scene.won = true;
        }

        gameEventQueue.pushEvent<MainCardResolvedGameEvent>();
    }
    else if (deckCard.special == CardSpecial::Deposit)
    {
        scene.deck.push_front(*scene.inventory[e.inventorySlotIdx]);
        scene.inventory[e.inventorySlotIdx].reset();
        gameEventQueue.pushEvent<BeforeCardSkipGameEvent>();
    }
    else
    {
        // TODO: trigger return animation
        // TODO: play sound
    }
}

void GameRulesEngine::operator()(const MonsterReactionTriggeredGameEvent& e)
{
    scene.activeAnimation = Animation {
        .kind = AnimationKind::EnemyAttack,
        .data = static_cast<size_t>(e.skipCardAfterReaction),
    };
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

    // TODO: play sound
    --weapon.quantity;

    scene.activeAnimation = Animation {
        .kind = AnimationKind::EnemyDamaged,
        .duration = sf::seconds(0.25f),
        .data = static_cast<size_t>(weapon.power),
    };
}

void GameRulesEngine::operator()(const MonsterStaggerEndedGameEvent& e)
{
    auto& deckCard = scene.deck.front();
    deckCard.power -= e.damage;
    if (deckCard.power <= 0)
    {
        gameEventQueue.pushEvent<MainCardResolvedGameEvent>();
    }
    else if (deckCard.special == CardSpecial::Retaliate)
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
    if (!rollForSuccess(0.25f)) return;

    scene.deck.push_back(CardBuilder::createCard(CardType::CrimsonHead));
}

void GameRulesEngine::operator()(const MainCardResolvedGameEvent&)
{
    if (scene.deck.front().special == CardSpecial::SpawnCrimsonHead)
    {
        gameEventQueue.pushEvent<ZombieDiedGameEvent>();
    }

    scene.activeAnimation = Animation {
        .kind = AnimationKind::TrashMainCard,
    };
}

void GameRulesEngine::update(const dgm::Time& time)
{
    updateActiveAnimation(time);
    if (scene.activeAnimation.has_value()) return;

    scene.usableInventorySlot = getUsableInventorySlot(scene.deck.front());
    scene.canTakeCard = scene.deck.front().traits & CardTrait::Pickable
                        && scene.usableInventorySlot;
    scene.canSafelySkipCard = !(scene.deck.front().traits & CardTrait::Enemy);

    if (input.isTakeButtonPressed() && scene.canTakeCard)
    {
        scene.activeAnimation = Animation {
            .kind = AnimationKind::TakeCard,
            .data = *scene.usableInventorySlot,
        };
    }
    else if (input.isSkipButtonPressed())
    {
        gameEventQueue.pushEvent<BeforeCardSkipGameEvent>();
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
            .position = worldPos,
        };
    }
    else
    { // moved
        scene.dragDrop->position = worldPos;
    }
}

void GameRulesEngine::handleDragEnded()
{
    if (!scene.dragDrop->inventoryIdx.has_value())
    {
        scene.dragDrop.reset();
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

    scene.dragDrop.reset();
}

void GameRulesEngine::updateActiveAnimation(const dgm::Time& time)
{
    if (!scene.activeAnimation.has_value()) return;

    scene.activeAnimation->elapsed += time.getElapsed();

    if (scene.activeAnimation->elapsed > scene.activeAnimation->duration)
    {
        if (scene.activeAnimation->kind == AnimationKind::SkipCard)
        {
            gameEventQueue.pushEvent<CardSkipEndedGameEvent>();
        }
        else if (scene.activeAnimation->kind == AnimationKind::TakeCard)
        {
            gameEventQueue.pushEvent<CardTakenGameEvent>(
                scene.activeAnimation->data);
        }
        else if (scene.activeAnimation->kind == AnimationKind::TrashMainCard)
        {
            gameEventQueue.pushEvent<MainCardTrashedGameEvent>();
        }
        else if (scene.activeAnimation->kind == AnimationKind::EnemyAttack)
        {
            gameEventQueue.pushEvent<MonsterReactionFinishedGameEvent>(
                static_cast<bool>(scene.activeAnimation->data));
        }
        else if (scene.activeAnimation->kind == AnimationKind::EnemyDamaged)
        {
            gameEventQueue.pushEvent<MonsterStaggerEndedGameEvent>(
                static_cast<int>(scene.activeAnimation->data));
        }

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
           || inventoryCard.special == CardSpecial::Combines
                  && incomingCard.special == CardSpecial::Combines
                  && inventoryCard.link == incomingCard.link;
}

bool GameRulesEngine::canCardInteractWithDeck(
    const Card& a, const std::list<Card>& deck)
{
    const auto deckTraits = deck.front().traits;
    return deckTraits & CardTrait::Enemy && a.traits & CardTrait::Weapon
           || deckTraits & CardTrait::KeyTarget && a.traits & CardTrait::KeyItem
                  && deck.front().link == a.link
           || deck.front().special == CardSpecial::Deposit;
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
    // TODO: play sound
    weapon.quantity = std::clamp(weapon.quantity + quantity, 0, MAX_AMMO);
}

void GameRulesEngine::popTopDeckCard()
{
    scene.deck.pop_front();
}

bool GameRulesEngine::rollForSuccess(float chance)
{
    const auto DICE_SIDES = 6;
    const auto roll = std::random_device {}();
    return chance >= (roll % DICE_SIDES + 1) / static_cast<float>(DICE_SIDES);
}
