#pragma once

#include "game/animations/Animations.hpp"
#include "game/builders/CardBuilder.hpp"
#include "game/definitions/Chance.hpp"
#include "game/definitions/DragDrop.hpp"
#include "game/definitions/GameStats.hpp"
#include "game/enums/GameScenario.hpp"
#include <DGM/dgm.hpp>
#include <array>
#include <list>
#include <memory>
#include <optional>

const static inline auto INTERNAL_GAME_RESOLUTION =
    sf::Vector2f { 128.f, 228.f };
const static inline auto INTERNAL_GAME_RESOLUTION_U =
    sf::Vector2u(INTERNAL_GAME_RESOLUTION);
constexpr const int MAX_HEARTS = 5;
constexpr const int MAX_AMMO = 3;
constexpr const float CRIMSON_HEAD_SPAWN_CHANCE = 0.5f;
constexpr const float EVADE_CHANCE_REGULAR = 0.65f;
constexpr const float EVADE_CHANCE_BLIND = 0.7f;
constexpr const float EVADE_CHANCE_VIGILANT = 0.15f;
constexpr const float EVADE_CHANCE_EVASIVE = 0.3f;
const static inline auto CARD_SIZE = sf::Vector2f { 76.f, 114.f };

[[nodiscard]] static sf::Vector2f getNthInventoryCardOffset(size_t idx) noexcept
{
    return sf::Vector2f { 8.f + idx * 43.5f, 182.f };
}

[[nodiscard]] static sf::Vector2f getNthBoosterChoiceOffset(size_t idx) noexcept
{
    return sf::Vector2f { 8.f + idx * 43.5f, 109.f };
}

class [[nodiscard]] ScenarioBuilderInterface
{
public:
    virtual ~ScenarioBuilderInterface() = default;

public:
    virtual int getInfectionLimit() const noexcept = 0;

    virtual std::list<Card> generateStartRoom() = 0;

    virtual std::list<Card> generateRoomDeck(const int linkID) = 0;
};

struct [[nodiscard]] Scene final
{
    std::list<Card> deck;
    int infectionProgress = 0;
    int infectionLimit = -1;
    std::unique_ptr<ScenarioBuilderInterface> builder;
    std::list<Card> discard = {};
    std::list<Card> cardsToAdd = {};
    std::array<std::optional<Card>, 3u> inventory = {};
    int hearts = MAX_HEARTS;
    std::optional<Animation> activeAnimation = std::nullopt;
    std::optional<DragDrop> dragDrop = std::nullopt;
    std::optional<std::array<CardType, 3u>> boosterChoice = std::nullopt;

    dgm::Rect mainCardBody = dgm::Rect(sf::Vector2f { 6.f, 27.f }, CARD_SIZE);
    dgm::Rect healthbarBody = dgm::Rect({ 3.f, 150.f }, { 80.f, 24.f });
    dgm::Rect trashBody = dgm::Rect({ 94.f, 102.f }, CARD_SIZE / 3.f);
    std::array<dgm::Rect, 3u> inventoryBodies = {
        dgm::Rect(getNthInventoryCardOffset(0), CARD_SIZE / 3.f),
        dgm::Rect(getNthInventoryCardOffset(1), CARD_SIZE / 3.f),
        dgm::Rect(getNthInventoryCardOffset(2), CARD_SIZE / 3.f),
    };
    std::array<dgm::Rect, 3u> choiceBodies = {
        dgm::Rect(getNthBoosterChoiceOffset(0), CARD_SIZE / 3.f),
        dgm::Rect(getNthBoosterChoiceOffset(1), CARD_SIZE / 3.f),
        dgm::Rect(getNthBoosterChoiceOffset(2), CARD_SIZE / 3.f),
    };

    // These are precomputed in each update()
    // and are used both by game and rendering logic.
    std::optional<size_t> usableInventorySlot;
    bool canTakeCard = false;
    bool canSafelySkipCard = false;

    GameStats stats;
    Chance chance;
};
