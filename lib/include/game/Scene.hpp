#pragma once

#include "game/CardBuilder.hpp"
#include <DGM/dgm.hpp>
#include <array>
#include <list>

struct [[nodiscard]] Scene final
{
    std::list<Card> deck;
    std::array<std::optional<Card>, 3u> inventory = {};
    int hearts = 5;
};
