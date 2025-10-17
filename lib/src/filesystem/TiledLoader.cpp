#include "filesystem/TiledLoader.hpp"
#include <DGM/classes/Utility.hpp>
#include <SFML/System/FileInputStream.hpp>
#include <nlohmann/json.hpp>

tiled::FiniteMapModel TiledLoader::loadLevel(const std::filesystem::path& path)
{
    auto file = dgm::Utility::loadAssetAllText(path);
    if (!file) throw std::runtime_error(file.error().getMessage());

    tiled::FiniteMapModel model = nlohmann::json::parse(file.value());
    return model;
}
