#pragma once

#include "gui/HeadingLevel.hpp"
#include "gui/Sizers.hpp"
#include "misc/Compatibility.hpp"
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>

namespace priv
{
    struct [[nodiscard]] BuilderProperties
    {
        unsigned baseHeight;
        unsigned cornerButtonDimension;
        unsigned cornerButtonPadding;
        unsigned titleHeight;
    };

    class [[nodiscard]] FinalizedLayoutBuilder final
    {
    public:
        FinalizedLayoutBuilder(
            tgui::Panel::Ptr container, const BuilderProperties& props)
            : container(container), props(props)
        {
        }

    public:
        [[nodiscard]] inline tgui::Panel::Ptr build() const
        {
            return container;
        }

    private:
        tgui::Panel::Ptr container;
        BuilderProperties props;
    };

    class [[nodiscard]] LayoutBuilderWithContentAndThreeButtons final
    {
    public:
        LayoutBuilderWithContentAndThreeButtons(
            tgui::Panel::Ptr container, const BuilderProperties& props)
            : container(container), props(props)
        {
        }

    public:
        FinalizedLayoutBuilder withNoBottomRightButton() const
        {
            return FinalizedLayoutBuilder(container, props);
        }

        FinalizedLayoutBuilder withBottomRightButton(tgui::Button::Ptr button);

    private:
        tgui::Panel::Ptr container;
        BuilderProperties props;
    };

    class [[nodiscard]] LayoutBuilderWithContentAndTwoButtons final
    {
    public:
        LayoutBuilderWithContentAndTwoButtons(
            tgui::Panel::Ptr container, const BuilderProperties& props)
            : container(container), props(props)
        {
        }

    public:
        LayoutBuilderWithContentAndThreeButtons withNoBottomLeftButton() const
        {
            return LayoutBuilderWithContentAndThreeButtons(container, props);
        }

        LayoutBuilderWithContentAndThreeButtons
        withBottomLeftButton(tgui::Button::Ptr button);

    private:
        tgui::Panel::Ptr container;
        BuilderProperties props;
    };

    class [[nodiscard]] LayoutBuilderWithContentAndOneButton final
    {
    public:
        LayoutBuilderWithContentAndOneButton(
            tgui::Panel::Ptr container, const BuilderProperties& props)
            : container(container), props(props)
        {
        }

    public:
        LayoutBuilderWithContentAndTwoButtons withNoTopRightButton() const
        {
            return LayoutBuilderWithContentAndTwoButtons(container, props);
        }

        LayoutBuilderWithContentAndTwoButtons
        withTopRightButton(tgui::Button::Ptr button);

    private:
        tgui::Panel::Ptr container;
        BuilderProperties props;
    };

    class [[nodiscard]] LayoutBuilderWithContent final
    {
    public:
        LayoutBuilderWithContent(
            tgui::Panel::Ptr container, const BuilderProperties& props)
            : container(container), props(props)
        {
        }

    public:
        LayoutBuilderWithContentAndOneButton withNoTopLeftButton() const
        {
            return LayoutBuilderWithContentAndOneButton(container, props);
        }

        LayoutBuilderWithContentAndOneButton
        withTopLeftButton(tgui::Button::Ptr button);

        FinalizedLayoutBuilder withNoCornerButtons()
        {
            return FinalizedLayoutBuilder(container, props);
        }

    private:
        tgui::Panel::Ptr container;
        BuilderProperties props;
    };

    class [[nodiscard]] LayoutBuilderWithBackgroundAndTitle final
    {
    public:
        LayoutBuilderWithBackgroundAndTitle(
            tgui::Panel::Ptr container, const BuilderProperties& props)
            : container(container), props(props)
        {
        }

    public:
        LayoutBuilderWithContent withContent(tgui::Container::Ptr content);

    private:
        tgui::Panel::Ptr container;
        BuilderProperties props;
    };

    class [[nodiscard]] LayoutBuilderWithBackground final
    {
    public:
        LayoutBuilderWithBackground(
            tgui::Panel::Ptr container,
            const BuilderProperties& props,
            const Sizer& sizer)
            : sizer(sizer), container(container), props(props)
        {
        }

    public:
        LayoutBuilderWithBackgroundAndTitle
        withTitle(const std::string& title, HeadingLevel level);

        LayoutBuilderWithBackgroundAndTitle
        withTexturedTitle(const sf::Texture& texture);

    private:
        const Sizer& sizer;
        tgui::Panel::Ptr container;
        BuilderProperties props;
    };
} // namespace priv

class [[nodiscard]] DefaultLayoutBuilder final
{
public:
    constexpr explicit DefaultLayoutBuilder(const Sizer& sizer) noexcept
        : sizer(sizer)
    {
    }

public:
    priv::LayoutBuilderWithBackground
    withBackgroundImage(const sf::Texture& texture);

    priv::LayoutBuilderWithBackground withNoBackgroundImage();

private:
    static priv::BuilderProperties buildProperties(const Sizer& sizer);

private:
    const Sizer& sizer;
};
