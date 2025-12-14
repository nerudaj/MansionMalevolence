#pragma once

#include "DGM/classes/ResourceManager.hpp"
#include <SFML/Audio/Music.hpp>
#include <string>
#include <vector>

enum class [[nodiscard]] JukeboxMode
{
    None,
    Menu,
    Game
};

class [[nodiscard]] Jukebox final
{
public:
    explicit Jukebox(dgm::ResourceManager& resmgr);

    Jukebox(const Jukebox&) = delete;
    Jukebox(Jukebox&&) = delete;

public:
    void switchMode(JukeboxMode mode);

    void setVolume(float volume);

private:
    void playNextSong();

public:
    dgm::ResourceManager& resmgr;
    std::vector<std::string> menuSongs;
    std::vector<std::string> ingameSongs;
    JukeboxMode mode = JukeboxMode::None;
    std::string currentSongName = "";
    static inline size_t cnt = 0; // used for random selection of a next song
};
