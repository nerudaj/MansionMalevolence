#include <appstate/AppStateInputDetector.hpp>
#include <gui/builders/WidgetBuilder.hpp>

static constexpr const char* PANEL_ID = "InputDetectionMessagePanel";
static constexpr const char* LABEL_ID = "InputDetectionMessageLabel";

AppStateInputDetector::~AppStateInputDetector()
{
    dic.gui.remove(dic.gui.get<tgui::Container>(PANEL_ID));
}

void AppStateInputDetector::input()
{
    while (const auto event = app.window.pollEvent())
    {
        if (event) dic.gui.handleEvent(event.value());
    }
}

void AppStateInputDetector::update()
{
    inputDetector.update();
    dic.virtualCursor.update(app.time, CURSOR_SPEED);
}

void AppStateInputDetector::draw()
{
    dic.gui.draw();
    dic.virtualCursor.draw();
}

void AppStateInputDetector::buildLayout()
{
    auto cancelPanel = tgui::Panel::create();
    cancelPanel->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
    cancelPanel->getRenderer()->setBorders(0);
    cancelPanel->onClick([&] { onCancel(); });
    dic.gui.add(cancelPanel, PANEL_ID);

    auto layout = tgui::Group::create({ "50%", "50%" });
    layout->setPosition({ "25%", "25%" });
    cancelPanel->add(layout);

    auto panel = tgui::Panel::create();
    layout->add(panel);

    auto labelLayout = tgui::Group::create({ "100%", "70%" });
    panel->add(labelLayout);

    auto label = WidgetBuilder::createTextLabel(
        dic.strings.getString(StringId::ReleaseAllInputs),
        dic.sizer,
        "justify"_true);
    panel->add(label, LABEL_ID);
}

void AppStateInputDetector::onInputDetectorReady()
{
    auto label = dic.gui.get<tgui::Label>(LABEL_ID);
    label->setText(dic.strings.getString(StringId::PressAKeyOrEscape));
}

void AppStateInputDetector::onCancel()
{
    dic.input.forceRelease(InputKind::BackButton);
    app.popState();
}
