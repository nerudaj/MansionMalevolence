#pragma once

#include "gui/Sizers.hpp"
#include "strings/StringProvider.hpp"
#include <DGM/classes/Compatibility.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <string>
#include <tuple>
#include <vector>

// https://github.com/llvm/llvm-project/issues/36032
struct [[nodiscard]] OptionConfig final
{
    bool disabled = false;
};

class [[nodiscard]] FormBuilder final
{
public:
    FormBuilder(const StringProvider& strings, const Sizer& sizer) noexcept
        : strings(strings), sizer(sizer)
    {
    }

    FormBuilder(const FormBuilder&) = delete;
    FormBuilder(FormBuilder&&) = delete;
    ~FormBuilder() = default;

public:
    FormBuilder& addOption(
        const StringId labelId,
        tgui::Widget::Ptr widget,
        OptionConfig config = {});

    FormBuilder& addOptionWithWidgetId(
        const StringId labelId,
        tgui::Widget::Ptr widget,
        const std::string widgetId);

    FormBuilder& addOptionWithSubmit(
        const StringId labelId,
        tgui::Widget::Ptr widget,
        tgui::Button::Ptr submitBtn);

    FormBuilder& addSeparator();

    [[nodiscard]] tgui::Container::Ptr build();

private:
    [[nodiscard]] tgui::Container::Ptr createOptionRow(
        const std::string& labelText,
        tgui::Widget::Ptr widgetPtr,
        std::optional<std::string> widgetId);

    [[nodiscard]] tgui::Container::Ptr createOptionRowWithSubmitButton(
        const std::string& labelText,
        tgui::Widget::Ptr widgetPtr,
        tgui::Button::Ptr buttonPtr);

private:
    struct [[nodiscard]] RowProps final
    {
        bool separator = false;
        std::string label;
        tgui::Widget::Ptr widget;
        std::optional<std::string> widgetId = {};
        std::optional<tgui::Button::Ptr> submitBtn = {};
    };

    const StringProvider& strings;
    const Sizer& sizer;
    std::vector<RowProps> rowsToBuild;
};
