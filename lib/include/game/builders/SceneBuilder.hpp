#pragma once

#include "game/definitions/Scene.hpp"
#include "game/enums/GameScenario.hpp"

enum class [[nodiscard]] GameState
{
    Continue,
    Finished
};

class [[nodiscard]] SceneBuilder final
{
public:
    static Scene createScene(const GameScenario scenario);

    static GameState updateScene(Scene& scene, const int completedLinkID);

    static std::array<CardType, 3u> generateBooster();
};
