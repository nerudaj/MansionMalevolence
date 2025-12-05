#include "game/builders/SceneBuilder.hpp"
#include "game/engine/RenderingEngine.hpp"
#include "misc/Compatibility.hpp"
#include <random>

template<class T>
void shuffleVec(std::vector<T>& vec)
{
    auto&& mt = std::mt19937 { std::random_device {}() };
    std::ranges::shuffle(vec, mt);
}

[[nodiscard]] static std::list<Card> generateTutorialDeck()
{
    return { CardBuilder::createCard(CardType::Zombie),
             CardBuilder::createCard(CardType::Zombie),
             CardBuilder::createCard(CardType::ShieldDoor),
             CardBuilder::createCard(CardType::Zombie),
             CardBuilder::createCard(CardType::GreenHerb),
             CardBuilder::createCard(CardType::Zombie),
             CardBuilder::createCard(CardType::Ammo),
             CardBuilder::createCard(CardType::ShieldKey) };
}

[[nodiscard]] static std::list<Card> generateNormalDeck()
{
    auto&& deckPart1 = std::vector<Card> {
        CardBuilder::createCard(CardType::DiamondDoor),
        CardBuilder::createCard(CardType::CrestDoorEmpty),
        CardBuilder::createCard(CardType::Zombie),
        CardBuilder::createCard(CardType::Zombie),
        CardBuilder::createCard(CardType::Cerberus),
        CardBuilder::createCard(CardType::Crate),
    };

    auto&& deckPart2 = std::vector<Card> {
        CardBuilder::createCard(CardType::Zombie),
        CardBuilder::createCard(CardType::Cerberus),
        CardBuilder::createCard(CardType::Ammo),
        CardBuilder::createCard(CardType::DiamondKey),
        CardBuilder::createCard(CardType::RedHerb),
        CardBuilder::createCard(CardType::MoonCrest),
    };

    shuffleVec(deckPart1);
    shuffleVec(deckPart2);

    return std::views::join(std::vector { deckPart1, deckPart2 })
           | uniranges::to<std::list>();
}

[[nodiscard]] static std::list<Card> generateHardDeck()
{
    return {
        CardBuilder::createCard(CardType::DiamondDoor),
        CardBuilder::createCard(CardType::DiamondKey),
        CardBuilder::createCard(CardType::Zombie),
        CardBuilder::createCard(CardType::Zombie),
        CardBuilder::createCard(CardType::Zombie),
        CardBuilder::createCard(CardType::Cerberus),
        CardBuilder::createCard(CardType::RedJewel),
        CardBuilder::createCard(CardType::YellowJewelBox),
        CardBuilder::createCard(CardType::SunCrest),
        CardBuilder::createCard(CardType::Ammo),
        CardBuilder::createCard(CardType::RedHerb),
    };
}

[[nodiscard]] static std::list<Card>
generateInitialDeck(const GameScenario scenario)
{
    switch (scenario)
    {
    case GameScenario::Tutorial_1:
        return generateTutorialDeck();
    case GameScenario::Normal:
        return generateNormalDeck();
    case GameScenario::Hard:
        return generateHardDeck();
    }

    return {};
}

Scene SceneBuilder::createScene(const GameScenario scenario)
{
    const sf::Vector2f CARD_SIZE = { 76.f, 114.f };

    return Scene {
        .scenario = scenario,
        .deck = generateInitialDeck(scenario),
        .inventory = { CardBuilder::createCard(CardType::Pistol), std::nullopt, std::nullopt },
        .mainCardBody =
            dgm::Rect(sf::Vector2f { 6.f, 28.f }, CARD_SIZE),
        .healthbarBody = dgm::Rect({ 3.f, 150.f }, { 80.f, 24.f }),
        .trashBody = dgm::Rect({ 94.f, 102.f }, { 25.f, 38.f }),
// clang-format off
        .inventoryBodies = {
            dgm::Rect(
                RenderingEngine::getNthInventoryCardOffset(0),
                CARD_SIZE / 3.f),
            dgm::Rect(
                RenderingEngine::getNthInventoryCardOffset(1),
                CARD_SIZE / 3.f),
            dgm::Rect(
                RenderingEngine::getNthInventoryCardOffset(2),
                CARD_SIZE / 3.f),
        },
        .choiceBodies = {
            dgm::Rect(
                RenderingEngine::getNthBoosterChoiceOffset(0),
                CARD_SIZE / 3.f),
            dgm::Rect(
                RenderingEngine::getNthBoosterChoiceOffset(1),
                CARD_SIZE / 3.f),
            dgm::Rect(
                RenderingEngine::getNthBoosterChoiceOffset(2),
                CARD_SIZE / 3.f),
        },
// clang-format on
    };
}

