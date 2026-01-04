#include "appstate/AppStateTutorial.hpp"
#include "appstate/CommonHandler.hpp"
#include "gui/Builders.hpp"

AppStateTutorial::AppStateTutorial(dgm::App& app, DependencyContainer& dic)
    : dgm::AppState(app), dic(dic)
{
    buildLayout();
}

void AppStateTutorial::input()
{
    CommonHandler::handleInput(app, dic, dic.settings.input);
}

void AppStateTutorial::update() {}

void AppStateTutorial::draw()
{
    dic.gui.draw();
    dic.virtualCursor.draw();
}

void AppStateTutorial::buildLayout()
{
    dic.gui.rebuildWith(
        DefaultLayoutBuilder(dic.sizer)
            .withNoBackgroundImage()
            .withTitle(
                dic.strings.getString(StringId::HowToPlay), HeadingLevel::H2)
            .withContent(WidgetBuilder::createCarousel(
                images.size(),
                [this](tgui::Container::Ptr content, size_t pageIdx)
                { onPageChanged(content, pageIdx); },
                dic.sizer))
            .withNoTopLeftButton()
            .withNoTopRightButton()
            .withBottomLeftButton(WidgetBuilder::createButton(
                dic.strings.getString(StringId::Back),
                [&] { app.popState(); },
                dic.sizer))
            .withNoBottomRightButton()
            .build());
}

void AppStateTutorial::onPageChanged(
    tgui::Container::Ptr content, size_t pageIdx)
{
    auto label = WidgetBuilder::createTextLabel(
        dic.strings.getString(labelIds[pageIdx]), dic.sizer, "justify"_true);
    label->setSize({ "100%", dic.sizer.getBaseContainerHeight() * 2 });
    label->setPosition({ "0%", "parent.height - height" });
    content->add(label, "DescriptionLabel");

    auto image = tgui::Panel::create();
    image->getRenderer()->setTextureBackground(
        dic.resmgr.get<tgui::Texture>(images[pageIdx]));
    image->setSize(
        { "(parent.height - DescriptionLabel.height) * 921 "
          "/ 1416",
          "parent.height - DescriptionLabel.height" });
    image->setPosition({ "parent.width / 2 - width / 2", "0%" });
    image->getRenderer()->setBorders(1);
    content->add(image, "ImagePanel");
}
