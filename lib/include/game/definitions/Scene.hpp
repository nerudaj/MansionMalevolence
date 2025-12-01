#pragma once

#include "game/animations/AnimationInterface.hpp"
#include "game/builders/CardBuilder.hpp"
#include "game/definitions/GameStats.hpp"
#include "game/enums/GameScenario.hpp"
#include <DGM/dgm.hpp>
#include <array>
#include <list>
#include <memory>
#include <optional>

const static inline auto INTERNAL_GAME_RESOLUTION =
    sf::Vector2f { 128.f, 228.f };
constexpr const int MAX_HEARTS = 5;
constexpr const int MAX_AMMO = 3;
constexpr const float CRIMSON_HEAD_SPAWN_CHANCE = 0.5f;
constexpr const float EVADE_CHANCE_REGULAR = 0.65f;
constexpr const float EVADE_CHANCE_BLIND = 0.7f;
constexpr const float EVADE_CHANCE_VIGILANT = 0.15f;
constexpr const float EVADE_CHANCE_EVASIVE = 0.3f;

struct [[nodiscard]] DragDrop final
{
    std::optional<size_t> inventoryIdx;
    bool canTrashCard;
    bool draggingMainCard;
    sf::Vector2f position;
    sf::Vector2f initialPosition;
};

struct [[nodiscard]] Chance final
{
    int evasiveChance = 0;
    int crimsonHeadChance = 0;
};

struct [[nodiscard]] Scene final
{
    GameScenario scenario = {};
    bool won = false;
    bool lost = false;
    std::list<Card> deck;
    std::array<std::optional<Card>, 3u> inventory = {};
    int hearts = MAX_HEARTS;
    std::unique_ptr<AnimationInterface> activeAnimation = nullptr;
    std::optional<DragDrop> dragDrop;

    dgm::Rect mainCardBody;
    dgm::Rect healthbarBody;
    dgm::Rect trashBody;
    std::array<dgm::Rect, 3u> inventoryBodies;

    // These are precomputed in each update()
    // and are used both by game and rendering logic.
    std::optional<size_t> usableInventorySlot;
    bool canTakeCard = false;
    bool canSafelySkipCard = false;
    bool preventInteractions = false;

    GameStats stats;
    Chance chance;
};
