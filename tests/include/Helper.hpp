#pragma once

#include "Paths.hpp"
#include <filesystem/ResourceLoader.hpp>
#include <fstream>
#include <misc/DependencyContainer.hpp>

class Helper final
{
public:
    static dgm::Window createDummyWindow()
    {
        return dgm::Window(dgm::WindowSettings {
            .resolution = { 1u, 1u },
            .title = "",
            .useFullscreen = false,
        });
    }

    static DependencyContainer createDummyDependencies(dgm::Window& window)
    {
        return DependencyContainer(
            window, ASSETS_PATH, Language::English, AppSettings {});
    }

    static std::string loadAllText(const std::filesystem::path& path)
    {
        std::ifstream load(path);
        load.seekg(0, std::ios_base::end);
        auto len = load.tellg();

        auto result = std::string(len, '\0');
        load.seekg(0, std::ios_base::beg);

        load.read(result.data(), len);
        return result;
    }
};

class GuiHelper
{
public:
    static void clickButton(tgui::Button::Ptr button)
    {
        button->leftMousePressed(
            button->getPosition() + button->getSize() / 2.f);
        button->leftMouseReleased(
            button->getPosition() + button->getSize() / 2.f);
    }
};