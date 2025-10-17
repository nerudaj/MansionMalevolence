#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <nlohmann/json.hpp>

struct [[nodiscard]] VideoSettings final
{
    sf::Vector2u resolution =
#ifdef ANDROID
        sf::VideoMode::getDesktopMode().size;
#elif _DEBUG
        sf::Vector2u(1280, 720);
#else
        sf::VideoMode::getDesktopMode().size;
#endif
    bool fullscreen =
#ifdef _DEBUG
        false;
#else
        true;
#endif
    float uiScale = 1.f;
};

namespace sf
{
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Vector2u, x, y);
}

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
    VideoSettings, resolution, fullscreen, uiScale);
