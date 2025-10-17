#pragma once

#include "settings/AppSettings.hpp"
#include <DGM/dgm.hpp>
#include <filesystem>

class ResourceLoader final
{
public:
    /**
     *  \brief Loads all assets from the directory where the application is
     * installed and stores them within a resource manager instance.
     *
     *  On Android, assets are packed inside the application itself and
     * extracted through Android NDK interface (unlike AppStorage that uses
     * regular C++ APIs).
     */
    static dgm::ResourceManager
    loadResources(const std::filesystem::path& assetDir);

    /**
     *  \brief Load application settings from the AppStorage location
     *
     *  Any errors are converted into exceptions and thrown.
     */
    static AppSettings loadSettings(const std::filesystem::path& file);
};
