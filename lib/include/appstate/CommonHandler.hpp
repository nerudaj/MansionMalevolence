#pragma once

#include "misc/DependencyContainer.hpp"
#include "settings/InputSettings.hpp"
#include <DGM/dgm.hpp>

struct [[nodiscard]] CommonHandlerOptions final
{
    bool disableGoBack = false;
};

class CommonHandler final
{
public:
    static void handleInput(
        dgm::App& app,
        DependencyContainer& dic,
        const InputSettings& settings,
        CommonHandlerOptions options = {});

    static void swallowAllEvents(dgm::App& app);
};
