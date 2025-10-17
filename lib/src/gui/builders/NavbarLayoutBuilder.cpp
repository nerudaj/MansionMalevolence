#include "gui/builders/NavbarLayoutBuilder.hpp"
#include "gui/Sizers.hpp"

namespace priv
{
    tgui::Container::Ptr FinalNavbarLayoutBuilder::build()
    {
        const auto containerHeight = sizer.getBaseContainerHeight();
        auto result = tgui::Group::create();

        auto navbarSection = tgui::Group::create({ "100%", containerHeight });
        navbarSection->add(widget);

        auto contentSection = tgui::Group::create(
            { "100%", uni::format("100% - {}", containerHeight).c_str() });
        contentSection->setPosition({ "0%", containerHeight });
        contentSection->add(content, contentPanelId.value_or(""));

        result->add(navbarSection);
        result->add(contentSection);

        return result;
    }
} // namespace priv
