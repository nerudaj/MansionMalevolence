#pragma once

#include "misc/Observable.hpp"
#include "strings/Language.hpp"
#include <nlohmann/json.hpp>

struct [[nodiscard]] AudioSettings_StorageModel final
{
    float soundVolume = 50.f;
    float musicVolume = 50.f;
    Language language = Language::English;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
    AudioSettings_StorageModel, soundVolume, musicVolume, language);

struct [[nodiscard]] AudioSettings_AppModel final
{
    Observable<float> soundVolume;
    Observable<float> musicVolume;
    Language language = Language::English;
};

/*
NOTE: Due to cyclical nature of certain dependencies, settings
must be loaded from disc to pure struct with regular data members.

That struct is used to create the Window and then can be passed
to DependencyContainer which will convert it into the application model.
*/
