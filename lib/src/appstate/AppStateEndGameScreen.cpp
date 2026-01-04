#include "appstate/AppStateEndGameScreen.hpp"
#include "appstate/CommonHandler.hpp"
#include "appstate/Messaging.hpp"
#include "gui/Builders.hpp"

AppStateEndGameScreen::AppStateEndGameScreen(
    dgm::App& app, DependencyContainer& dic, const GameStats& stats) noexcept
    : dgm::AppState(app), dic(dic), stats(stats)
{
    buildLayout();
    dic.jukebox.switchMode(JukeboxMode::Menu);
}

void AppStateEndGameScreen::input()
{
    CommonHandler::handleInput(
        app,
        dic,
        dic.settings.input,
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

    auto content = tgui::VerticalLayout::create();

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

    auto tablePanel = tgui::ScrollablePanel::create();
    tablePanel->add(table.build());
    content->add(tablePanel);

    auto buttonList =
        ButtonListBuilder(dic.strings, dic.sizer)
            .addButton(
                StringId::Retry,
                [this] { app.popState(Messaging::serialize<RestartGame>()); })
            .addButton(
                StringId::BackToMenu,
                [this] { app.popState(Messaging::serialize<PopIfNotMenu>()); })
            .build();

    auto buttonPanel = tgui::Group::create();
    buttonPanel->add(buttonList);

    content->add(buttonPanel);

    dic.gui.rebuildWith(
        DefaultLayoutBuilder(dic.sizer)
            .withBackgroundImage(dic.resmgr.get<sf::Texture>("menubgr.png"))
            .withTitle(
                dic.strings.getString(StringId::YouSurvived), HeadingLevel::H2)
            .withContent(content)
            .withNoCornerButtons()
            .build());
}
