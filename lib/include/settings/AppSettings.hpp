#pragma once

#include "settings/AudioSettings.hpp"
#include "settings/BindingsSettings.hpp"
#include "settings/InputSettings.hpp"
#include "settings/VideoSettings.hpp"
#include <nlohmann/json.hpp>

struct [[nodiscard]] AppSettings final
{
    AudioSettings audio;
    VideoSettings video;
    InputSettings input;
    BindingsSettings bindings;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AppSettings, audio, video, input, bindings);
