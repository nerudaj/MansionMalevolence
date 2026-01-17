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
    const std::vector<std::tuple<StringId, GameScenario>> levels = {
#ifdef _DEBUG
        { StringId::LevelTutorial1, GameScenario::Tutorial_1 },
#endif
        { StringId::LevelEasy, GameScenario::Easy },
        { StringId::LevelNormal, GameScenario::Normal },
        { StringId::LevelHard, GameScenario::Hard },
        { StringId::LevelNightmare, GameScenario::Nightmare },
        { StringId::LevelNightmare2, GameScenario::OneRoom },
    };

    auto btnListBuilder = ButtonListBuilder(dic.strings, dic.sizer, dic.resmgr);
    for (auto&& [stringId, scenario] : levels)
    {
        const bool isCleared = dic.settings.save.clearedScenarioNames.contains(
            nlohmann::json(scenario).dump());

        btnListBuilder.addButton(
            stringId,
            [this, scenario]
            { app.pushState<AppStatePreGame>(dic, settings, scenario); },
            isCleared);
    }

    dic.gui.rebuildWith(
        DefaultLayoutBuilder(dic.sizer)
            .withBackgroundImage(dic.resmgr.get<sf::Texture>("menubgr.png"))
            .withTitle(
                dic.strings.getString(StringId::LevelSelect), HeadingLevel::H2)
            .withContent(btnListBuilder.build())
            .withNoTopLeftButton()
            .withNoTopRightButton()
            .withBottomLeftButton(WidgetBuilder::createButton(
                dic.strings.getString(StringId::Back),
                [this] { app.popState(); },
                dic.sizer))
            .withNoBottomRightButton()
            .build());
}
