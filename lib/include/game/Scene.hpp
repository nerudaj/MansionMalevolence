#pragma once

#include <DGM/dgm.hpp>

// You can delete this one
struct DummyEntity
{
    dgm::Rect body;
    dgm::Animation animation;
    bool facingLeft = false;
    sf::Vector2f forward = { 0.f, 0.f };
};

struct [[nodiscard]] Scene final
{
    DummyEntity dummy;
    sf::Vector2f groundPosition;
};
