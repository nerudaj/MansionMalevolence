#pragma once

#include <SFML/System/Vector2.hpp>
#include <optional>

struct [[nodiscard]] DragDrop final
{
    std::optional<size_t> inventoryIdx;
    bool canTrashCard;
    bool draggingMainCard;
    sf::Vector2f position;
    sf::Vector2f initialPosition;
};
