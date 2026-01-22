#pragma once

#include "game/definitions/Scene.hpp"
#include "game/engine/AudioEngine.hpp"
#include "game/enums/GameEndReason.hpp"
#include "game/events/EventQueue.hpp"
#include "game/events/GameEvents.hpp"
#include "input/Input.hpp"
#include "settings/VideoSettings.hpp"

class [[nodiscard]] GameRulesEngine final
{
public:
    GameRulesEngine(
        EventQueue<GameEvent>& gameEventQueue,
        AudioEngine& audioEngine,
        Scene& scene,
        Input& input,
        const VideoSettings& settings) noexcept
        : gameEventQueue(gameEventQueue)
        , audioEngine(audioEngine)
        , scene(scene)
        , input(input)
        , settings(settings)
    {
        scene.activeAnimation = AnimationFadeIn();
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

    void operator()(const CardUsedOnAnotherInventoryCardGameEvent& e);

    void operator()(const ZombieDiedGameEvent& e);

    void operator()(const MainCardResolvedGameEvent& e);

    void operator()(const AttackWindupAnimationEndedGameEvent&);

    void operator()(const FlipCardAnimationEndedGameEvent&);

public:
    void update(const dgm::Time& time);

    void handleSkip();

    void handleTake();

    void handleDragEnded();

    void handleFinishedAnimation();

    std::optional<size_t> getUsableInventorySlot(const Card& card) const;

    static bool canInventoryCardCombineWithIncoming(
        const Card& inventoryCard, const Card& incomingCard);

    static bool
    canCardInteractWithMainCard(const Card& a, const Card& mainCard);

    [[nodiscard]] bool gameEnded() const noexcept;

    [[nodiscard]] bool gameWon() const noexcept;

    [[nodiscard]] GameEndReason getGameEnding() const noexcept;

    dgm::Animation::PlaybackStatus
    updateHealAnimation(AnimationHeal& a, const dgm::Time& time);

private:
    sf::Vector2f screenToWorld(const sf::Vector2f& pos);

    void handleDragStartedOrMoved(sf::Vector2f& pos);

    void updateActiveAnimation(const dgm::Time& time);

    std::optional<size_t>
    findCollidingInventoryIdx(const sf::Vector2f& pointerPos);

    void reloadWeapon(Card& weapon, int quantity);

    void transformTopCard(CardType from, CardType to);

    void shuffleNewCardIntoDeck();

    void combineCards(Card& inventoryCard, const Card& incoming);

private:
    EventQueue<GameEvent>& gameEventQueue;
    AudioEngine& audioEngine;
    Scene& scene;
    Input& input;
    const VideoSettings& settings;
};
