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
    // TODO: placeholder
    return {
        CardBuilder::createCard(CardType::DiamondDoor),
        CardBuilder::createCard(CardType::CrestDoorEmpty),
        CardBuilder::createCard(CardType::Zombie),
        CardBuilder::createCard(CardType::Zombie),
        CardBuilder::createCard(CardType::Cerberus),
        CardBuilder::createCard(CardType::CrimsonHead),
        CardBuilder::createCard(CardType::Crate),
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
            dgm::Rect(RenderingEngine::getDeckCardOffset(), CARD_SIZE),
        .healthbarBody = dgm::Rect({ 35.f, 7.f }, { 85.f, 16.f }),
        .trashBody = dgm::Rect({ 5.f, 207.f }, { 116.f, 18.f }),
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

GameState SceneBuilder::updateScene(Scene& scene, const int completedLinkID)
{
    if (scene.scenario == GameScenario::Tutorial_1
        && completedLinkID == SPECIAL_SHIELD_KEYDOOR)
        return GameState::Finished;

    if (scene.scenario == GameScenario::Normal
        && completedLinkID == SPECIAL_CREST_DOOR)
        return GameState::Finished;

    if (scene.scenario == GameScenario::Normal
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

        scene.deck.insert(scene.deck.end(), incoming.begin(), incoming.end());
    }

    return GameState::Continue;
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
