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
    dic.jukebox.switchMode(JukeboxMode::Menu);
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

    auto content = tgui::VerticalLayout::create();
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

    buttonList->setPosition({ "25%", "parent.height / 2 - height / 2" });

    auto buttonPanel = tgui::Panel::create();
    buttonPanel->add(buttonList);

    content->add(buttonPanel);

    dic.gui.rebuildWith(
        DefaultLayoutBuilder(dic.sizer)
            .withNoBackgroundImage()
            .withTitle(
                dic.strings.getString(
                    won ? StringId::YouSurvived : StringId::YouDied),
                HeadingLevel::H2)
            .withContent(content)
            .withNoCornerButtons()
            .build());
}
