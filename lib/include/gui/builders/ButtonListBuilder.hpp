#pragma once

#include "gui/Sizers.hpp"
#include "misc/Compatibility.hpp"
#include "strings/StringProvider.hpp"
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <functional>
#include <string>
#include <vector>

class [[nodiscard]] ButtonListBuilder final
{
public:
    ButtonListBuilder(
        const StringProvider& strings, const Sizer& sizer) noexcept
        : strings(strings), sizer(sizer)
    {
    }

    ButtonListBuilder(const ButtonListBuilder&) = delete;
    ButtonListBuilder(ButtonListBuilder&&) = delete;
    ~ButtonListBuilder() = default;

public:
    ButtonListBuilder& addPadding(const std::string& percent);

    ButtonListBuilder& addButton(
        const StringId labelId,
        std::function<void(void)> onClick,
        const std::string& buttonId = "");

    [[nodiscard]] tgui::Container::Ptr build(
        tgui::HorizontalAlignment alignment =
            tgui::HorizontalAlignment::Center);

private:
    struct ButtonProps
    {
        std::string label;
        std::function<void(void)> onClick;
        std::string buttonId;
    };

    struct Padding
    {
        std::string heightPercent;
    };

    const StringProvider& strings;
    const Sizer& sizer;
    std::vector<std::variant<ButtonProps, Padding>> buttonProps;
};
