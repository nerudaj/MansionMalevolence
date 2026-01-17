#pragma once

#include <nlohmann/json.hpp>

enum class [[nodiscard]] GameScenario
{
    Tutorial_1,
    Easy,
    Normal,
    Hard,
    Nightmare,
    OneRoom,
};

NLOHMANN_JSON_SERIALIZE_ENUM(
    GameScenario,
    {
        { GameScenario::Tutorial_1, "Tutorial1" },
        { GameScenario::Easy, "Easy" },
        { GameScenario::Normal, "Normal" },
        { GameScenario::Hard, "Hard" },
        { GameScenario::Nightmare, "Nightmare" },
        { GameScenario::OneRoom, "OneRoom" },
    });
