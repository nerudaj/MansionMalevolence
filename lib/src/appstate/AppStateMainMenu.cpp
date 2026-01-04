#include "appstate/AppStateMainMenu.hpp"
#include "appstate/AppStateLevelSelect.hpp"
#include "appstate/AppStateOptions.hpp"
#include "appstate/AppStateTutorial.hpp"
#include "appstate/CommonHandler.hpp"
#include "gui/Builders.hpp"
#include "misc/CMakeVars.hpp"
#include "strings/StringProvider.hpp"

AppStateMainMenu::AppStateMainMenu(
    dgm::App& app, DependencyContainer& dic, AppSettings& settings) noexcept
    : dgm::AppState(app), dic(dic), settings(settings)
{
    buildLayout();
    dic.jukebox.switchMode(JukeboxMode::Menu);
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
    dic.jukebox.switchMode(JukeboxMode::Menu);
}

void AppStateMainMenu::buildLayout()
{
    dic.gui.rebuildWith(
        DefaultLayoutBuilder(dic.sizer)
            .withBackgroundImage(dic.resmgr.get<sf::Texture>("menubgr.png"))
            .withTexturedTitle(
                dic.resmgr.get<sf::Texture>("zombiecards_logo.png"))
            .withContent(
                ButtonListBuilder(dic.strings, dic.sizer)
                    .addButton(StringId::PlayButton, [&] { onPlay(); })
                    .addButton(StringId::Options, [&] { onOptions(); })
                    .addButton(StringId::HowToPlay, [&] { onHowToPlay(); })
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
    app.pushState<AppStateLevelSelect>(dic, settings);
}

void AppStateMainMenu::onOptions()
{
    app.pushState<AppStateOptions>(dic, settings);
}

void AppStateMainMenu::onHowToPlay()
{
    app.pushState<AppStateTutorial>(dic);
}

void AppStateMainMenu::onExit()
{
    app.exit();
}
