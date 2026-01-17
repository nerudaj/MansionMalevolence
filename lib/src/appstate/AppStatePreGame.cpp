#include "appstate/AppStatePreGame.hpp"
#include "appstate/AppStateGame.hpp"
#include "appstate/Messaging.hpp"
#include "types/Overloads.hpp"

AppStatePreGame::AppStatePreGame(
    dgm::App& app,
    DependencyContainer& dic,
    AppSettings& settings,
    GameScenario scenario)
    : dgm::AppState(app), dic(dic), settings(settings), scenario(scenario)
{
}

void AppStatePreGame::input()
{
    app.pushState<AppStateGame>(dic, settings, scenario);
}

void AppStatePreGame::update() {}

void AppStatePreGame::draw() {}

void AppStatePreGame::restoreFocusImpl(const std::string& msg)
{
    if (auto message = Messaging::deserialize(msg))
    {
        std::visit(
            overloads {
                [&](RestartGame) { /*continue*/ },
                [&](auto) { app.popState(msg); },
            },
            *message);
    }
}
