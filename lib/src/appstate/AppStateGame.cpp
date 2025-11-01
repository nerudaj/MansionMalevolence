#include "appstate/AppStateGame.hpp"
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
            std::optional<sf::Event> e = sf::Event::TouchBegan {
                .finger = 0,
                .position =
                    event->getIf<sf::Event::MouseButtonPressed>()->position,
            };
            dic.touchController.processEvent(e);
        }
        else if (event->is<sf::Event::MouseButtonReleased>())
        {
            std::optional<sf::Event> e = sf::Event::TouchBegan {
                .finger = 0,
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
    renderingEngine.update(app.time);

    gameEvents.processEvents([&](const DummyGameEvent&) {});
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
        std::visit([&](PopIfNotMenu&) { app.popState(msg); }, *message);
    }
}

Scene AppStateGame::buildScene(const dgm::ResourceManager& resmgr)
{
    auto randomCardType = []
    {
        return static_cast<CardType>(
            rand() % std::to_underlying(CardType::Max));
    };

    return Scene { .deck =
                       std::views::iota(0u, 20u)
                       | std::views::transform(
                           [&](size_t) -> CardType { return randomCardType(); })
                       | std::views::transform(CardBuilder::createCard)
                       | uniranges::to<std::list>(),
                   .inventory = {} };
}
