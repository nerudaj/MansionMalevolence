#include "appstate/AppStateMainMenu.hpp"
#include "appstate/AppStateGame.hpp"
#include "appstate/AppStateOptions.hpp"
#include "appstate/CommonHandler.hpp"
#include "gui/Builders.hpp"
#include "misc/CMakeVars.hpp"
#include "strings/StringProvider.hpp"

AppStateMainMenu::AppStateMainMenu(
    dgm::App& app, DependencyContainer& dic, AppSettings& settings) noexcept
    : dgm::AppState(app), dic(dic), settings(settings)
{
    buildLayout();
}

void AppStateMainMenu::input()
{
    CommonHandler::handleInput(
        app,
        dic,
        settings.input,
        CommonHandlerOptions {
            .disableGoBack = true,
        });
}

void AppStateMainMenu::update() {}

void AppStateMainMenu::draw()
{
    dic.gui.draw();
    dic.virtualCursor.draw();
}

void AppStateMainMenu::restoreFocusImpl(const std::string&)
{
    buildLayout();
}

void AppStateMainMenu::buildLayout()
{
    dic.gui.rebuildWith(
        DefaultLayoutBuilder(dic.sizer)
            .withBackgroundImage(
                dic.resmgr.get<sf::Texture>("placeholder-background.png"))
            .withTitle(CMakeVars::TITLE, HeadingLevel::H1)
            .withContent(ButtonListBuilder(dic.strings, dic.sizer)
                             .addButton(StringId::PlayButton, [&] { onPlay(); })
                             .addButton(StringId::Options, [&] { onOptions(); })
                             .addButton(
                                 StringId::ExitButton,
                                 [&] { onExit(); },
                                 "MainMenu_Button_Exit")
                             .build())
            .withNoCornerButtons()
            .build());
}

void AppStateMainMenu::onPlay()
{
    app.pushState<AppStateGame>(dic, settings);
}

void AppStateMainMenu::onOptions()
{
    app.pushState<AppStateOptions>(dic, settings);
}

void AppStateMainMenu::onExit()
{
    app.exit();
}
