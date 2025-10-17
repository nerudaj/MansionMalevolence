#include "gui/builders/FormBuilder.hpp"
#include "gui/Sizers.hpp"
#include "gui/builders/WidgetBuilder.hpp"
#include <ranges>

FormBuilder& FormBuilder::addOption(
    const StringId labelId, tgui::Widget::Ptr widget, OptionConfig config)
{
    widget->setEnabled(!config.disabled);
    rowsToBuild.push_back({
        .label = strings.getString(labelId),
        .widget = widget,
    });
    return *this;
}

FormBuilder& FormBuilder::addOptionWithWidgetId(
    const StringId labelId,
    tgui::Widget::Ptr widget,
    const std::string widgetId)
{
    rowsToBuild.push_back({ .label = strings.getString(labelId),
                            .widget = widget,
                            .widgetId = widgetId });
    return *this;
}

FormBuilder& FormBuilder::addOptionWithSubmit(
    const StringId labelId,
    tgui::Widget::Ptr widget,
    tgui::Button::Ptr submitBtn)
{
    rowsToBuild.push_back({ .label = strings.getString(labelId),
                            .widget = widget,
                            .submitBtn = submitBtn });
    return *this;
}

FormBuilder& FormBuilder::addSeparator()
{
    rowsToBuild.push_back({ .separator = true });
    return *this;
}

tgui::Container::Ptr FormBuilder::build()
{
    auto&& verticalLayout = tgui::GrowVerticalLayout::create();
    verticalLayout->getRenderer()->setPadding({ 10.f, 10.f });

    for (auto&& [idx, props] : std::views::enumerate(rowsToBuild))
    {
        auto&& row =
            props.separator ? WidgetBuilder::createRow(sizer)
            : props.submitBtn
                ? createOptionRowWithSubmitButton(
                      props.label, props.widget, props.submitBtn.value())
                : createOptionRow(props.label, props.widget, props.widgetId);

        if (props.separator)
        {
            row->add(WidgetBuilder::createSeparator());
        }

        verticalLayout->add(row);
    }

    return verticalLayout;
}

tgui::Container::Ptr FormBuilder::createOptionRow(
    const std::string& labelText,
    tgui::Widget::Ptr widgetPtr,
    std::optional<std::string> widgetId)
{
    auto&& row = WidgetBuilder::createRow(sizer);
    row->add(WidgetBuilder::createTextLabel(labelText, sizer));

    auto&& widgetPanel = tgui::Group::create({ "40%", "100%" });
    widgetPanel->setPosition("60%", "0%");
    row->add(widgetPanel);

    widgetId
        .or_else(
            [&]() -> std::optional<std::string>
            {
                widgetPanel->add(widgetPtr);
                return std::nullopt;
            })
        .and_then(
            [&](auto id) -> std::optional<std::string>
            {
                widgetPanel->add(widgetPtr, id);
                return id;
            });

    return row;
}

tgui::Container::Ptr FormBuilder::createOptionRowWithSubmitButton(
    const std::string& labelText,
    tgui::Widget::Ptr widgetPtr,
    tgui::Button::Ptr buttonPtr)
{
    auto&& row = WidgetBuilder::createRow(sizer);
    row->add(WidgetBuilder::createTextLabel(labelText, sizer));

    auto&& widgetContainer = tgui::Group::create({ "25%", "100%" });
    widgetContainer->setPosition("60%", "0%");
    widgetContainer->add(widgetPtr);
    row->add(widgetContainer);

    auto&& btnContainer = tgui::Group::create({ "11%", "90%" });
    btnContainer->setPosition("87%", "5%");
    btnContainer->add(buttonPtr);
    row->add(btnContainer);

    return row;
}
