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
#ifdef ANDROID
    layout->setSize({ "90%", "100%" });
#else
    layout->setSize({ "50%", "100%" });
#endif

    const std::string& horizontalPosition = [&]
    {
        if (alignment == tgui::HorizontalAlignment::Center)
#ifdef ANDROID
            return "5%";
#else
            return "25%";
#endif
        else if (alignment == tgui::HorizontalAlignment::Right)
#ifdef ANDROID
            return "10%";
#else
            return "50%";
#endif
        return "0%";
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
        if (verticalAlignment == tgui::VerticalAlignment::Center)
            return "parent.height / 2 - height / 2";
        else if (verticalAlignment == tgui::VerticalAlignment::Bottom)
            return "parent.height - height";
        return "0%";
    }();

    layout->setPosition(
        { horizontalPosition.c_str(), verticalPosition.c_str() });

    return layout;
}
