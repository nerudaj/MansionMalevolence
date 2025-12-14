#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

struct [[nodiscard]] Playlist final
{
    std::vector<std::string> menu;
    std::vector<std::string> game;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Playlist, menu, game);
