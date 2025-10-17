#include "appstate/CommonHandler.hpp"

void CommonHandler::handleInput(
    dgm::App& app,
    DependencyContainer& dic,
    const InputSettings& settings,
    CommonHandlerOptions options)
{
    dic.virtualCursor.update(app.time, settings.cursorSpeed);

    while (const auto event = app.window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            app.exit();
        else
        {
            dic.gui.handleEvent(*event);
        }
    }

    if (dic.input.isConfirmPressed())
    {
        dic.gui.emulateClick(dic.virtualCursor.getPosition());
    }
    else if (!options.disableGoBack && dic.input.isBackButtonPressed())
    {
        app.popState();
    }
}

void CommonHandler::swallowAllEvents(dgm::App& app)
{
    while (const auto event = app.window.pollEvent())
    {
        if (event->is<sf::Event::Closed>()) app.exit();
    }
}
