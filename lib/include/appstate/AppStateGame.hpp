#pragma once

#include "game/Scene.hpp"
#include "game/engine/GameRulesEngine.hpp"
#include "game/engine/RenderingEngine.hpp"
#include "game/events/EventQueue.hpp"
#include "misc/DependencyContainer.hpp"
#include "settings/AppSettings.hpp"
#include <DGM/dgm.hpp>
#include <SFML/Audio.hpp>
#include <vector>

class [[nodiscard]] AppStateGame : public dgm::AppState
{
public:
    AppStateGame(dgm::App& app, DependencyContainer& dic, AppSettings& settings)
        : dgm::AppState(app)
        , dic(dic)
        , settings(settings)
        , scene(buildScene(dic.resmgr))
        , gameRulesEngine(gameEvents, scene, dic.input)
        , renderingEngine(dic.resmgr, scene, settings, dic.touchController)
        , sound(dic.resmgr.get<sf::SoundBuffer>("land.wav"))
    {
        sound.setVolume(100.f);
    }

public:
    void input() override;

    void update() override;

    void draw() override;

private:
    void restoreFocusImpl(const std::string& msg) override;

    static Scene buildScene(const dgm::ResourceManager& resmgr);

private:
    DependencyContainer& dic;
    AppSettings& settings;
    Scene scene;
    EventQueue<GameEvent> gameEvents;
    GameRulesEngine gameRulesEngine;
    RenderingEngine renderingEngine;
    sf::Sound sound;
};
