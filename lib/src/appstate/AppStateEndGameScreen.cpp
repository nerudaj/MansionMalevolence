#include "appstate/AppStateEndGameScreen.hpp"
#include "appstate/CommonHandler.hpp"
#include "appstate/Messaging.hpp"
#include "gui/Builders.hpp"

AppStateEndGameScreen::AppStateEndGameScreen(
    dgm::App& app,
    DependencyContainer& dic,
    const AppSettings& settings,
    const GameStats& stats,
    bool won) noexcept
    : dgm::AppState(app), dic(dic), settings(settings), stats(stats), won(won)
{
    buildLayout();
}

void AppStateEndGameScreen::input()
{
    CommonHandler::handleInput(
        app,
        dic,
        settings.input,
        CommonHandlerOptions {
            .disableGoBack = true,
        });
}

void AppStateEndGameScreen::update() {}

void AppStateEndGameScreen::draw()
{
    dic.gui.draw();
    dic.virtualCursor.draw();
}

void AppStateEndGameScreen::buildLayout()
{
    auto createLabel = [&](StringId id)
    {
        return WidgetBuilder::createTextLabel(
            dic.strings.getString(id), dic.sizer);
    };

    auto table = TableBuilder(dic.sizer).withNoHeading();
    table.addRow(
        { createLabel(StringId::TurnsTaken),
          WidgetBuilder::createTextLabel(
              std::to_string(stats.turnsTaken), dic.sizer, "justify"_true) });
    table.addRow(
        { createLabel(StringId::ShotsFired),
          WidgetBuilder::createTextLabel(
              std::to_string(stats.shotsFired), dic.sizer, "justify"_true) });
    table.addRow({ createLabel(StringId::EnemiesRouted),
                   WidgetBuilder::createTextLabel(
                       std::to_string(stats.enemiesKilled),
                       dic.sizer,
                       "justify"_true) });

    dic.gui.rebuildWith(
        DefaultLayoutBuilder(dic.sizer)
            .withNoBackgroundImage()
            .withTitle(
                dic.strings.getString(
                    won ? StringId::YouSurvived : StringId::YouDied),
                HeadingLevel::H2)
            .withContent(table.build())
            .withNoTopLeftButton()
            .withNoTopRightButton()
            .withBottomLeftButton(WidgetBuilder::createButton(
                dic.strings.getString(StringId::Retry),
                [this] { app.popState(Messaging::serialize<RestartGame>()); },
                dic.sizer))
            .withBottomRightButton(WidgetBuilder::createButton(
                dic.strings.getString(StringId::BackToMenu),
                [this] { app.popState(Messaging::serialize<PopIfNotMenu>()); },
                dic.sizer))
            .build());
}
