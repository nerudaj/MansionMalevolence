#pragma once

#include "game/enums/CardImage.hpp"
#include "game/enums/CardSpecial.hpp"
#include "game/enums/CardTrait.hpp"
#include "game/enums/CardType.hpp"
#include <array>
#include <string>

struct [[nodiscard]] Card final
{
    CardImage image = {};
    CardTrait traits = CardTrait::None;
    CardSpecial special = CardSpecial::None;
    int quantity = 0;
    int power = 0;
    int link = 0; // for IDs that pair related cards together
    std::array<char, 17> name = {};
    std::array<char, 17> text1 = {};
    std::array<char, 17> text2 = {};
    std::array<char, 17> text3 = {};
    std::array<char, 17> text4 = {};
};
