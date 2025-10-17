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

tgui::Container::Ptr
ButtonListBuilder::build(tgui::HorizontalAlignment alignment)
{
    auto&& layout = tgui::GrowVerticalLayout::create();
    layout->setSize({ "50%", "100%" });

    if (alignment == tgui::HorizontalAlignment::Left)
        layout->setPosition({ "0%", "0%" });
    else if (alignment == tgui::HorizontalAlignment::Center)
        layout->setPosition({ "25%", "0%" });
    else
        layout->setPosition({ "50%", "0%" });
    layout->getRenderer()->setSpaceBetweenWidgets(sizer.getBaseFontSize());

    for (auto&& [idx, props] : std::views::enumerate(buttonProps))
    {
        auto group = tgui::Group::create(
            { "100%", sizer.getBaseContainerHeight() * 1.5f });
        auto&& button =
            WidgetBuilder::createButton(props.label, props.onClick, sizer);
        group->add(button, props.buttonId);

        layout->add(group);
    }

    return layout;
}
