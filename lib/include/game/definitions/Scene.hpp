#pragma once

#include "game/builders/CardBuilder.hpp"
#include "game/enums/AnimationKind.hpp"
#include <DGM/dgm.hpp>
#include <array>
#include <list>
#include <optional>

const static inline auto INTERNAL_GAME_RESOLUTION =
    sf::Vector2f { 128.f, 228.f };
constexpr const int MAX_HEARTS = 5;
constexpr const int MAX_AMMO = 3;
constexpr const float EVADE_CHANCE_REGULAR = 0.5f;
constexpr const float EVADE_CHANCE_BLIND = 0.8f;
constexpr const float EVADE_CHANCE_VIGILANT = 0.2f;

struct [[nodiscard]] Animation final
{
    AnimationKind kind = {};
    sf::Time elapsed = sf::seconds(0.f);
    sf::Time duration = sf::seconds(0.5f);
    size_t data = 0;
};

struct [[nodiscard]] DragDrop final
{
    std::optional<size_t> inventoryIdx;
    bool canTrashCard;
    sf::Vector2f position;
};

struct [[nodiscard]] Scene final
{
    bool won = false;
    bool lost = false;
    std::list<Card> deck;
    std::array<std::optional<Card>, 3u> inventory = {};
    int hearts = MAX_HEARTS;
    std::optional<Animation> activeAnimation;
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
};
