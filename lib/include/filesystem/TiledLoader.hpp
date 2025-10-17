#pragma once

#include "filesystem/models/TiledModels.hpp"

class [[nodiscard]] TiledLoader final
{
public:
    static tiled::FiniteMapModel loadLevel(const std::filesystem::path& path);
};
