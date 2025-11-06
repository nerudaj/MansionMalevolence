#include "game/engine/GameRulesEngine.hpp"
#include <algorithm>
#include <limits>

void GameRulesEngine::operator()(const CardTakenGameEvent& e)
{
    if (scene.inventory[e.inventorySlotIdx].has_value())
    {
        // TODO: play sound
        scene.inventory[e.inventorySlotIdx] =
            CardBuilder::createCard(CardType::MixedHerbs);
    }
    else
    {
        scene.inventory[e.inventorySlotIdx] = scene.deck.front();
    }

    scene.deck.pop_front();
}

void GameRulesEngine::operator()(const CardSkippedGameEvent&)
{
    scene.deck.push_back(scene.deck.front());
    scene.deck.pop_front();
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
            gameEventQueue.pushEvent<MonsterReactionTriggeredGameEvent>(
                "skipCardAfterReaction"_false);
        }
    }
    // TODO: keys
    else
    {
        // TODO: trigger return animation
        // TODO: play sound
    }
}

void GameRulesEngine::operator()(const MonsterReactionTriggeredGameEvent& e)
{
    scene.hearts -= scene.deck.front().power;
    // TODO: trigger slash animation
    // TODO: play sound

    if (e.skipCardAfterReaction)
    {
        gameEventQueue.pushEvent<CardSkippedGameEvent>();
    }
}

void GameRulesEngine::operator()(const MainCardTrashedGameEvent& e)
{
    scene.deck.pop_front();
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

        scene.activeAnimation.reset();
    }
}

std::optional<size_t>
GameRulesEngine::getUsableInventorySlot(const Card& card) const
{
    // First check combinable cards
    for (auto&& [idx, slot] : std::ranges::views::enumerate(scene.inventory))
    {
        if (slot && canCardsCombine(*slot, card)) return idx;
    }

    // Then empty slots
    for (auto&& [idx, slot] : std::ranges::views::enumerate(scene.inventory))
    {
        if (!slot) return idx;
    }

    return std::nullopt;
}

bool GameRulesEngine::canCardsCombine(const Card& a, const Card& b)
{
    return std::max(a.image, b.image) == CardImage::GreenHerb
           && std::min(a.image, b.image) == CardImage::RedHerb;
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
        1 - (INTERNAL_GAME_RESOLUTION.y * scale / settings.resolution.y);

    return pos / scale
           - sf::Vector2f { 0.f, INTERNAL_GAME_RESOLUTION.y * yoffsetRelative };
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
