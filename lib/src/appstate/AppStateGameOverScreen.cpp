#include "appstate/AppStateGameOverScreen.hpp"
#include "appstate/CommonHandler.hpp"
#include "appstate/Messaging.hpp"
#include "gui/Builders.hpp"

AppStateGameOverScreen::AppStateGameOverScreen(
    dgm::App& app, DependencyContainer& dic, GameEndReason endReason) noexcept
    : dgm::AppState(app)
    , dic(dic)
    , endReason(endReason)
    , fadeInAnimation(sf::seconds(1.5f))
{
    buildLayout();
    dic.jukebox.switchMode(JukeboxMode::Menu);
}

void AppStateGameOverScreen::input()
{
    CommonHandler::handleInput(
        app,
        dic,
        dic.settings.input,
        CommonHandlerOptions {
            .disableGoBack = true,
        });
}

void AppStateGameOverScreen::update()
{
    if (fadeInAnimation.elapsed <= fadeInAnimation.duration)
    {
        updateAnimation(fadeInAnimation, app.time);
        deathReasonLabel->getRenderer()->setOpacity(fadeInAnimation.perc);
    }
}

void AppStateGameOverScreen::draw()
{
    dic.gui.draw();
    dic.virtualCursor.draw();
}

void AppStateGameOverScreen::buildLayout()
{
    auto content = tgui::VerticalLayout::create();

    const auto stringId =
        endReason == GameEndReason::InfectionMax   ? StringId::KilledByInfection
        : endReason == GameEndReason::ZombieBite   ? StringId::KilledByZombie
        : endReason == GameEndReason::CerberusBark ? StringId::KilledByCerberus
        : endReason == GameEndReason::CrimsonHeadScreech
            ? StringId::KilledByCrisonHead
        : endReason == GameEndReason::LickerLick ? StringId::KilledByLicker
                                                 : StringId::KilledByTyrant;
    deathReasonLabel = WidgetBuilder::createHeading(
        dic.strings.getString(stringId), dic.sizer, HeadingLevel::H2);
    deathReasonLabel->getRenderer()->setTextColor(tgui::Color(255, 0, 77));
    deathReasonLabel->getRenderer()->setOpacity(0.f);
    auto labelPanel = tgui::Group::create();
    labelPanel->add(deathReasonLabel);
    content->add(labelPanel);

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
                dic.strings.getString(StringId::YouDied), HeadingLevel::H2)
            .withContent(content)
            .withNoCornerButtons()
            .build());
}
