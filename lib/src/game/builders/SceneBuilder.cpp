#include "game/builders/SceneBuilder.hpp"
#include "game/engine/RenderingEngine.hpp"
#include "misc/Compatibility.hpp"
#include <random>

[[nodiscard]] static std::list<Card> generateDeck()
{
    // Note always fill the deck with two useless cards so the rendering never
    // breaks
    auto&& deck =
        std::vector<Card> { CardBuilder::createCard(CardType::BookCase),
                            CardBuilder::createCard(CardType::ShieldDoor),
                            CardBuilder::createCard(CardType::ShieldKey),
                            CardBuilder::createCard(CardType::Zombie),
                            CardBuilder::createCard(CardType::Zombie),
                            CardBuilder::createCard(CardType::Zombie),
                            CardBuilder::createCard(CardType::Zombie),
                            CardBuilder::createCard(CardType::Zombie),
                            CardBuilder::createCard(CardType::GreenHerb),
                            CardBuilder::createCard(CardType::Crate),
                            CardBuilder::createCard(CardType::MoonCrestLeft),
                            CardBuilder::createCard(CardType::Ammo),
                            CardBuilder::createCard(CardType::MoonCrestDoor) };

    /*auto randomCardType = []
    {
        return static_cast<CardType>(
            rand() % std::to_underlying(CardType::BookCase));
    };

    for (auto&& type :
         std::views::iota(0u, 20u)
             | std::views::transform(
                 [&](size_t) -> CardType { return randomCardType(); }))
    {
        deck.push_back(CardBuilder::createCard(type));
    }*/

    auto&& mt = std::mt19937 { std::random_device {}() };
    std::ranges::shuffle(deck, mt);

    return deck | uniranges::to<std::list>();
}

Scene SceneBuilder::createScene()
{

    const sf::Vector2f CARD_SIZE = { 76.f, 114.f };

    return Scene {
        .deck = generateDeck(),
        .inventory = { CardBuilder::createCard(CardType::Pistol), std::nullopt, std::nullopt },
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

void SceneBuilder::spawnCardsAfterFirstKeyTarget(Scene& scene)
{
    auto&& incoming =
        std::vector<Card> { CardBuilder::createCard(CardType::Shotgun),
                            CardBuilder::createCard(CardType::Ammo),
                            CardBuilder::createCard(CardType::GreenHerb),
                            CardBuilder::createCard(CardType::RedHerb),
                            CardBuilder::createCard(CardType::Licker),
                            CardBuilder::createCard(CardType::Licker),
                            CardBuilder::createCard(CardType::Cerberus),
                            CardBuilder::createCard(CardType::MoonCrestRight) };

    auto&& mt = std::mt19937 { std::random_device {}() };
    std::ranges::shuffle(incoming, mt);

    scene.deck.insert(scene.deck.end(), incoming.begin(), incoming.end());
}
