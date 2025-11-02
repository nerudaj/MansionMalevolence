#include "gui/builders/TableBuilder.hpp"
#include "gui/Sizers.hpp"
#include "gui/builders/WidgetBuilder.hpp"
#include "types/SemanticTypes.hpp"
#include <cassert>
#include <ranges>

void priv::TableBuilder::addRow(const std::vector<tgui::Widget::Ptr>& cells)
{
    assert(!heading || heading->size() == cells.size());
    assert(rowsOfCells.empty() || rowsOfCells.front().size() == cells.size());
    rowsOfCells.push_back(cells);
}

void priv::TableBuilder::addSeparator()
{
    rowsOfCells.push_back({});
}

template<class Range>
[[nodiscard]] static auto enumerate(Range&& range)
{
    return std::views::zip(
        std::views::iota(size_t { 0 }, std::ranges::size(range)),
        std::forward<Range>(range));
}

[[nodiscard]] static tgui::Container::Ptr
createCell(const tgui::Widget::Ptr& content, size_t column, size_t totalColumns)
{
    const size_t columnWidth = 100 / totalColumns;

    auto cell = tgui::Group::create(
        { tgui::Layout(std::to_string(columnWidth) + "%"), "100%" });
    cell->setPosition(
        { tgui::Layout(std::to_string(column * columnWidth) + "%"), "0%" });
    cell->add(content);

    return cell;
}

static void
addColoredBackground(tgui::Container::Ptr& container, const tgui::Color& color)
{
    auto panel = tgui::Panel::create();
    panel->getRenderer()->setBorders(0);
    panel->getRenderer()->setBackgroundColor(color);
    container->add(panel);
}

tgui::Widget::Ptr priv::TableBuilder::build()
{
    auto&& panel = tgui::GrowVerticalLayout::create();

    assert(heading || !rowsOfCells.empty());

    const auto&& columnCount =
        heading ? heading->size() : rowsOfCells.front().size();

    if (heading)
    {
        auto&& row = WidgetBuilder::createRow(sizer);

        for (auto&& [columnIdx, cellText] : enumerate(heading.value()))
        {
            row->add(createCell(
                WidgetBuilder::createTextLabel(cellText, sizer, "justify"_true),
                columnIdx,
                columnCount));
        }

        panel->add(row);
    }

    auto&& rowIdx = heading ? 1u : 0u;

    for (auto&& row : rowsOfCells)
    {
        auto&& color = rowIdx % 2 == 1 ? tgui::Color(128, 128, 128, 64)
                                       : tgui::Color::Transparent;
        auto&& rowWidget = WidgetBuilder::createRow(sizer);
        addColoredBackground(rowWidget, color);
        rowWidget->setPosition({ "0%", rowWidget->getSize().y * rowIdx++ });

        if (row.empty()) // separator
        {
            rowWidget->add(WidgetBuilder::createSeparator());
        }
        else
        {
            for (auto&& [columnIdx, cellText] : enumerate(row))
            {
                rowWidget->add(createCell(cellText, columnIdx, columnCount));
            }
        }

        panel->add(rowWidget);
    }

    return panel;
}
