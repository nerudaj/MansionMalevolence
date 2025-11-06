#pragma once

#include "game/definitions/Card.hpp"

class [[nodiscard]] CardBuilder final
{
public:
    static Card createCard(CardType type);
};
