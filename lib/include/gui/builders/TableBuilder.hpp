#pragma once

#include "gui/Sizers.hpp"
#include "misc/Compatibility.hpp"
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <optional>
#include <string>
#include <vector>

namespace priv
{
    class [[nodiscard]] TableBuilder final
    {
    public:
        constexpr explicit TableBuilder(const Sizer& sizer) noexcept
            : sizer(sizer)
        {
        }

        constexpr explicit TableBuilder(
            const Sizer& sizer, const std::vector<std::string>& headingCells)
            : sizer(sizer), heading(headingCells)
        {
        }

        void addRow(const std::vector<tgui::Widget::Ptr>& cells);

        void addSeparator();

        [[nodiscard]] tgui::Widget::Ptr build();

    private:
        const Sizer& sizer;
        std::optional<std::vector<std::string>> heading;
        std::vector<std::vector<tgui::Widget::Ptr>> rowsOfCells;
    };
} // namespace priv

class [[nodiscard]] TableBuilder final
{
public:
    constexpr explicit TableBuilder(const Sizer& sizer) noexcept : sizer(sizer)
    {
    }

    TableBuilder(TableBuilder&&) = default;
    TableBuilder(const TableBuilder&) = delete;

    constexpr priv::TableBuilder withNoHeading() const noexcept
    {
        return priv::TableBuilder(sizer);
    }

    constexpr priv::TableBuilder
    withHeading(const std::vector<std::string>& cells)
    {
        return priv::TableBuilder(sizer, cells);
    }

private:
    const Sizer& sizer;
};
