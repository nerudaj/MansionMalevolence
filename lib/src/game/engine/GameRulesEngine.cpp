#include "game/engine/GameRulesEngine.hpp"
#include <algorithm>
#include <limits>

void GameRulesEngine::operator()(const CardTakenGameEvent& e)
{
    if (scene.inventory[e.inventorySlotIdx].has_value())
    {
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
    auto card = scene.deck.front();
    if (card.traits & CardTrait::Enemy) scene.hearts -= card.power;
    // TODO: trigger slash animation

    scene.deck.push_back(card);
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
        scene.activeAnimation = Animation {
            .kind = AnimationKind::SkipCard,
        };
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

        scene.activeAnimation = std::nullopt;
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
