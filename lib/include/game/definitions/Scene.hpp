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
};
