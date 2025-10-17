#include "gui/builders/DefaultLayoutBuilder.hpp"
#include "gui/TguiHelper.hpp"
#include "gui/builders/WidgetBuilder.hpp"
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>

namespace priv
{
    static tgui::Container::Ptr containButton(
        tgui::Button::Ptr button,
        const BuilderProperties& props,
        tgui::HorizontalAlignment align,
        tgui::VerticalAlignment valign)
    {
        assert(
            align != tgui::HorizontalAlignment::Center
            && valign != tgui::VerticalAlignment::Center);

        const auto isTexturedButton = button->getText().empty();

        auto&& panel = tgui::Group::create(
            isTexturedButton
                ? tgui::Layout2d { props.cornerButtonDimension,
                                   props.cornerButtonDimension }
                : tgui::Layout2d { props.cornerButtonDimension * 2,
                                   props.cornerButtonDimension * 0.6f });
        panel->setPosition({
            (align == tgui::HorizontalAlignment::Left
                 ? uni::format("{}", props.cornerButtonPadding)
                 : uni::format(
                       "parent.width - width - {}", props.cornerButtonPadding))
                .c_str(),
            (valign == tgui::VerticalAlignment::Top
                 ? uni::format("{}", props.cornerButtonPadding)
                 : uni::format(
                       "parent.height - height - {}",
                       props.cornerButtonPadding))
                .c_str(),
        });
        panel->add(button);
        return panel;
    }

    FinalizedLayoutBuilder
    LayoutBuilderWithContentAndThreeButtons::withBottomRightButton(
        tgui::Button::Ptr button)
    {
        container->add(containButton(
            button,
            props,
            tgui::HorizontalAlignment::Right,
            tgui::VerticalAlignment::Bottom));
        return FinalizedLayoutBuilder(container, props);
    }

    LayoutBuilderWithContentAndThreeButtons
    LayoutBuilderWithContentAndTwoButtons::withBottomLeftButton(
        tgui::Button::Ptr button)
    {
        container->add(containButton(
            button,
            props,
            tgui::HorizontalAlignment::Left,
            tgui::VerticalAlignment::Bottom));
        return LayoutBuilderWithContentAndThreeButtons(container, props);
    }

    LayoutBuilderWithContentAndTwoButtons
    LayoutBuilderWithContentAndOneButton::withTopRightButton(
        tgui::Button::Ptr button)
    {
        container->add(containButton(
            button,
            props,
            tgui::HorizontalAlignment::Right,
            tgui::VerticalAlignment::Top));
        return LayoutBuilderWithContentAndTwoButtons(container, props);
    }

    LayoutBuilderWithContentAndOneButton
    LayoutBuilderWithContent::withTopLeftButton(tgui::Button::Ptr button)
    {
        container->add(containButton(
            button,
            props,
            tgui::HorizontalAlignment::Left,
            tgui::VerticalAlignment::Top));
        return LayoutBuilderWithContentAndOneButton(container, props);
    }

    LayoutBuilderWithContent LayoutBuilderWithBackgroundAndTitle::withContent(
        tgui::Container::Ptr content)
    {
        auto&& contentPanel = tgui::Group::create({
            "70%",
            uni::format(
                "100% - {} - {} - {}",
                props.titleHeight,
                props.cornerButtonDimension,
                props.cornerButtonPadding)
                .c_str(),
        });
        contentPanel->setPosition(
            { "parent.width / 2 - width / 2", props.titleHeight });
        contentPanel->add(content, "DefaultLayoutContentPanel");
        container->add(contentPanel);
        return LayoutBuilderWithContent(container, props);
    }

    LayoutBuilderWithBackgroundAndTitle LayoutBuilderWithBackground::withTitle(
        const std::string& title, HeadingLevel level)
    {
        auto&& panel = tgui::Group::create({ "100%", props.titleHeight });
        panel->add(WidgetBuilder::createHeading(title, sizer, level));
        container->add(panel);
        return LayoutBuilderWithBackgroundAndTitle(container, props);
    }

    LayoutBuilderWithBackgroundAndTitle
    LayoutBuilderWithBackground::withTexturedTitle(const sf::Texture& texture)
    {
        tgui::Texture ttexture(texture);

        auto&& panelContainer = tgui::Group::create(
            { texture.getSize().x * props.titleHeight / texture.getSize().y,
              props.titleHeight });
        panelContainer->setPosition({ "parent.width / 2 - width / 2", "0%" });

        auto panel = tgui::Panel::create();
        panel->getRenderer()->setTextureBackground(ttexture);

        container->add(panel);

        return LayoutBuilderWithBackgroundAndTitle(container, props);
    }
} // namespace priv

priv::LayoutBuilderWithBackground
DefaultLayoutBuilder::withBackgroundImage(const sf::Texture& texture)
{
    auto&& bgr = tgui::Panel::create();
    bgr->getRenderer()->setTextureBackground(
        TguiHelper::convertTexture(texture));
    return priv::LayoutBuilderWithBackground(
        bgr, buildProperties(sizer), sizer);
}

priv::LayoutBuilderWithBackground DefaultLayoutBuilder::withNoBackgroundImage()
{
    return priv::LayoutBuilderWithBackground(
        tgui::Panel::create(), buildProperties(sizer), sizer);
}

priv::BuilderProperties
DefaultLayoutBuilder::buildProperties(const Sizer& sizer)
{
    const auto baseHeight = sizer.getBaseContainerHeight();

    return priv::BuilderProperties {
        .baseHeight = baseHeight,
        .cornerButtonDimension = baseHeight * 2,
        .cornerButtonPadding = baseHeight / 2,
        .titleHeight = 3 * baseHeight,
    };
}
