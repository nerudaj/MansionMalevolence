#include "appstate/AppStateOptions.hpp"
#include "appstate/AppStateInputDetector.hpp"
#include "appstate/CommonHandler.hpp"
#include "gui/Builders.hpp"
#include "gui/Sizers.hpp"
#include "misc/Compatibility.hpp"
#include "strings/InputKindToStringMapper.hpp"
#include "types/Overloads.hpp"
#include <filesystem/AppStorage.hpp>
#include <ranges>

const tgui::Color CONTENT_BGCOLOR = tgui::Color(255, 255, 255, 64);
constexpr const char* TABS_ID = "Options_Tabs";

static std::string resolutionToString(const sf::Vector2u& vec)
{
    return uni::format("{}x{}", vec.x, vec.y);
}

static std::vector<std::string> getResolutionStrings()
{
    return sf::VideoMode::getFullscreenModes()
           | std::views::transform([](const sf::VideoMode& mode)
                                   { return mode.size; })
           | std::views::transform(resolutionToString)
           | uniranges::to<std::vector>();
}

static std::string intValueFormatter(float val)
{
    return std::to_string(static_cast<int>(val));
}

AppStateOptions::AppStateOptions(
    dgm::App& app,
    DependencyContainer& dic,
    AppSettings& settings,
    bool invokedFromPause) noexcept
    : dgm::AppState(app)
    , dic(dic)
    , settings(settings)
    , invokedFromPause(invokedFromPause)
{
    buildLayout();
}

void AppStateOptions::input()
{
    CommonHandler::handleInput(app, dic, settings.input);

    auto tabs = dic.gui.get<tgui::Tabs>(TABS_ID);
    if (dic.input.isMenuCycleLeftPressed())
    {
        tabs->select(
            (tabs->getSelectedIndex() + tabs->getTabsCount() - 1)
            % tabs->getTabsCount());
    }
    else if (dic.input.isMenuCycleRightPressed())
    {
        tabs->select((tabs->getSelectedIndex() + 1) % tabs->getTabsCount());
    }
}

void AppStateOptions::update() {}

void AppStateOptions::draw()
{
    dic.gui.draw();
    dic.virtualCursor.draw();
}

void AppStateOptions::buildLayout()
{
    const auto languages = std::vector<std::string> {
        "english",
        "cesky",
        "polski",
    };

    dic.gui.rebuildWith(
        DefaultLayoutBuilder(dic.sizer)
            .withNoBackgroundImage()
            .withTitle(
                dic.strings.getString(StringId::Options), HeadingLevel::H2)
            .withContent(
                FormBuilder(dic.strings, dic.sizer)
                    .addOption(
                        StringId::MusicVolume,
                        WidgetBuilder::createSlider(
                            settings.audio.musicVolume,
                            [&](float val)
                            { settings.audio.musicVolume = val; },
                            dic.gui,
                            dic.sizer,
                            SliderProperties { .valueFormatter =
                                                   intValueFormatter,
                                               .low = 0.f,
                                               .high = 100.f,
                                               .step = 1.f }))
                    .addOption(
                        StringId::SoundVolume,
                        WidgetBuilder::createSlider(
                            settings.audio.soundVolume,
                            [&](float val)
                            { settings.audio.soundVolume = val; },
                            dic.gui,
                            dic.sizer,
                            SliderProperties { .valueFormatter =
                                                   intValueFormatter,
                                               .low = 0.f,
                                               .high = 100.f,
                                               .step = 1.f }))
#ifndef ANDROID
                    .addOption(
                        StringId::SetResolution,
                        WidgetBuilder::createDropdown(
                            getResolutionStrings(),
                            resolutionToString(settings.video.resolution),
                            [this](size_t idx)
                            {
                                onResolutionSelected(
                                    sf::VideoMode::getFullscreenModes()[idx]
                                        .size);
                            },
                            dic.sizer),
                        OptionConfig { .disabled = invokedFromPause })
                    .addOption(
                        StringId::EnableFullscreen,
                        WidgetBuilder::createCheckbox(
                            settings.video.fullscreen,
                            [this](bool val)
                            {
                                settings.video.fullscreen = val;
                                app.window.toggleFullscreen();
                                app.window.getSfmlWindowContext()
                                    .setMouseCursorVisible(false);
                            }))
#endif
                    .addOption(
                        StringId::MenuLanguage,
                        WidgetBuilder::createDropdown(
                            languages,
                            languages.at(std::to_underlying(
                                dic.settings.audio.language)),
                            [&](size_t idx)
                            {
                                const auto lang = static_cast<Language>(idx);
                                dic.settings.audio.language = lang;
                                dic.strings.changeLanguage(lang);
                                refresh();
                            },
                            dic.sizer))
                    .build())
            .withNoTopLeftButton()
            .withNoTopRightButton()
            .withBottomLeftButton(WidgetBuilder::createButton(
                dic.strings.getString(StringId::Back),
                [&]
                {
                    AppStorage::saveFile(
                        "settings.json", fromAppSettingsModel(dic.settings));
                    onBack();
                },
                dic.sizer))
            .withNoBottomRightButton()
            .build());
}

void AppStateOptions::refresh()
{
    buildLayout();
}

void AppStateOptions::onBack()
{
    app.popState();
}

void AppStateOptions::onResolutionSelected(const sf::Vector2u& resolution)
{
    settings.video.resolution = resolution;
    app.window.changeResolution(resolution);
    dic.gui.setWindow(app.window.getSfmlWindowContext());
    dic.touchController.updateFromNewWindowSize(resolution);
    dic.virtualCursor.forceSyncPosition();
    app.window.getSfmlWindowContext().setMouseCursorVisible(false);

    refresh();

    // TODO: Open "Are you sure dialog?"
}
