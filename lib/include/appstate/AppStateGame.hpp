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
        : dgm::AppState(
              app, dgm::AppStateConfig { .clearColor = sf::Color(17, 29, 53) })
        , dic(dic)
        , settings(settings)
        , scene(SceneBuilder::createScene(scenario))
        , scenario(scenario)
        , gameRulesEngine(
              gameEvents, dic.audioEngine, scene, dic.input, settings.video)
        , renderingEngine(
              app.window, dic.resmgr, scene, settings, dic.touchController)
    {
        dic.jukebox.switchMode(JukeboxMode::Game);
    }

public:
    void input() override;

    void update() override;

    void draw() override;

    void onGameEnded();

private:
    void restoreFocusImpl(const std::string& msg) override;

private:
    DependencyContainer& dic;
    AppSettings& settings;
    Scene scene;
    GameScenario scenario;
    EventQueue<GameEvent> gameEvents;
    GameRulesEngine gameRulesEngine;
    RenderingEngine renderingEngine;
};
