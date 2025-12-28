#include "filesystem/ResourceLoader.hpp"
#include "filesystem/AppStorage.hpp"
#include "filesystem/TiledLoader.hpp"
#include "misc/Compatibility.hpp"
#include "misc/Playlist.hpp"
#include <SFML/Audio.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Tgui.hpp>
#include <expected>

static std::expected<tgui::Font, dgm::Error>
loadTguiFont(const std::filesystem::path& path)
{
    try
    {
        return tgui::Font(path.string());
    }
    catch (const std::exception& ex)
    {
        return std::unexpected(dgm::Error(ex.what()));
    }
}

static std::expected<tgui::Theme::Ptr, dgm::Error>
loadTguiTheme(const std::filesystem::path& path)
{
    try
    {
        return tgui::Theme::create(path.string());
    }
    catch (const std::exception& ex)
    {
        return std::unexpected(dgm::Error(ex.what()));
    }
}

static inline std::expected<tiled::FiniteMapModel, dgm::Error>
loadTiledMap(const std::filesystem::path& path)
{
    try
    {
        return TiledLoader::loadLevel(path);
    }
    catch (const std::exception& ex)
    {
        return std::unexpected { dgm::Error(ex.what()) };
    }
}

static std::expected<sf::Music, dgm::Error>
loadSong(const std::filesystem::path& path)
{
    try
    {
        auto music = sf::Music(path);
        return music;
    }
    catch (const std::exception& ex)
    {
        return std::unexpected { dgm::Error(ex.what()) };
    }
}

static std::expected<Playlist, dgm::Error>
loadPlaylist(const std::filesystem::path& path)
{
    try
    {
        const auto assetText = dgm::Utility::loadAssetAllText(path);
        if (!assetText) return std::unexpected { assetText.error() };
        const auto json = nlohmann::json::parse(assetText.value());
        const Playlist playlist = json;
        return playlist;
    }
    catch (const std::exception& ex)
    {
        return std::unexpected { dgm::Error(ex.what()) };
    }
}

dgm::ResourceManager
ResourceLoader::loadResources(const std::filesystem::path& assetDir)
{
    dgm::ResourceManager resmgr;

    if (auto result = resmgr.loadResourcesFromDirectory<sf::Font>(
            assetDir / "fonts", dgm::Utility::loadFont, { ".ttf" });
        !result)
    {
        throw std::runtime_error(uni::format(
            "Could not load font: {}", result.error().getMessage()));
    }

    if (auto result = resmgr.loadResourcesFromDirectory<tgui::Font>(
            assetDir / "fonts", loadTguiFont, { ".ttf" });
        !result)
    {
        throw std::runtime_error(uni::format(
            "Could not load font: {}", result.error().getMessage()));
    }

    if (auto result = resmgr.loadResourcesFromDirectory<tgui::Theme::Ptr>(
            assetDir / "ui-themes", loadTguiTheme, { ".txt" });
        !result)
    {
        throw std::runtime_error(uni::format(
            "Could not load theme: {}", result.error().getMessage()));
    }

    if (auto result = resmgr.loadResourcesFromDirectory<sf::Texture>(
            assetDir / "graphics", dgm::Utility::loadTexture, { ".png" });
        !result)
    {
        throw std::runtime_error(uni::format(
            "Could not load texture: {}", result.error().getMessage()));
    }

    if (auto result = resmgr.loadResourcesFromDirectory<dgm::AnimationStates>(
            assetDir / "graphics",
            dgm::Utility::loadAnimationStates,
            { ".anim" });
        !result)
    {
        throw std::runtime_error(uni::format(
            "Could not load animation states: {}",
            result.error().getMessage()));
    }

    if (auto result = resmgr.loadResourcesFromDirectory<dgm::Clip>(
            assetDir / "graphics", dgm::Utility::loadClip, { ".clip" });
        !result)
    {
        throw std::runtime_error(uni::format(
            "Could not load clip: {}", result.error().getMessage()));
    }

    if (auto result = resmgr.loadResourcesFromDirectory<sf::SoundBuffer>(
            assetDir / "sounds", dgm::Utility::loadSound, { ".wav" });
        !result)
    {
        throw std::runtime_error(uni::format(
            "Could not load sound: {}", result.error().getMessage()));
    }

    if (auto result = resmgr.loadResourcesFromDirectory<sf::Music>(
            assetDir / "music", loadSong, { ".ogg" });
        !result)
    {
        throw std::runtime_error(uni::format(
            "Could not load song: {}", result.error().getMessage()));
    }

    if (auto result = resmgr.loadResourcesFromDirectory<Playlist>(
            assetDir / "music", loadPlaylist, { ".json" });
        !result)
    {
        throw std::runtime_error(uni::format(
            "Could not load playlist: {}", result.error().getMessage()));
    }

    return resmgr;
}

AppSettings_StorageModel
ResourceLoader::loadSettings(const std::filesystem::path& file)
{
    auto settingsJson = AppStorage::loadFile(file);

    if (settingsJson)
    {
        try
        {
            AppSettings_StorageModel settings =
                nlohmann::json::parse(settingsJson.value());
            return settings;
        }
        catch (const std::exception& ex)
        {
            sf::err() << ex.what() << std::endl;
        }
    }
    else
    {
        sf::err() << settingsJson.error().getMessage() << std::endl;
    }

    return AppSettings_StorageModel {};
}
