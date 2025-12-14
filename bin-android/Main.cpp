#include <appstate/AppStateMainMenu.hpp>
#include <DGM/dgm.hpp>
#include <filesystem/AppStorage.hpp>
#include <misc/CMakeVars.hpp>
#include <misc/Compatibility.hpp>
#include <misc/DependencyContainer.hpp>
#include <SFML/System/Err.hpp>

const auto SETTINGS_FILE_NAME = std::filesystem::path("settings.json");

auto getVerticalResolution()
{
    auto&& modes = sf::VideoMode::getFullscreenModes();
    for (auto&& mode : modes)
    {
        if (mode.size.x < mode.size.y) return mode.size;
    }

    if (!modes.empty())
    {
        return modes[0].size;
    }

    return sf::Vector2u { 405, 720 };
}

int main(int, char*[])
{
    try
    {
        auto&& settings = ResourceLoader::loadSettings(SETTINGS_FILE_NAME);

        // sf::VideoMode::getDesktopMode() is broken, returning 2174x1080 on my device
        // which has resolution of 2400x1080 (as reported by getFullscreenModes)
        auto&& modes = sf::VideoMode::getFullscreenModes();
        if (!modes.empty())
          settings.video.resolution = getVerticalResolution();

        auto&& window = dgm::Window(dgm::WindowSettings {
            .resolution = settings.video.resolution,
            .title = CMakeVars::TITLE,
            .useFullscreen = settings.video.fullscreen,
        });
        auto&& app = dgm::App(window);
        auto&& dependencies = DependencyContainer(window, "", Language::English, settings);

        settings.audio.registerObserver([&dependencies](const AudioSettings& settings)
        {
            dependencies.jukebox.setVolume(settings.musicVolume);
        });

        window.getSfmlWindowContext().setMouseCursorVisible(false);

        app.pushState<AppStateMainMenu>(dependencies, settings);
        app.run();
        
        AppStorage::saveFile(SETTINGS_FILE_NAME, settings);
    }
    catch (const std::exception& ex)
    {
        sf::err() << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
