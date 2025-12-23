#include "gui/builders/ButtonListBuilder.hpp"
#include "gui/Sizers.hpp"
#include "gui/builders/WidgetBuilder.hpp"
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <ranges>

ButtonListBuilder& ButtonListBuilder::addButton(
    const StringId labelId,
    std::function<void(void)> onClick,
    const std::string& buttonId)
{
    buttonProps.emplace_back(strings.getString(labelId), onClick, buttonId);
    return *this;
}

tgui::Container::Ptr ButtonListBuilder::build(
    tgui::HorizontalAlignment alignment,
    tgui::VerticalAlignment verticalAlignment)
{
    auto&& layout = tgui::GrowVerticalLayout::create();
    layout->setSize({ "50%", "100%" });

    const std::string& horizontalPosition = [&]
    {
        if (alignment == tgui::HorizontalAlignment::Left)
            return "0%";
        else if (alignment == tgui::HorizontalAlignment::Center)
            return "25%";
        else
            return "50%";
    }();
    layout->getRenderer()->setSpaceBetweenWidgets(
        static_cast<float>(sizer.getBaseFontSize()));

    for (auto&& [idx, props] : std::views::enumerate(buttonProps))
    {
        auto group = tgui::Group::create(
            { "100%", sizer.getBaseContainerHeight() * 1.5f });
        auto&& button =
            WidgetBuilder::createButton(props.label, props.onClick, sizer);
        group->add(button, props.buttonId);

        layout->add(group);
    }

    const std::string& verticalPosition = [&]
    {
        if (verticalAlignment == tgui::VerticalAlignment::Top)
            return "0%";
        else if (verticalAlignment == tgui::VerticalAlignment::Center)
            return "parent.height / 2 - height / 2";
        else if (verticalAlignment == tgui::VerticalAlignment::Bottom)
            return "parent.height - height";
    }();

    layout->setPosition(
        { horizontalPosition.c_str(), verticalPosition.c_str() });

    return layout;
}
