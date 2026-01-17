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
            dic.strings.getString(id), dic.sizer, "justify"_true);
    };

    auto createIntLabel = [&](int value)
    {
        return WidgetBuilder::createTextLabel(
            std::to_string(value), dic.sizer, "justify"_true);
    };

    auto content = tgui::VerticalLayout::create();

    auto table = TableBuilder(dic.sizer).withNoHeading();
    table.addRow({ createLabel(StringId::TurnsTaken),
                   createIntLabel(stats.turnsTaken) });
    table.addRow({ createLabel(StringId::ShotsFired),
                   createIntLabel(stats.shotsFired) });
    table.addRow({ createLabel(StringId::EnemiesRouted),
                   createIntLabel(stats.enemiesKilled) });
    table.addRow({ createLabel(StringId::DamageTaken),
                   createIntLabel(stats.damageTaken) });

    auto tablePanel = tgui::ScrollablePanel::create();
    tablePanel->add(table.build());
    content->add(tablePanel);

    auto buttonList =
        ButtonListBuilder(dic.strings, dic.sizer, dic.resmgr)
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
