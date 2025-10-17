#include "appstate/AppStateGame.hpp"
#include "appstate/AppStatePause.hpp"
#include "appstate/Messaging.hpp"

void AppStateGame::input()
{
    if (dic.input.isBackButtonPressed())
    {
        app.pushState<AppStatePause>(dic, settings);
    }

    while (const auto event = app.window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            app.exit();
        }
        else
        {
            dic.touchController.processEvent(event);
        }
    }
}

void AppStateGame::update()
{
    gameRulesEngine.update(app.time);
    renderingEngine.update(app.time);

    gameEvents.processEvents(
        [&](const DummyGameEvent& e)
        {
            sound.setBuffer(dic.resmgr.get<sf::SoundBuffer>(e.soundName));
            sound.play();
        });
}

void AppStateGame::draw()
{
    renderingEngine.draw(app.window);
}

void AppStateGame::restoreFocusImpl(const std::string& msg)
{
    if (auto message = Messaging::deserialize(msg))
    {
        std::visit([&](PopIfNotMenu&) { app.popState(msg); }, *message);
    }
}

Scene AppStateGame::buildScene(const dgm::ResourceManager& resmgr)
{
    return Scene { .deck = { CARD_DEFS.at(CardType::Pistol) },
                   .inventory = { CARD_DEFS.at(CardType::Shotgun),
                                  CARD_DEFS.at(CardType::Shotgun),
                                  CARD_DEFS.at(CardType::Shotgun) } };
}
