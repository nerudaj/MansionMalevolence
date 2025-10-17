#include "appstate/AppStateOptions.hpp"
#include "appstate/AppStateInputDetector.hpp"
#include "appstate/CommonHandler.hpp"
#include "gui/Builders.hpp"
#include "gui/Sizers.hpp"
#include "misc/Compatibility.hpp"
#include "strings/InputKindToStringMapper.hpp"
#include "types/Overloads.hpp"
#include <ranges>

const tgui::Color CONTENT_BGCOLOR = tgui::Color(255, 255, 255, 64);
constexpr const char* TABS_ID = "Options_Tabs";

std::string resolutionToString(const sf::Vector2u& vec)
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
    dgm::App& app, DependencyContainer& dic, AppSettings& settings) noexcept
    : dgm::AppState(app), dic(dic), settings(settings)
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
    dic.gui.rebuildWith(
        DefaultLayoutBuilder(dic.sizer)
            .withNoBackgroundImage()
            .withTitle(
                dic.strings.getString(StringId::Options), HeadingLevel::H1)
            .withContent(TabbedLayoutBuilder(dic.strings, dic.sizer)
                             .addTab(
                                 StringId::VideoOptionsTab,
                                 [&](tgui::Container::Ptr content)
                                 { onVideoTabSelected(content); })
                             .addTab(
                                 StringId::AudioOptionsTab,
                                 [&](tgui::Container::Ptr content)
                                 { onAudioTabSelected(content); })
                             .addTab(
                                 StringId::InputOptionsTab,
                                 [&](tgui::Container::Ptr content)
                                 { onInputTabSelected(content); })
                             .addTab(
                                 StringId::BindingsOptionsTab,
                                 [&](tgui::Container::Ptr content)
                                 { onBindingsTabSelected(content); })
                             .setTabSelected(StringId::VideoOptionsTab)
                             .build(TabbedLayoutOptions {
                                 .tabsWidgetId = TABS_ID,
                                 .contentIsScrollable = true,
                             }))
            .withNoTopLeftButton()
            .withNoTopRightButton()
            .withBottomLeftButton(WidgetBuilder::createButton(
                dic.strings.getString(StringId::Back),
                [&] { onBack(); },
                dic.sizer))
            .withNoBottomRightButton()
            .build());
}

void AppStateOptions::refresh()
{
    buildLayout();
}

void AppStateOptions::onVideoTabSelected(tgui::Container::Ptr content)
{
    content->removeAllWidgets();
    content->add(
        FormBuilder(dic.strings, dic.sizer)
#ifndef ANDROID
            .addOption(
                StringId::EnableFullscreen,
                WidgetBuilder::createCheckbox(
                    settings.video.fullscreen,
                    [&](bool val)
                    {
                        settings.video.fullscreen = val;
                        app.window.toggleFullscreen();
                    }))
            .addOption(
                StringId::SetResolution,
                WidgetBuilder::createDropdown(
                    getResolutionStrings(),
                    resolutionToString(settings.video.resolution),
                    [&](size_t idx)
                    {
                        onResolutionSelected(
                            sf::VideoMode::getFullscreenModes()[idx].size);
                    },
                    dic.sizer))
#endif
            .addOptionWithSubmit(
                StringId::SetUiScale,
                WidgetBuilder::createSlider(
                    settings.video.uiScale,
                    [&](float val) { settings.video.uiScale = val; },
                    dic.gui,
                    dic.sizer,
                    SliderProperties {
                        .valueFormatter = [](float val)
                        { return uni::format("{:.1f}", val); },
                        .low = 1.f,
                        .high = 2.f,
                        .step = 0.1f,
                    }),
                WidgetBuilder::createButton(
                    dic.strings.getString(StringId::Apply),
                    [&] { refresh(); },
                    dic.sizer))
#ifdef _DEBUG
            .addOption(
                StringId::SetTheme,
                WidgetBuilder::createDropdown(
                    dic.resmgr.getLoadedResourceIds<tgui::Theme::Ptr>().value(),
                    "",
                    [&](size_t idx)
                    {
                        dic.gui.setTheme(dic.resmgr.get<tgui::Theme::Ptr>(
                            dic.resmgr.getLoadedResourceIds<tgui::Theme::Ptr>()
                                .value()[idx]));
                        refresh();
                    },
                    dic.sizer))
#endif
            .build());
}

void AppStateOptions::onAudioTabSelected(tgui::Container::Ptr content)
{
    content->removeAllWidgets();
    content->add(
        FormBuilder(dic.strings, dic.sizer)
            .addOption(
                StringId::MusicVolume,
                WidgetBuilder::createSlider(
                    settings.audio.musicVolume,
                    [&](float val) { settings.audio.musicVolume = val; },
                    dic.gui,
                    dic.sizer,
                    SliderProperties { .valueFormatter = intValueFormatter,
                                       .low = 0.f,
                                       .high = 100.f,
                                       .step = 1.f }))
            .addOption(
                StringId::SoundVolume,
                WidgetBuilder::createSlider(
                    settings.audio.soundVolume,
                    [&](float val) { settings.audio.soundVolume = val; },
                    dic.gui,
                    dic.sizer,
                    SliderProperties { .valueFormatter = intValueFormatter,
                                       .low = 0.f,
                                       .high = 100.f,
                                       .step = 1.f }))
            .build());
}

