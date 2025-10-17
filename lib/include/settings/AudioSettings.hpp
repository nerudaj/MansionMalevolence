#pragma once

#include <nlohmann/json.hpp>

struct [[nodiscard]] AudioSettings final
{
    float soundVolume = 50.f;
    float musicVolume = 50.f;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AudioSettings, soundVolume, musicVolume);
