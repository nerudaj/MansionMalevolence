#include "game/engine/GameRulesEngine.hpp"
#include <algorithm>
#include <limits>

void GameRulesEngine::update(const dgm::Time& time)
{
    updateActiveAnimation(time);
    if (scene.activeAnimation.has_value()) return;

    if (input.isTakeButtonPressed()
        && scene.deck.front().traits & CardTrait::Pickable)
    {
        if (auto&& slotIdx = getEmptyInventorySlot())
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
            if (scene.deck.front().traits & CardTrait::Enemy)
                scene.hearts -= scene.deck.front().power;

            scene.deck.push_back(scene.deck.front());
            scene.deck.pop_front();
        }
        else if (scene.activeAnimation->kind == AnimationKind::TakeCard)
        {
            scene.inventory[scene.activeAnimation->data] = scene.deck.front();
            scene.deck.pop_front();
        }

        scene.activeAnimation = std::nullopt;
    }
}

std::optional<size_t> GameRulesEngine::getEmptyInventorySlot() const
{
    for (auto&& [idx, slot] : uniranges::enumerate_view(scene.inventory))
    {
        if (!slot) return idx;
    }

    return std::nullopt;
}
