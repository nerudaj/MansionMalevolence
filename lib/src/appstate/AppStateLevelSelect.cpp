#include "appstate/AppStateLevelSelect.hpp"
#include "appstate/AppStatePreGame.hpp"
#include "appstate/CommonHandler.hpp"
#include "appstate/Messaging.hpp"
#include "gui/Builders.hpp"

AppStateLevelSelect::AppStateLevelSelect(
    dgm::App& app, DependencyContainer& dic, AppSettings& settings)
    : dgm::AppState(app), dic(dic), settings(settings)
{
    buildLayout();
}

void AppStateLevelSelect::input()
{
    CommonHandler::handleInput(app, dic, settings.input);
}

void AppStateLevelSelect::update() {}

void AppStateLevelSelect::draw()
{
    dic.gui.draw();
    dic.virtualCursor.draw();
}

void AppStateLevelSelect::restoreFocusImpl(const std::string& msg)
{
    if (auto message = Messaging::deserialize(msg))
    {
        std::visit([&](auto) { app.popState(msg); }, *message);
    }
}

void AppStateLevelSelect::buildLayout()
{
    dic.gui.rebuildWith(
        DefaultLayoutBuilder(dic.sizer)
            .withNoBackgroundImage()
            .withTitle(
                dic.strings.getString(StringId::LevelSelect), HeadingLevel::H2)
            .withContent(
                ButtonListBuilder(dic.strings, dic.sizer)
#ifdef _DEBUG
                    .addButton(
                        StringId::LevelTutorial1,
                        [this]
                        {
                            app.pushState<AppStatePreGame>(
                                dic, settings, GameScenario::Tutorial_1);
                        })
#endif
                    .addButton(
                        StringId::LevelEasy,
                        [this]
                        {
                            app.pushState<AppStatePreGame>(
                                dic, settings, GameScenario::Easy);
                        })
                    .addButton(
                        StringId::LevelNormal,
                        [this]
                        {
                            app.pushState<AppStatePreGame>(
                                dic, settings, GameScenario::Normal);
                        })
                    .addButton(
                        StringId::LevelHard,
                        [this]
                        {
                            app.pushState<AppStatePreGame>(
                                dic, settings, GameScenario::Hard);
                        })
                    .addButton(
                        StringId::LevelNightmare,
                        [this]
                        {
                            app.pushState<AppStatePreGame>(
                                dic, settings, GameScenario::Nightmare);
                        })
                    .build())
            .withNoTopLeftButton()
            .withNoTopRightButton()
            .withBottomLeftButton(WidgetBuilder::createButton(
                dic.strings.getString(StringId::Back),
                [this] { app.popState(); },
                dic.sizer))
            .withNoBottomRightButton()
            .build());
}