void AppStateOptions::onInputTabSelected(tgui::Container::Ptr content)
{
    content->removeAllWidgets();
    content->add(
        FormBuilder(dic.strings, dic.sizer)
            .addOption(
                StringId::GamepadDeadzone,
                WidgetBuilder::createSlider(
                    settings.input.gamepadDeadzone,
                    [&](float val) { settings.input.gamepadDeadzone = val; },
                    dic.gui,
                    dic.sizer,
                    SliderProperties { .valueFormatter = intValueFormatter,
                                       .low = 0.f,
                                       .high = 100.f,
                                       .step = 1.f }))
            .addOption(
                StringId::CursorSpeed,
                WidgetBuilder::createSlider(
                    settings.input.cursorSpeed,
                    [&](float val) { settings.input.cursorSpeed = val; },
                    dic.gui,
                    dic.sizer,
                    SliderProperties { .valueFormatter = intValueFormatter,
                                       .low = 100.f,
                                       .high = 1000.f,
                                       .step = 10.f }))
#if defined(_DEBUG) or defined(ANDROID)
            .addOption(
                StringId::ShowTouchControls,
                WidgetBuilder::createCheckbox(
                    settings.input.showTouchControls,
                    [&](bool val) { settings.input.showTouchControls = val; }))
#endif
            .build());
}

template<class V, class T>
bool doesVariantContain(const V& v, T t)
{
    if (std::holds_alternative<T>(v)) return std::get<T>(v) == t;
    return false;
}

void AppStateOptions::onBindingsTabSelected(tgui::Container::Ptr content)
{
    auto&& inputKindMapper = InputKindToStringMapper(dic.strings);
    auto&& hwInputMapper = HwInputToStringMapper();

    auto tableBuilder = TableBuilder(dic.sizer).withHeading({
        dic.strings.getString(StringId::BindingHeadingAction),
        dic.strings.getString(StringId::BindingHeadingKMB),
        dic.strings.getString(StringId::BindingsHeadingGamepad),
    });

    auto buttonOrNothing = [&](tgui::Button::Ptr ptr,
                               bool useNothing) -> tgui::Widget::Ptr
    {
        if (useNothing) return WidgetBuilder::createTextLabel("", dic.sizer);
        return ptr;
    };

    auto addSectionToTable = [&](std::map<InputKind, Binding>& bindings)
    {
        for (auto&& [action, binding] : bindings)
        {
            auto&& [kmbBinding, gamepadBinding] = binding;

            const bool isEscapeKey =
                doesVariantContain(kmbBinding, sf::Keyboard::Key::Escape);
            const bool noKmb =
                doesVariantContain(kmbBinding, std::monostate {});
            const bool noGmp =
                doesVariantContain(gamepadBinding, std::monostate {});

            auto label = WidgetBuilder::createTextLabel(
                inputKindMapper.inputKindToString(action), dic.sizer);
            label->getRenderer()->setPadding({ "5%", "0%", "0%", "0%" });

            tableBuilder.addRow({
                label,
                buttonOrNothing(
                    WidgetBuilder::createRowButton(
                        std::visit(hwInputMapper, kmbBinding),
                        [&]
                        {
                            std::function<void(KmbBinding)> callback =
                                [&](KmbBinding b)
                            { onInputDetected(action, b, bindings); };
                            app.pushState<AppStateInputDetector>(
                                dic, settings.input, std::move(callback));
                        },
                        dic.sizer,
                        WidgetOptions { .id =
                                            getBindButtonId<KmbBinding>(action),
                                        .enabled = !isEscapeKey }),
                    noKmb),
                buttonOrNothing(
                    WidgetBuilder::createRowButton(
                        std::visit(hwInputMapper, gamepadBinding),
                        [&]
                        {
                            std::function<void(GamepadBinding)> callback =
                                [&](GamepadBinding b)
                            { onInputDetected(action, b, bindings); };
                            app.pushState<AppStateInputDetector>(
                                dic, settings.input, std::move(callback));
                        },
                        dic.sizer,
                        WidgetOptions {
                            .id = getBindButtonId<GamepadBinding>(action),
                        }),
                    noGmp),
            });
        }
    };

    addSectionToTable(settings.bindings.ingameBindings);
    tableBuilder.addSeparator();
    addSectionToTable(settings.bindings.menuBindings);

    content->removeAllWidgets();
    content->add(tableBuilder.build());
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

    refresh();

    // TODO: Open "Are you sure dialog?"
}
