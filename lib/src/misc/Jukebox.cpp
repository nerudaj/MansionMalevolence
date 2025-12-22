#include "misc/Jukebox.hpp"
#include "misc/Playlist.hpp"

Jukebox::Jukebox(dgm::ResourceManager& resmgr) : resmgr(resmgr)
{
    const auto& playlist = resmgr.get<Playlist>("playlist.json");
    menuSongs = playlist.menu;
    ingameSongs = playlist.game;
    playNextSong();
}

void Jukebox::switchMode(JukeboxMode newMode)
{
    if (mode == newMode) return;
    mode = newMode;
    playNextSong();
}

void Jukebox::setVolume(float newVolume)
{
    assert(0.f <= newVolume && newVolume <= 100.f);
    assert(!currentSongName.empty());
    resmgr.getMutable<sf::Music>(currentSongName).setVolume(newVolume);
    volume = newVolume;
}

void Jukebox::playNextSong()
{
    if (!currentSongName.empty())
        resmgr.getMutable<sf::Music>(currentSongName).stop();

    assert(!menuSongs.empty());
    assert(!ingameSongs.empty());

    if (mode == JukeboxMode::Menu)
    {
        currentSongName = menuSongs[cnt % menuSongs.size()];
    }
    else
    {
        currentSongName = ingameSongs[cnt % ingameSongs.size()];
    }

    ++cnt;

    auto& song = resmgr.getMutable<sf::Music>(currentSongName);
    song.setVolume(volume);
    song.setLooping(true);
    song.play();
}
