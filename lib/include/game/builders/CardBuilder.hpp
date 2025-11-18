#pragma once

#include "game/definitions/Card.hpp"

class [[nodiscard]] CardBuilder final
{
public:
    static Card createCard(CardType type);

    static Card combineCards(const Card& a, const Card& b);
};
