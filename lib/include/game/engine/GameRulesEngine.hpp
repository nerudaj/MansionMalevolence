#pragma once

#include "game/Scene.hpp"
#include "game/events/EventQueue.hpp"
#include "game/events/GameEvents.hpp"
#include "input/Input.hpp"

class [[nodiscard]] GameRulesEngine final
{
public:
    GameRulesEngine(
        EventQueue<GameEvent>& gameEventQueue,
        Scene& scene,
        Input& input) noexcept
        : gameEventQueue(gameEventQueue), scene(scene), input(input)
    {
    }

    GameRulesEngine(GameRulesEngine&&) = delete;
    GameRulesEngine(const GameRulesEngine&) = delete;

public:
    void update(const dgm::Time& time);

private:
    void updateDummyAnimation(
        dgm::Animation& animation,
        const sf::Vector2f& forward,
        const dgm::Time& time);

    void updateDummy(DummyEntity& dummy, const float deltaTime);

private:
    EventQueue<GameEvent>& gameEventQueue;
    Scene& scene;
    Input& input;
};