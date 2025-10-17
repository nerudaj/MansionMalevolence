#include "game/engine/GameRulesEngine.hpp"
#include <algorithm>
#include <limits>

void GameRulesEngine::update(const dgm::Time& time)
{
    updateDummy(scene.dummy, time.getDeltaTime());
    updateDummyAnimation(scene.dummy.animation, scene.dummy.forward, time);
}

#pragma region "Demo logic, delete in production"

void GameRulesEngine::updateDummyAnimation(
    dgm::Animation& animation,
    const sf::Vector2f& forward,
    const dgm::Time& time)
{
    // Updating animations should be a job of a dedicated
    // component but this is a simple demo

    if (animation.getStateName() != "land")
    {
        if (forward.y < 0.f)
        {
            if (animation.getStateName() != "jump")
                gameEventQueue.pushEvent<DummyGameEvent>(
                    std::string("jump.wav"));
            animation.setState("jump", true);
        }
        else if (forward.y == 0.f)
        {
            if (animation.getStateName() == "fall")
            {
                if (animation.getStateName() != "land")
                    gameEventQueue.pushEvent<DummyGameEvent>(
                        std::string("land.wav"));
                animation.setState("land", false);
            }
            else if (forward.x == 0.f)
                animation.setState("idle", true);
            else
            {
                animation.setState("run", true);
            }
        }
        else
        {
            animation.setState("fall", true);
        }
    }

    if (animation.update(time) == dgm::Animation::PlaybackStatus::Finished
        && animation.getStateName() == "land")
    {
        animation.setState("idle", true);
    }
}

void GameRulesEngine::updateDummy(DummyEntity& dummy, const float deltaTime)
{
    const float JUMP_IMPULSE = -256.f;
    const float GRAVITY = 256.f;
    const float MAX_FALL_SPEED = 256.f;
    const float SPEED = 128.f;

    // Apply horizontal impulse
    const float airControlNerf = dummy.forward.y == 0.f ? 1.f : 0.75f;
    dummy.forward.x = input.getHorizontalVelocity() * SPEED * airControlNerf;

    // Apply vertical impulse
    const bool shouldJump = input.isJumpPressed() && dummy.forward.y == 0.f;
    if (shouldJump) dummy.forward.y = JUMP_IMPULSE;

    // Apply gravity
    dummy.forward.y = std::clamp(
        dummy.forward.y + GRAVITY * deltaTime,
        -std::numeric_limits<float>::infinity(),
        MAX_FALL_SPEED);

    dummy.body.move(dummy.forward * deltaTime);
    dummy.facingLeft =
        dummy.forward.x < 0.f || dummy.forward.x == 0.f && dummy.facingLeft;

    auto pos = dummy.body.getPosition();
    if (pos.y + dummy.body.getSize().y > scene.groundPosition.y)
    {
        dummy.forward.y = 0.f;
        dummy.body.setPosition(
            { pos.x, scene.groundPosition.y - dummy.body.getSize().y });
    }
}

#pragma region