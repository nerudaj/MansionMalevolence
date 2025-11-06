#include "game/builders/SceneBuilder.hpp"
#include "game/engine/RenderingEngine.hpp"
#include "misc/Compatibility.hpp"

Scene SceneBuilder::createScene()
{
    auto randomCardType = []
    {
        return static_cast<CardType>(
            rand() % std::to_underlying(CardType::Max));
    };

    std::ignore = std::views::iota(0u, 20u)
                  | std::views::transform(
                      [&](size_t) -> CardType { return randomCardType(); })
                  | std::views::transform(CardBuilder::createCard)
                  | uniranges::to<std::list>();

    const sf::Vector2f CARD_SIZE = { 76.f, 114.f };

    return Scene {
        .deck = { CardBuilder::createCard(CardType::RedHerb),
                  CardBuilder::createCard(CardType::GreenHerb),
                  CardBuilder::createCard(CardType::Pistol),
                  CardBuilder::createCard(CardType::Zombie),
                  CardBuilder::createCard(CardType::Licker) },
        .inventory = {},
        .mainCardBody =
            dgm::Rect(RenderingEngine::getDeckCardOffset(), CARD_SIZE),
        .healthbarBody = dgm::Rect({ 35.f, 7.f }, { 85.f, 16.f }),
        .trashBody = dgm::Rect({ 7.f, 211.f }, { 114.f, 10.f }),
        .inventoryBodies = { dgm::Rect(
                                 RenderingEngine::getNthInventoryCardOffset(0),
                                 CARD_SIZE / 3.f),
                             dgm::Rect(
                                 RenderingEngine::getNthInventoryCardOffset(1),
                                 CARD_SIZE / 3.f),
                             dgm::Rect(
                                 RenderingEngine::getNthInventoryCardOffset(2),
                                 CARD_SIZE / 3.f), },
    };
}
