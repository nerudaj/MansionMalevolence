#include "input/TouchController.hpp"
#include <SFML/System/Err.hpp>

void TouchInput::reset()
{
    if (kind == TouchObjectKind::Button)
        // place touch position out of touchArea - button is not pressed
        touchPosition = touchArea.getPosition()
                        - sf::Vector2f {
                              touchArea.getRadius(),
                              touchArea.getRadius(),
                          };
    else
        touchPosition = touchArea.getPosition();
}

TouchModel::TouchModel(const sf::Vector2u& windowSize)
    : objects(std::array {
          TouchInput(
              TouchObjectKind::Button,
              { windowSize.x * 0.12f, windowSize.x * 0.12f },
              windowSize.x * 0.08f),
          TouchInput(
              TouchObjectKind::Button,
              { windowSize.x * 0.852f, windowSize.y * 0.258f },
              windowSize.x * 0.123f),
          TouchInput(
              TouchObjectKind::Button,
              { windowSize.x * 0.852f, windowSize.y * 0.5375f },
              windowSize.x * 0.123f),
      })
{
}

void TouchController::processEvent(const std::optional<sf::Event>& e)
{
    if (e->is<sf::Event::TouchBegan>())
        processEvent(*e->getIf<sf::Event::TouchBegan>());
    else if (e->is<sf::Event::TouchEnded>())
        processEvent(*e->getIf<sf::Event::TouchEnded>());
    else if (e->is<sf::Event::TouchMoved>())
        processEvent(*e->getIf<sf::Event::TouchMoved>());
}

[[nodiscard]] bool TouchController::isBackPressed() const
{
    return model.pauseButton.readButton();
}

bool TouchController::isTakePressed() const
{
    return model.takeButton.readButton();
}

bool TouchController::isSkipPressed() const
{
    return model.skipButton.readButton();
}

void TouchController::processEvent(const sf::Event::TouchBegan& e)
{
    for (auto&& [idx, object] : std::ranges::views::enumerate(model.objects))
    {
        if (dgm::Collision::basic(object.touchArea, e.position))
        {
            object.touchPosition = sf::Vector2f(e.position);
            model.fingerToTouchObject[e.finger] = idx;
        }
    }
}

void TouchController::processEvent(const sf::Event::TouchEnded& e)
{
    if (!model.fingerToTouchObject.contains(e.finger)) return;

    auto idx = model.fingerToTouchObject.at(e.finger);
    model.fingerToTouchObject.erase(e.finger);
    model.objects[idx].reset();
}

void TouchController::processEvent(const sf::Event::TouchMoved& e)
{
    // Update touchPosition, but normalize it so it stays inside the touchArea
    // even when the finger moves out of it
    if (!model.fingerToTouchObject.contains(e.finger)) return;

    auto idx = model.fingerToTouchObject.at(e.finger);
    auto& obj = model.objects[idx];
    auto direction = sf::Vector2f(e.position) - obj.touchArea.getPosition();
    auto length = direction.length();
    obj.touchPosition =
        obj.touchArea.getPosition()
        + dgm::Math::toUnit(direction)
              * std::clamp(length, 0.f, obj.touchArea.getRadius());
}
