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
    auto content = tgui::Panel::create();

    auto hbox = tgui::HorizontalLayout::create();
    auto prevButton = WidgetBuilder::createButton(
        dic.strings.getString(StringId::Prev),
        [this]
        {
            page = page == 0 ? images.size() - 1 : page - 1;
            updatePage();
        },
        dic.sizer);
    auto nextButton = WidgetBuilder::createButton(
        dic.strings.getString(StringId::Next),
        [this]
        {
            page = (page + 1) % images.size();
            updatePage();
        },
        dic.sizer);
    hbox->add(prevButton);
    hbox->add(nextButton);
    hbox->setSize({ "90%", dic.sizer.getBaseContainerHeight() });
    hbox->setPosition({ "5%", "parent.height - height" });
    content->add(hbox, "HButtonBox");

    auto label = WidgetBuilder::createTextLabel("", dic.sizer);
    label->setSize({ "90%", dic.sizer.getBaseContainerHeight() * 2 });
    label->setPosition({ "5%", "parent.height - height - HButtonBox.height" });
    content->add(label, "DescriptionLabel");

    auto image = tgui::Panel::create();
    image->setSize(
        { "(parent.height - HButtonBox.height - DescriptionLabel.height) * 921 "
          "/ 1416",
          "parent.height - HButtonBox.height - DescriptionLabel.height" });
    image->setPosition({ "parent.width / 2 - width / 2", "0%" });
    image->getRenderer()->setBorders(1);
    content->add(image, "ImagePanel");

    dic.gui.rebuildWith(
        DefaultLayoutBuilder(dic.sizer)
            .withNoBackgroundImage()
            .withTitle(
                dic.strings.getString(StringId::HowToPlay), HeadingLevel::H2)
            .withContent(content)
            .withNoTopLeftButton()
            .withNoTopRightButton()
            .withBottomLeftButton(WidgetBuilder::createButton(
                dic.strings.getString(StringId::Back),
                [&] { app.popState(); },
                dic.sizer))
            .withNoBottomRightButton()
            .build());

    updatePage();
}

void AppStateTutorial::updatePage()
{
    auto image = dic.gui.get<tgui::Panel>("ImagePanel");
    image->getRenderer()->setTextureBackground(
        dic.resmgr.get<tgui::Texture>(images[page]));

    auto label = dic.gui.get<tgui::Label>("DescriptionLabel");
    label->setText(dic.strings.getString(labelIds[page]));
}
