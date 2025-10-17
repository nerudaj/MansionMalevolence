#include "gui/builders/WidgetBuilder.hpp"
#include "gui/Sizers.hpp"
#include "gui/TguiHelper.hpp"
#include "misc/Compatibility.hpp"
#include "types/SemanticTypes.hpp"
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <ranges>

[[nodiscard]] static std::string randomString(std::size_t len)
{
    constexpr auto CHARS = std::string_view(
        "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    return std::views::iota(0u, len)
           | std::views::transform([CHARS](std::size_t)
                                   { return CHARS[rand() % CHARS.size()]; })
           | uniranges::to<std::string>();
}

tgui::Label::Ptr WidgetBuilder::createLabelInternal(
    const std::string& text,
    const float sizeMultiplier,
    const Sizer& sizer,
    const bool justify)
{
    auto&& label = tgui::Label::create(text);
    label->setVerticalAlignment(tgui::VerticalAlignment::Center);
    label->setHorizontalAlignment(
        justify ? tgui::HorizontalAlignment::Center
                : tgui::HorizontalAlignment::Left);
    label->setTextSize(
        static_cast<unsigned>(sizer.getBaseFontSize() * sizeMultiplier));
    label->setSize({ "100%", "100%" });
    return label;
}

tgui::Container::Ptr WidgetBuilder::createRow(const Sizer& sizer)
{
    return tgui::Group::create({ "100%", sizer.getBaseContainerHeight() });
}

tgui::Button::Ptr WidgetBuilder::createButton(
    const Label& label,
    std::function<void(void)> onClick,
    const Sizer& sizer,
    WidgetOptions options)
{
    auto&& button = tgui::Button::create(label);
    button->onClick(onClick);
    button->setTextSize(sizer.getBaseFontSize());
    button->setSize({ "100%", "100%" });

    applyOptionsToWidget(options, button);

    return button;
}

[[nodiscard]] tgui::Button::Ptr WidgetBuilder::createRowButton(
    const Label& label,
    std::function<void(void)> onClick,
    const Sizer& sizer,
    WidgetOptions options)
{
    auto&& button = createButton(label, onClick, sizer, options);
    button->setSize({ "90%", "90%" });
    button->setPosition({ "5%", "5%" });
    button->getRenderer()->setBackgroundColor(tgui::Color::Transparent);

    return button;
}

tgui::CheckBox::Ptr WidgetBuilder::createCheckbox(
    bool checked, std::function<void(bool)> onChange, WidgetOptions options)
{
    auto&& checkbox = tgui::CheckBox::create();
    auto&& size = checkbox->getSizeLayout();
    checkbox->setPosition(("100%" - size.x) / 2, ("100%" - size.y) / 2);
    checkbox->setChecked(checked);
    checkbox->onChange(onChange);

    applyOptionsToWidget(options, checkbox);

    return checkbox;
}

tgui::Container::Ptr WidgetBuilder::createSlider(
    float value,
    std::function<void(float)> onChange,
    Gui& gui,
    const Sizer& sizer,
    const SliderProperties& properties,
    WidgetOptions options)
{
    auto&& result = tgui::Group::create();
    const auto&& ID = randomString(16);

    auto&& dummyLabel =
        tgui::Label::create(properties.valueFormatter(properties.high));
    dummyLabel->setTextSize(sizer.getBaseFontSize());
    dummyLabel->setAutoSize(true);
    gui.add(dummyLabel, "DummyLabel");
    auto size = dummyLabel->getSizeLayout();

    auto&& valueLabel = createTextLabel(
        properties.valueFormatter(value), sizer, "justify"_true);
    valueLabel->setSize(size.x, "100%");
    valueLabel->setPosition("parent.width" - size.x, "0%");
    result->add(valueLabel, ID);

    auto&& slider = tgui::Slider::create(properties.low, properties.high);
    slider->setPosition("2%", "25%");
    slider->setSize("parent.width - 4% - " - size.x, "50%");
    slider->setStep(properties.step);
    slider->setValue(value);
    slider->onValueChange(
        [&gui,
         ID,
         onChange,
         valueFormatter = std::move(properties.valueFormatter)](float value)
        {
            gui.get<tgui::Label>(ID)->setText(valueFormatter(value));
            onChange(value);
        });

    applyOptionsToWidget(options, slider);

    result->add(slider);

    gui.remove(gui.get<tgui::Label>("DummyLabel"));

    return result;
}

tgui::ComboBox::Ptr WidgetBuilder::createDropdown(
    const std::vector<std::string>& items,
    const std::string& selected,
    std::function<void(std::size_t)> onSelect,
    const Sizer& sizer,
    WidgetOptions options)
{
    auto&& dropdown = tgui::ComboBox::create();
    dropdown->setSize("100%", "80%");
    dropdown->setPosition("0%", "10%");
    dropdown->setTextSize(sizer.getBaseFontSize());
    updateDropdownItems(dropdown, items);
    dropdown->setSelectedItem(selected);
    dropdown->onItemSelect(onSelect);

    applyOptionsToWidget(options, dropdown);

    return dropdown;
}

tgui::EditBox::Ptr WidgetBuilder::createTextInput(
    const std::string& initialValue,
    std::function<void(tgui::String)> onChange,
    const Sizer& sizer,
    const std::string& regexValidator,
    WidgetOptions options)
{
    auto&& box = tgui::EditBox::create();
    box->setSize("100%", "80%");
    box->setPosition("0%", "10%");
    if (!regexValidator.empty()) box->setInputValidator(regexValidator);
    box->setText(initialValue);
    box->setTextSize(sizer.getBaseFontSize());
    box->onTextChange(onChange);

    applyOptionsToWidget(options, box);

    return box;
}

tgui::Tabs::Ptr WidgetBuilder::createTabs(
    const std::vector<Label>& tabLabels,
    std::function<void(const tgui::String&)> onTabChange,
    const Sizer& sizer,
    WidgetOptions options)
{
    auto&& tabs = tgui::Tabs::create();
    tabs->setSize({ "100%", "100%" });
    tabs->setTextSize(sizer.getBaseFontSize());

    for (auto&& label : tabLabels)
    {
        tabs->add(label);
    }

    tabs->onTabSelect(onTabChange);

    applyOptionsToWidget(options, tabs);

    return tabs;
}

tgui::SeparatorLine::Ptr WidgetBuilder::createSeparator()
{
    auto sep = tgui::SeparatorLine::create({ "100%", 1 });
    sep->setPosition({ "0%", "49%" });
    return sep;
}

void WidgetBuilder::updateDropdownItems(
    tgui::ComboBox::Ptr dropdown, const std::vector<std::string>& items)
{
    dropdown->removeAllItems();
    for (auto&& item : items)
    {
        dropdown->addItem(item, item);
    }
    dropdown->setSelectedItem(items.front());
}
