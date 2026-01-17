#pragma once

#include "gui/Sizers.hpp"
#include "misc/Compatibility.hpp"
#include "strings/StringProvider.hpp"
#include <DGM/classes/ResourceManager.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <functional>
#include <string>
#include <vector>

class [[nodiscard]] ButtonListBuilder final
{
public:
    ButtonListBuilder(
        const StringProvider& strings,
        const Sizer& sizer,
        const dgm::ResourceManager& resmgr) noexcept
        : strings(strings), sizer(sizer), resmgr(resmgr)
    {
    }

    ButtonListBuilder(const ButtonListBuilder&) = delete;
    ButtonListBuilder(ButtonListBuilder&&) = delete;
    ~ButtonListBuilder() = default;

public:
    ButtonListBuilder& addButton(
        const StringId labelId,
        std::function<void(void)> onClick,
        bool cleared = false,
        const std::string& buttonId = "");

    [[nodiscard]] tgui::Container::Ptr build(
        tgui::HorizontalAlignment alignment = tgui::HorizontalAlignment::Center,
        tgui::VerticalAlignment verticalAlignment =
            tgui::VerticalAlignment::Center);

private:
    struct ButtonProps
    {
        std::string label;
        std::function<void(void)> onClick;
        bool cleared;
        std::string buttonId;
    };

    const StringProvider& strings;
    const Sizer& sizer;
    const dgm::ResourceManager& resmgr;
    std::vector<ButtonProps> buttonProps;
};
