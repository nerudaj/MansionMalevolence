#include "game/engine/GameRulesEngine.hpp"
#include "game/builders/SceneBuilder.hpp"
#include <algorithm>
#include <limits>

void GameRulesEngine::operator()(const CardTakenGameEvent& e)
{
    if (scene.inventory[e.inventorySlotIdx].has_value())
    {
        const auto image = scene.inventory[e.inventorySlotIdx].value().image;
        if (image == CardImage::GreenHerb || image == CardImage::RedHerb)
        {
            // TODO: play sound
            scene.inventory[e.inventorySlotIdx] =
                CardBuilder::createCard(CardType::MixedHerbs);
        }
        else if (
            image == CardImage::MoonCrestLeft
            || image == CardImage::MoonCrestRight)
        {
            // TODO: play sound
            scene.inventory[e.inventorySlotIdx] =
                CardBuilder::createCard(CardType::MoonCrest);
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

void GameRulesEngine::operator()(const CardSkippedGameEvent&)
{
    scene.deck.push_back(scene.deck.front());
    popTopDeckCard();
}

void GameRulesEngine::operator()(const InventoryCardTrashedGameEvent& e)
{
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
    if (card.traits & CardTrait::Weapon
        && scene.deck.front().traits & CardTrait::Enemy)
    {
        if (card.quantity == 0)
        {
            // TODO: fail
            return;
        }

        // TODO: play sound
        scene.deck.front().power -= card.power;
        --card.quantity;

        if (scene.deck.front().power <= 0)
        {
            scene.activeAnimation = Animation {
                .kind = AnimationKind::TrashMainCard,
            };
        }
        else
        {
            // TODO: trigger only for tyrant
        }
    }
    else if (
        card.traits & CardTrait::KeyItem1
        && scene.deck.front().traits & CardTrait::KeyTarget1)
    {
        scene.inventory[e.inventorySlotIdx].reset();
        SceneBuilder::spawnCardsAfterFirstKeyTarget(scene);
        scene.activeAnimation = Animation {
            .kind = AnimationKind::TrashMainCard,
        };
    }
    else if (
        card.traits & CardTrait::KeyItem2
        && scene.deck.front().traits & CardTrait::KeyTarget2)
    {
        scene.won = true;
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
        scene.activeAnimation = Animation {
            .kind = AnimationKind::SkipCard,
        };
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

void GameRulesEngine::update(const dgm::Time& time)
{
    updateActiveAnimation(time);
    if (scene.activeAnimation.has_value()) return;

    if (input.isTakeButtonPressed()
        && scene.deck.front().traits & CardTrait::Pickable)
    {
        if (auto&& slotIdx = getUsableInventorySlot(scene.deck.front()))
        {
            scene.activeAnimation = Animation {
                .kind = AnimationKind::TakeCard,
                .data = *slotIdx,
            };
        }
    }
    else if (input.isSkipButtonPressed())
    {
        if (scene.deck.front().traits & CardTrait::Enemy)
            gameEventQueue.pushEvent<MonsterReactionTriggeredGameEvent>(
                "skipCardAfterReaction"_true);
        else
        {
            scene.activeAnimation = Animation {
                .kind = AnimationKind::SkipCard,
            };
        }
    }
    else if (auto pos = input.getDragPosition(); pos != sf::Vector2f {})
    { // drag'n'drop started/moved
        const auto worldPos = screenToWorld(pos);
        if (!scene.dragDrop.has_value())
        { // started
            scene.dragDrop = DragDrop {
                .inventoryIdx = findCollidingInventoryIdx(worldPos),
                .position = worldPos,
            };
        }
        else
        { // moved
            scene.dragDrop->position = worldPos;
        }
    }
    else if (scene.dragDrop.has_value())
    { // drag'n'drop ended, evaluate
        if (scene.dragDrop->inventoryIdx.has_value())
        {
            if (dgm::Collision::basic(
                    scene.mainCardBody, scene.dragDrop->position))
            {
                gameEventQueue.pushEvent<InventoryCardUsedOnMainCardGameEvent>(
                    scene.dragDrop->inventoryIdx.value());
            }
            else if (dgm::Collision::basic(
                         scene.healthbarBody, scene.dragDrop->position))
            {
                gameEventQueue.pushEvent<InventoryCardUsedForHealingGameEvent>(
                    scene.dragDrop->inventoryIdx.value());
            }
            else if (dgm::Collision::basic(
                         scene.trashBody, scene.dragDrop->position))
            {
                gameEventQueue.pushEvent<InventoryCardTrashedGameEvent>(
                    scene.dragDrop->inventoryIdx.value());
            }
            else
            {
                for (auto&& [idx, inventoryBody] :
                     std::views::enumerate(scene.inventoryBodies))
                {
                    if (!scene.inventory[idx].has_value()
                        || static_cast<size_t>(idx)
                               == scene.dragDrop->inventoryIdx.value())
                        continue;

                    if (dgm::Collision::basic(
                            inventoryBody, scene.dragDrop->position)
                        && canInventoryCardCombineWithIncoming(
                            *scene.inventory[idx],
                            *scene.inventory[*scene.dragDrop->inventoryIdx]))
                    {
                        gameEventQueue
                            .pushEvent<CardUsedOnAnotherInventoryCardGameEvent>(
                                *scene.dragDrop->inventoryIdx,
                                static_cast<size_t>(idx));
                    }
                }
            }
        }

        scene.dragDrop.reset();
    }
}

void GameRulesEngine::updateActiveAnimation(const dgm::Time& time)
{
    if (!scene.activeAnimation.has_value()) return;

    scene.activeAnimation->elapsed += time.getElapsed();

    if (scene.activeAnimation->elapsed > scene.activeAnimation->duration)
    {
        if (scene.activeAnimation->kind == AnimationKind::SkipCard)
        {
            gameEventQueue.pushEvent<CardSkippedGameEvent>();
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
    return std::max(inventoryCard.image, incomingCard.image)
                   == CardImage::GreenHerb
               && std::min(inventoryCard.image, incomingCard.image)
                      == CardImage::RedHerb
           || inventoryCard.traits & CardTrait::Weapon
                  && incomingCard.traits & CardTrait::Ammo
                  && inventoryCard.quantity < MAX_AMMO
           || std::min(inventoryCard.image, incomingCard.image)
                      == CardImage::MoonCrestLeft
                  && std::max(inventoryCard.image, incomingCard.image)
                         == CardImage::MoonCrestRight;
}

bool GameRulesEngine::canCardInteractWithDeck(
    const Card& a, const std::list<Card>& deck)
{
    const auto deckTraits = deck.front().traits;
    return deckTraits & CardTrait::Enemy && a.traits & CardTrait::Weapon
           || deckTraits & CardTrait::KeyTarget1
                  && a.traits & CardTrait::KeyItem1
           || deckTraits & CardTrait::KeyTarget2
                  && a.traits & CardTrait::KeyItem2;
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

    if (scene.deck.front().image == CardImage::Cerberus)
    {
        gameEventQueue.pushEvent<MonsterReactionTriggeredGameEvent>(
            "skipCardAfterReaction"_false);
    }
}