std::list<Card> SceneBuilder::getCardsForRoom(
    const GameScenario scenario, const int completedLinkID)
{
    if (scenario == GameScenario::Normal
        && completedLinkID == SPECIAL_DIAMOND_KEYDOOR)
    {
        auto&& incoming = std::vector<Card> {
            CardBuilder::createCard(CardType::Shotgun),
            CardBuilder::createCard(CardType::SunCrest),
            CardBuilder::createCard(CardType::GreenHerb),
            CardBuilder::createCard(CardType::Licker),
            CardBuilder::createCard(CardType::Licker),
            CardBuilder::createCard(CardType::Licker),
            CardBuilder::createCard(CardType::Ammo),
        };

        shuffleVec(incoming);

        return std::list(incoming.begin(), incoming.end());
    }

    if (scenario == GameScenario::Hard
        && completedLinkID == SPECIAL_DIAMOND_KEYDOOR)
    {
        auto&& incoming = std::vector<Card> {
            CardBuilder::createCard(CardType::MoonCrestLeft),
            CardBuilder::createCard(CardType::CrestDoorEmpty),
            CardBuilder::createCard(CardType::ShieldKey),
            CardBuilder::createCard(CardType::ShieldDoor),
            CardBuilder::createCard(CardType::GreenHerb),
            CardBuilder::createCard(CardType::Crate),
            CardBuilder::createCard(CardType::Shotgun),
            CardBuilder::createCard(CardType::Cerberus),
            CardBuilder::createCard(CardType::Licker),
            CardBuilder::createCard(CardType::Zombie),
        };

        shuffleVec(incoming);

        return std::list(incoming.begin(), incoming.end());
    }

    if (scenario == GameScenario::Hard
        && completedLinkID == SPECIAL_SHIELD_KEYDOOR)
    {
        auto&& incoming = std::vector<Card> {
            CardBuilder::createCard(CardType::YellowJewel),
            CardBuilder::createCard(CardType::RedJewelBox),
            CardBuilder::createCard(CardType::Ammo),
            CardBuilder::createCard(CardType::Cerberus),
            CardBuilder::createCard(CardType::Licker),
            CardBuilder::createCard(CardType::Licker),
            CardBuilder::createCard(CardType::Zombie),
            CardBuilder::createCard(CardType::Crate),
        };

        shuffleVec(incoming);

        return std::list(incoming.begin(), incoming.end());
    }

    if (scenario == GameScenario::Hard && completedLinkID == SPECIAL_CREST_DOOR)
    {
        auto&& incoming = std::vector<Card> {
            CardBuilder::createCard(CardType::Crate),
            CardBuilder::createCard(CardType::Tyrant),
            CardBuilder::createCard(CardType::Vaccine),
        };

        return std::list(incoming.begin(), incoming.end());
    }

    return {};
}

std::array<CardType, 3u> SceneBuilder::generateBooster()
{
    auto possibleCards = std::vector {
        CardType::Ammo,    CardType::Ammo,     CardType::RedHerb,
        CardType::RedHerb, CardType::FirstAid, CardType::GreenHerb,
    };

    shuffleVec(possibleCards);

    auto result = std::array<CardType, 3u> {};
    for (unsigned idx = 0; idx < result.size(); ++idx)
        result[idx] = possibleCards[idx];
    return result;
}
