#pragma once

#include "game/CardBuilder.hpp"
#include "game/enums/AnimationKind.hpp"
#include <DGM/dgm.hpp>
#include <array>
#include <list>
#include <optional>

const static inline auto INTERNAL_GAME_RESOLUTION =
    sf::Vector2f { 128.f, 228.f };

struct [[nodiscard]] Animation final
{
    AnimationKind kind = {};
    sf::Time elapsed = sf::seconds(0.f);
    sf::Time duration = sf::seconds(0.5f);
    size_t data = 0;
};

struct [[nodiscard]] Scene final
{
    std::list<Card> deck;
    std::array<std::optional<Card>, 3u> inventory = {};
    int hearts = 5;
    std::optional<Animation> activeAnimation;

    /*dgm::Rect mainCardBody;
    dgm::Rect healthbarBody;
    dgm::Rect trashBody;
    dgm::Rect inventory1Body;
    dgm::Rect inventory2Body;
    dgm::Rect inventory3Body;*/
};
