#pragma once

#include <DGM/classes/Error.hpp>
#include <expected>
#include <nlohmann/json.hpp>
#include <string>

/**
 * Class that represents host-specific application storage
 *
 * \detail This is meant for storing settings, save files, user-created content,
 * etc.
 *
 * On Windows, this storage defaults to %APPDATA%/<AppName>.
 * On Android, this storage defaults to /Android/data/<appid>/files
 */
class AppStorage
{
public:
    static std::expected<std::string, dgm::Error>
    loadFile(const std::filesystem::path& file);

    static void
    saveFile(const std::filesystem::path& file, const std::string& data);

    /**
     * Helper wrapper method for saveFile. If an object is serializable into
     * nlohmann::json, it performs the serialization and then calls saveFile.
     */
    template<class Serializable>
    static void saveFile(
        const std::filesystem::path& path, const Serializable& serializable)
    {
        saveFile(path, nlohmann::json(serializable).dump(4));
    }
};
