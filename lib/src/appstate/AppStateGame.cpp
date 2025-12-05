#include "appstate/AppStateGame.hpp"
#include "appstate/AppStateEndGameScreen.hpp"
#include "appstate/AppStatePause.hpp"
#include "appstate/Messaging.hpp"

void AppStateGame::input()
{
    if (dic.input.isBackButtonPressed())
    {
        app.pushState<AppStatePause>(dic, settings);
    }

    dic.virtualCursor.update(app.time, settings.input.cursorSpeed);

    while (const auto event = app.window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            app.exit();
        }
        else if (event->is<sf::Event::MouseButtonPressed>())
        {
            const std::optional<sf::Event> e = sf::Event::TouchBegan {
                .finger = 0,
                .position =
                    event->getIf<sf::Event::MouseButtonPressed>()->position,
            };
            dic.touchController.processEvent(e);
        }
        else if (event->is<sf::Event::MouseButtonReleased>())
        {
            const std::optional<sf::Event> e = sf::Event::TouchBegan {
                .finger = 0,
            };
            dic.touchController.processEvent(e);
        }
        else if (
            event->is<sf::Event::MouseMoved>()
            && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            const std::optional<sf::Event> e = sf::Event::TouchMoved {
                .finger = 0,
                .position = event->getIf<sf::Event::MouseMoved>()->position,
            };
            dic.touchController.processEvent(e);
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
    assert(!scene.deck.empty());
    renderingEngine.update(app.time);

    gameEvents.processEvents(gameRulesEngine);

    if (gameRulesEngine.gameEnded())
        app.pushState<AppStateEndGameScreen>(
            dic, settings, scene.stats, gameRulesEngine.gameWon());
}

void AppStateGame::draw()
{
    renderingEngine.draw(app.window);
    dic.virtualCursor.draw();
}

void AppStateGame::restoreFocusImpl(const std::string& msg)
{
    if (auto message = Messaging::deserialize(msg))
    {
        std::visit([&](auto) { app.popState(msg); }, *message);
    }
}
