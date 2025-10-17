#pragma once

#include <DGM/dgm.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Tgui.hpp>
#include <memory>
#include <set>
#include <string>

class [[nodiscard]] Gui final
{
public:
    Gui(dgm::Window& window) noexcept : gui(window.getSfmlWindowContext()) {}

public:
    template<class T>
    T::Ptr get(const std::string& id) const
    {
        return gui.get<T>(id);
    }

    void add(const tgui::Widget::Ptr& widget)
    {
        gui.add(widget);
    }

    void add(const tgui::Widget::Ptr& widget, const tgui::String& id)
    {
        gui.add(widget, id);
    }

    void remove(const tgui::Widget::Ptr& widget)
    {
        gui.remove(widget);
    }

    void handleEvent(const sf::Event& event)
    {
        gui.handleEvent(event);
    }

    void draw()
    {
        gui.draw();
    }

    void rebuildWith(const tgui::Widget::Ptr& layout)
    {
        removeAllWidgets();
        add(layout);
    }

    void removeAllWidgets()
    {
        gui.removeAllWidgets();
    }

    void setWindow(sf::RenderWindow& window)
    {
        gui.setWindow(window);
    }

    void setFont(const tgui::Font& font)
    {
        gui.setFont(font);
    }

    void setTheme(tgui::Theme::Ptr theme)
    {
        tgui::Theme::setDefault(theme);
    }

    void emulateClick(const sf::Vector2f& position)
    {
        gui.handleEvent(sf::Event::MouseButtonPressed {
            .button = sf::Mouse::Button::Left,
            .position = sf::Vector2i(position),
        });
        gui.handleEvent(sf::Event::MouseButtonReleased {
            .button = sf::Mouse::Button::Left,
            .position = sf::Vector2i(position),
        });
    }

private:
    tgui::Gui gui;
};
