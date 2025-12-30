#pragma once

#include "game/enums/SoundId.hpp"
#include <DGM/classes/ResourceManager.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

class [[nodiscard]] AudioEngine final
{
public:
    explicit AudioEngine(const dgm::ResourceManager& resmgr) noexcept
        : resmgr(resmgr)
    {
    }

public:
    sf::Time playSound(SoundId::IdType id)
    {
        const auto& buffer = resmgr.get<sf::SoundBuffer>(id);
        sound.setBuffer(buffer);
        sound.play();
        return buffer.getDuration();
    }

    void setVolume(float volume)
    {
        sound.setVolume(volume);
    }

private:
    const dgm::ResourceManager& resmgr;
    sf::SoundBuffer emptySoundBuffer;
    sf::Sound sound = sf::Sound(emptySoundBuffer);
};
