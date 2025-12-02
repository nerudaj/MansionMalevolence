#pragma once

#include "game/builders/SceneBuilder.hpp"
#include "game/definitions/Scene.hpp"
#include "game/engine/GameRulesEngine.hpp"
#include "game/engine/RenderingEngine.hpp"
#include "game/enums/GameScenario.hpp"
#include "game/events/EventQueue.hpp"
#include "misc/DependencyContainer.hpp"
#include "settings/AppSettings.hpp"
#include <DGM/dgm.hpp>
#include <SFML/Audio.hpp>
#include <vector>

class [[nodiscard]] AppStateGame : public dgm::AppState
{
public:
    AppStateGame(
        dgm::App& app,
        DependencyContainer& dic,
        AppSettings& settings,
        GameScenario scenario)
        : dgm::AppState(app)
        , dic(dic)
        , settings(settings)
        , scene(SceneBuilder::createScene(scenario))
        , audioEngine(dic.resmgr)
        , gameRulesEngine(
              gameEvents, audioEngine, scene, dic.input, settings.video)
        , renderingEngine(dic.resmgr, scene, settings, dic.touchController)
    {
    }

public:
    void input() override;

    void update() override;

    void draw() override;

private:
    void restoreFocusImpl(const std::string& msg) override;

private:
    DependencyContainer& dic;
    AppSettings& settings;
    Scene scene;
    EventQueue<GameEvent> gameEvents;
    AudioEngine audioEngine;
    GameRulesEngine gameRulesEngine;
    RenderingEngine renderingEngine;
};
