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

std::unique_ptr<ScenarioBuilderInterface>
makeBuilder(const GameScenario scenario)
{
    if (scenario == GameScenario::Tutorial_1)
        return std::make_unique<TutorialScenarioBuilder>();
    if (scenario == GameScenario::Easy)
        return std::make_unique<EasyScenarioBuilder>();
    else if (scenario == GameScenario::Normal)
        return std::make_unique<NormalScenarioBuilder>();
    return std::make_unique<HardScenarioBuilder>();
}

Scene SceneBuilder::createScene(const GameScenario scenario)
{
    const sf::Vector2f CARD_SIZE = { 76.f, 114.f };
    auto builder = makeBuilder(scenario);

    return Scene
    {
        .deck = builder->generateStartRoom(),
        .infectionLimit = builder->getInfectionLimit(),
        .builder = std::move(builder),
        .inventory = { CardBuilder::createCard(CardType::Pistol), std::nullopt, std::nullopt },
        .mainCardBody =
            dgm::Rect(sf::Vector2f { 6.f, 27.f }, CARD_SIZE),
        .healthbarBody = dgm::Rect({ 3.f, 150.f }, { 80.f, 24.f }),
        .trashBody = dgm::Rect({ 94.f, 102.f }, CARD_SIZE / 3.f),
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

std::array<CardType, 3u>
SceneBuilder::generateBooster(CardImage boosterCardImage)
{
    if (boosterCardImage == CardImage::WeaponLocker)
    {
        return { CardType::PistolParts, CardType::Shotgun, CardType::Crossbow };
    }

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

int TutorialScenarioBuilder::getInfectionLimit() const noexcept
{
    return 20;
}

std::list<Card> TutorialScenarioBuilder::generateStartRoom()
{
    return {
        CardBuilder::createCard(CardType::Zombie),
        CardBuilder::createCard(CardType::Zombie),
        CardBuilder::createCard(CardType::ShieldDoor),
        CardBuilder::createCard(CardType::Zombie),
        CardBuilder::createCard(CardType::GreenHerb),
        CardBuilder::createCard(CardType::Zombie),
        CardBuilder::createCard(CardType::Ammo),
        CardBuilder::createCard(CardType::WeaponLockerKey),
        CardBuilder::createCard(CardType::LockedWeaponLocker),
        CardBuilder::createCard(CardType::ShieldKey),
    };
}

std::list<Card> TutorialScenarioBuilder::generateRoomDeck(const int)
{
    return { CardBuilder::createCard(CardType::Vaccine) };
}

int NormalScenarioBuilder::getInfectionLimit() const noexcept
{
    return 75;
}

std::list<Card> NormalScenarioBuilder::generateStartRoom()
{
    auto deck = std::vector {
        CardBuilder::createCard(CardType::Ammo),
        CardBuilder::createCard(CardType::ShieldDoor),
        CardBuilder::createCard(CardType::Zombie),
        CardBuilder::createCard(CardType::Zombie),
        CardBuilder::createCard(CardType::Cerberus),
    };

    shuffleVec(deck);

    auto deck2 = std::vector {
        CardBuilder::createCard(CardType::Zombie),
        CardBuilder::createCard(CardType::ShieldKey),
        CardBuilder::createCard(CardType::DiamondDoor),
        CardBuilder::createCard(CardType::FirstAid),
    };

    shuffleVec(deck2);

    return std::views::join(std::vector { deck, deck2 })
           | uniranges::to<std::list>();
}

std::list<Card> NormalScenarioBuilder::generateRoomDeck(const int linkID)
{
    if (linkID == SPECIAL_SHIELD_KEYDOOR)
    {
        auto&& incoming = std::vector<Card> {
            CardBuilder::createCard(CardType::PistolParts),
            CardBuilder::createCard(CardType::SunCrest),
            CardBuilder::createCard(CardType::DiamondKey),
            CardBuilder::createCard(CardType::Cerberus),
            CardBuilder::createCard(CardType::CrimsonHead),
            CardBuilder::createCard(CardType::Crate),
            CardBuilder::createCard(CardType::GreenHerb),
        };

        shuffleVec(incoming);

        return std::list(incoming.begin(), incoming.end());
    }
    else if (linkID == SPECIAL_DIAMOND_KEYDOOR)
    {
        auto&& incoming = std::vector<Card> {
            CardBuilder::createCard(CardType::Shotgun),
            CardBuilder::createCard(CardType::MoonCrest),
            CardBuilder::createCard(CardType::CrestDoorEmpty),
            CardBuilder::createCard(CardType::Licker),
            CardBuilder::createCard(CardType::Zombie),
            CardBuilder::createCard(CardType::Cerberus),
            CardBuilder::createCard(CardType::Crate),
            CardBuilder::createCard(CardType::Ammo),
        };

        shuffleVec(incoming);

        return std::list(incoming.begin(), incoming.end());
    }
    else if (linkID == SPECIAL_CREST_DOOR)
    {
        return { CardBuilder::createCard(CardType::CrimsonHead),
                 CardBuilder::createCard(CardType::Licker),
                 CardBuilder::createCard(CardType::Vaccine) };
    }

    assert(false);
    return {};
}

int HardScenarioBuilder::getInfectionLimit() const noexcept
{
    return 115;
}

std::list<Card> HardScenarioBuilder::generateStartRoom()
{
    auto deck = std::vector {
        CardBuilder::createCard(CardType::Ammo),
        CardBuilder::createCard(CardType::MoonCrestLeft),
        CardBuilder::createCard(CardType::ShieldDoor),
        CardBuilder::createCard(CardType::DiamondDoor),
        CardBuilder::createCard(CardType::CrestDoorEmpty),
        CardBuilder::createCard(CardType::Zombie),
        CardBuilder::createCard(CardType::Zombie),
        CardBuilder::createCard(CardType::Cerberus),
        CardBuilder::createCard(CardType::Cerberus),
    };

    shuffleVec(deck);

    auto deck2 = std::vector {
        CardBuilder::createCard(CardType::DiamondKey),
        CardBuilder::createCard(CardType::FirstAid),
    };

    return std::views::join(std::vector { deck, deck2 })
           | uniranges::to<std::list>();
}

std::list<Card> HardScenarioBuilder::generateRoomDeck(const int linkID)
{
    if (linkID == SPECIAL_DIAMOND_KEYDOOR)
    {
        auto&& incoming = std::vector<Card> {
            CardBuilder::createCard(CardType::RedJewel),
            CardBuilder::createCard(CardType::UnlockedWeaponLocker),
            CardBuilder::createCard(CardType::Crate),
            CardBuilder::createCard(CardType::CrimsonHead),
            CardBuilder::createCard(CardType::SunCrest),
            CardBuilder::createCard(CardType::ShieldKey),
            CardBuilder::createCard(CardType::Zombie),
            CardBuilder::createCard(CardType::Ammo),
            CardBuilder::createCard(CardType::Cerberus),
        };

        shuffleVec(incoming);

        return incoming | uniranges::to<std::list>();
    }
    else if (linkID == SPECIAL_SHIELD_KEYDOOR)
    {
        auto&& incoming = std::vector<Card> {
            CardBuilder::createCard(CardType::RedJewelBox),
            CardBuilder::createCard(CardType::MoonCrestRight),
            CardBuilder::createCard(CardType::Licker),
            CardBuilder::createCard(CardType::Licker),
            CardBuilder::createCard(CardType::Zombie),
            CardBuilder::createCard(CardType::GreenHerb),
            CardBuilder::createCard(CardType::Ammo),
            CardBuilder::createCard(CardType::Ammo),
        };

        shuffleVec(incoming);

        return incoming | uniranges::to<std::list>();
    }
    else if (linkID == SPECIAL_CREST_DOOR)
    {
        auto&& incoming = std::vector<Card> {
            CardBuilder::createCard(CardType::Crate),
            CardBuilder::createCard(CardType::Tyrant),
            CardBuilder::createCard(CardType::Vaccine),
        };

        return incoming | uniranges::to<std::list>();
    }

    assert(false);
    return {};
}

int EasyScenarioBuilder::getInfectionLimit() const noexcept
{
    return 45;
}

std::list<Card> EasyScenarioBuilder::generateStartRoom()
{
    auto deck = std::vector {
        CardBuilder::createCard(CardType::Ammo),
        CardBuilder::createCard(CardType::GreenHerb),
        CardBuilder::createCard(CardType::ShieldDoor),
        CardBuilder::createCard(CardType::Zombie),
        CardBuilder::createCard(CardType::Zombie),
        CardBuilder::createCard(CardType::Zombie),
        CardBuilder::createCard(CardType::DiamondDoor),
    };

    shuffleVec(deck);

    auto deck2 = std::vector {
        CardBuilder::createCard(CardType::ShieldKey),
    };

    return std::views::join(std::vector { deck, deck2 })
           | uniranges::to<std::list>();
}

std::list<Card> EasyScenarioBuilder::generateRoomDeck(const int linkID)
{
    if (linkID == SPECIAL_SHIELD_KEYDOOR)
    {
        auto deck = std::vector {
            CardBuilder::createCard(CardType::DiamondKey),
            CardBuilder::createCard(CardType::Crate),
            CardBuilder::createCard(CardType::Cerberus),
            CardBuilder::createCard(CardType::Crossbow),
            CardBuilder::createCard(CardType::Zombie),
            CardBuilder::createCard(CardType::RedHerb),
        };

        shuffleVec(deck);

        return deck | uniranges::to<std::list>();
    }
    else if (linkID == SPECIAL_DIAMOND_KEYDOOR)
    {
        auto deck = std::vector {
            CardBuilder::createCard(CardType::Licker),
            CardBuilder::createCard(CardType::Vaccine),
        };

        // Not shuffling on purpose

        return deck | uniranges::to<std::list>();
    }

    assert(false);
    return {};
}
