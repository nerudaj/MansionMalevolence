#pragma once

#include "game/enums/CardImage.hpp"
#include "game/enums/CardType.hpp"
#include <map>
#include <string>

struct [[nodiscard]] Card final
{
    CardImage image = {};
    int ammo = 0;
    int power = 0;
    bool bites = false;
    std::array<char, 17> name = {};
    std::array<char, 17> text1 = {};
    std::array<char, 17> text2 = {};
    std::array<char, 17> text3 = {};
    std::array<char, 17> text4 = {};
};

const std::map<CardType, Card> CARD_DEFS = {
    { CardType::Pistol,
      Card { .image = CardImage::Pistol,
             .ammo = 3,
             .power = 1,
             .name = "pistol",
             .text1 = "it uses 9mm ammo",
             .text2 = "standard police",
             .text3 = "firearm" } },
    { CardType::Shotgun,
      Card { .image = CardImage::Shotgun,
             .ammo = 2,
             .power = 2,
             .name = "shotgun",
             .text1 = "remington m1100" } }
};
