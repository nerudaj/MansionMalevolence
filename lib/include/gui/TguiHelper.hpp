#pragma once

#include "misc/Compatibility.hpp"
#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>

class TguiHelper final
{
public:
    [[nodiscard]] static tgui::Texture
    convertTexture(const sf::Texture& texture)
    {
        auto result = tgui::Texture();
        result.loadFromPixelData(
            texture.getSize(), texture.copyToImage().getPixelsPtr());
        return result;
    }

    [[nodiscard]] static tgui::Texture
    convertTexture(const sf::Texture& texture, const sf::IntRect& clipping)
    {
        auto result = tgui::Texture();
        result.loadFromPixelData(
            texture.getSize(),
            texture.copyToImage().getPixelsPtr(),
            tgui::UIntRect(
                tgui::Vector2u(clipping.position.x, clipping.position.y),
                tgui::Vector2u(clipping.size.x, clipping.size.y)));
        return result;
    }
};
