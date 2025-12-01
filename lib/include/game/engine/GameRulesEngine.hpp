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

    void operator()(const CardSkipStartedGameEvent& e);

    void operator()(const CardSkipEndedGameEvent& e);

    void operator()(const BeforeCardSkipGameEvent& e);

    void operator()(const InventoryCardTrashedGameEvent& e);

    void operator()(const InventoryCardUsedForHealingGameEvent& e);

    void operator()(const InventoryCardUsedOnMainCardGameEvent& e);

    void operator()(const MonsterReactionTriggeredGameEvent& e);

    void operator()(const MonsterReactionFinishedGameEvent& e);

    void operator()(const MonsterShotAtGameEvent& e);

    void operator()(const MonsterStaggerEndedGameEvent& e);

    void operator()(const MainCardTrashedGameEvent& e);

    void operator()(const CardUsedOnAnotherInventoryCardGameEvent& e);

    void operator()(const ZombieDiedGameEvent& e);

    void operator()(const MainCardResolvedGameEvent& e);

    void operator()(const DoorOpenedGameEvent& e);

    void operator()(const ShuffleNewCardsIntoDeck& e);

public:
    void update(const dgm::Time& time);

    void handleSkip();

    void handleTake();

    void handleDragEnded();

    std::optional<size_t> getUsableInventorySlot(const Card& card) const;

    static bool canInventoryCardCombineWithIncoming(
        const Card& inventoryCard, const Card& incomingCard);

    static bool
    canCardInteractWithDeck(const Card& a, const std::list<Card>& deck);

private:
    sf::Vector2f screenToWorld(const sf::Vector2f& pos);

    void handleDragStartedOrMoved(sf::Vector2f& pos);

    void updateActiveAnimation(const dgm::Time& time);

    std::optional<size_t>
    findCollidingInventoryIdx(const sf::Vector2f& pointerPos);

    void reloadWeapon(Card& weapon, int quantity);

    void popTopDeckCard();

    bool rollForSuccess(float chance);

private:
    EventQueue<GameEvent>& gameEventQueue;
    Scene& scene;
    Input& input;
    const VideoSettings& settings;
};
