#include "appstate/AppStateEndGameScreen.hpp"
#include "appstate/CommonHandler.hpp"
#include "appstate/Messaging.hpp"
#include "gui/Builders.hpp"

AppStateEndGameScreen::AppStateEndGameScreen(
    dgm::App& app,
    DependencyContainer& dic,
    const AppSettings& settings,
    const GameStats& stats,
    GameEndReason endReason) noexcept
    : dgm::AppState(app)
    , dic(dic)
    , settings(settings)
    , stats(stats)
    , endReason(endReason)
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

    auto content = tgui::VerticalLayout::create();

    if (endReason == GameEndReason::Won)
    {
        auto table = TableBuilder(dic.sizer).withNoHeading();
        table.addRow({ createLabel(StringId::TurnsTaken),
                       WidgetBuilder::createTextLabel(
                           std::to_string(stats.turnsTaken),
                           dic.sizer,
                           "justify"_true) });
        table.addRow({ createLabel(StringId::ShotsFired),
                       WidgetBuilder::createTextLabel(
                           std::to_string(stats.shotsFired),
                           dic.sizer,
                           "justify"_true) });
        table.addRow({ createLabel(StringId::EnemiesRouted),
                       WidgetBuilder::createTextLabel(
                           std::to_string(stats.enemiesKilled),
                           dic.sizer,
                           "justify"_true) });

        auto tablePanel = tgui::ScrollablePanel::create();
        tablePanel->add(table.build());
        content->add(tablePanel);
    }
    else
    {
        const auto stringId =
            endReason == GameEndReason::InfectionMax
                ? StringId::KilledByInfection
            : endReason == GameEndReason::ZombieBite ? StringId::KilledByZombie
            : endReason == GameEndReason::CerberusBark
                ? StringId::KilledByCerberus
            : endReason == GameEndReason::CrimsonHeadScreech
                ? StringId::KilledByCrisonHead
            : endReason == GameEndReason::LickerLick ? StringId::KilledByLicker
                                                     : StringId::KilledByTyrant;
        auto label = WidgetBuilder::createTextLabel(
            dic.strings.getString(stringId), dic.sizer, "justify"_true);
        label->getRenderer()->setTextColor(tgui::Color(255, 0, 77));
        auto labelPanel = tgui::Panel::create();
        labelPanel->add(label);
        content->add(labelPanel);
    }

    auto buttonList =
        ButtonListBuilder(dic.strings, dic.sizer)
            .addButton(
                StringId::Retry,
                [this] { app.popState(Messaging::serialize<RestartGame>()); })
            .addButton(
                StringId::BackToMenu,
                [this] { app.popState(Messaging::serialize<PopIfNotMenu>()); })
            .build();

    auto buttonPanel = tgui::Panel::create();
    buttonPanel->add(buttonList);

    content->add(buttonPanel);

    dic.gui.rebuildWith(
        DefaultLayoutBuilder(dic.sizer)
            .withNoBackgroundImage()
            .withTitle(
                dic.strings.getString(
                    endReason == GameEndReason::Won ? StringId::YouSurvived
                                                    : StringId::YouDied),
                HeadingLevel::H2)
            .withContent(content)
            .withNoCornerButtons()
            .build());
}
