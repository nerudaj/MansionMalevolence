#pragma once

#include "settings/AudioSettings.hpp"
#include "settings/BindingsSettings.hpp"
#include "settings/InputSettings.hpp"
#include "settings/SavePosition.hpp"
#include "settings/VideoSettings.hpp"
#include <nlohmann/json.hpp>

struct [[nodiscard]] AppSettings_StorageModel final
{
    AudioSettings_StorageModel audio;
    VideoSettings video;
    InputSettings input;
    BindingsSettings bindings;
    SavePosition save;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
    AppSettings_StorageModel, audio, video, input, bindings, save);

struct [[nodiscard]] AppSettings final
{
    AudioSettings_AppModel audio;
    VideoSettings video;
    InputSettings input;
    BindingsSettings bindings;
    SavePosition save;
};

static inline AppSettings_StorageModel
fromAppSettingsModel(const AppSettings& s)
{
    return AppSettings_StorageModel
    {
        .audio = {
            .soundVolume = s.audio.soundVolume,
            .musicVolume = s.audio.musicVolume,
        },
        .video = s.video,
        .input = s.input,
        .bindings = s.bindings,
        .save = s.save,
    };
}
