#pragma once

#include <DGM/classes/Animation.hpp>
#include <DGM/classes/Time.hpp>
#include <functional>
#include <game/events/GameEvents.hpp>
#include <optional>
#include <types/SemanticTypes.hpp>

struct Card;
struct Scene;

class [[nodiscard]] AnimationInterface
{
public:
    AnimationInterface(const sf::Time duration = sf::seconds(0.5f)) noexcept
        : duration(duration)
    {
    }

    AnimationInterface(AnimationInterface&&) = delete;
    AnimationInterface(const AnimationInterface&) = delete;
    virtual ~AnimationInterface() = default;

public:
    dgm::Animation::PlaybackStatus update(const dgm::Time& time)
    {
        elapsed += time.getElapsed();
        perc = elapsed / duration;
        return elapsed > duration ? dgm::Animation::PlaybackStatus::Finished
                                  : dgm::Animation::PlaybackStatus::Playing;
    }

    virtual void render(
        const Scene& scene,
        const sf::Vector2f& baseOffset,
        std::function<void(const Card&, const Position&, const Scale&)>
            renderCard,
        std::function<void(const Position&, const Scale&)> renderCardBack)
        const = 0;

    virtual std::optional<GameEvent> finalize() const = 0;

protected:
    float getPercFactor() const noexcept
    {
        return perc;
    }

private:
    sf::Time elapsed = sf::Time::Zero;
    sf::Time duration = sf::Time::Zero;
    float perc = 0.f;
};
