#pragma once

#include "game/definitions/Scene.hpp"
#include "game/enums/GameScenario.hpp"

class [[nodiscard]] SceneBuilder final
{
public:
    static Scene createScene(const GameScenario scenario);

    static std::list<Card>
    getCardsForRoom(const GameScenario scenario, const int completedLinkID);

    static std::array<CardType, 3u> generateBooster();
};
