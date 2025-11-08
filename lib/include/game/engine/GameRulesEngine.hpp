#pragma once

#include "game/definitions/Scene.hpp"
#include "game/events/EventQueue.hpp"
#include "game/events/GameEvents.hpp"
#include "input/Input.hpp"
#include "settings/VideoSettings.hpp"

class [[nodiscard]] GameRulesEngine final
{
public:
    GameRulesEngine(
        EventQueue<GameEvent>& gameEventQueue,
        Scene& scene,
        Input& input,
        const VideoSettings& settings) noexcept
        : gameEventQueue(gameEventQueue)
        , scene(scene)
        , input(input)
        , settings(settings)
    {
    }

    GameRulesEngine(GameRulesEngine&&) = delete;
    GameRulesEngine(const GameRulesEngine&) = delete;

public:
    void operator()(const CardTakenGameEvent& e);

    void operator()(const CardSkippedGameEvent& e);

    void operator()(const InventoryCardTrashedGameEvent& e);

    void operator()(const InventoryCardUsedForHealingGameEvent& e);

    void operator()(const InventoryCardUsedOnMainCardGameEvent& e);

    void operator()(const MonsterReactionTriggeredGameEvent& e);

    void operator()(const MonsterReactionFinishedGameEvent& e);

    void operator()(const MainCardTrashedGameEvent& e);

public:
    void update(const dgm::Time& time);

    void updateActiveAnimation(const dgm::Time& time);

    std::optional<size_t> getUsableInventorySlot(const Card& card) const;

    static bool canCardsCombine(const Card& a, const Card& b);

private:
    sf::Vector2f screenToWorld(const sf::Vector2f& pos);

    std::optional<size_t>
    findCollidingInventoryIdx(const sf::Vector2f& pointerPos);

private:
    EventQueue<GameEvent>& gameEventQueue;
    Scene& scene;
    Input& input;
    const VideoSettings& settings;
};
