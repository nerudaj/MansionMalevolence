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
    else if (scenario == GameScenario::Hard)
        return std::make_unique<HardScenarioBuilder>();
    else if (scenario == GameScenario::Nightmare)
        return std::make_unique<NightmareScenarioBuilder>();
    else if (scenario == GameScenario::OneRoom)
        return std::make_unique<OneRoomScenarioBuilder>();
    throw std::runtime_error("unsupported scenario");
}

Scene SceneBuilder::createScene(const GameScenario scenario)
{
    auto builder = makeBuilder(scenario);

    return Scene {
        .deck = builder->generateStartRoom(),
        .infection = {
            .limit = builder->getInfectionLimit(),
        },
        .builder = std::move(builder),
        .inventory = { CardBuilder::createCard(CardType::Pistol),
                       std::nullopt,
                       std::nullopt },
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
    return 10;
}

std::list<Card> TutorialScenarioBuilder::generateStartRoom()
{
    return {
        CardBuilder::createCard(CardType::MixedHerbs),
        CardBuilder::createCard(CardType::Shotgun),
        CardBuilder::createCard(CardType::Ammo),
        CardBuilder::createCard(CardType::Tyrant),
        CardBuilder::createCard(CardType::Vaccine),
        CardBuilder::createCard(CardType::WeaponLockerKey),
        CardBuilder::createCard(CardType::LockedWeaponLocker),
        CardBuilder::createCard(CardType::MoonCrestLeft),
        CardBuilder::createCard(CardType::MoonCrestRight),
        CardBuilder::createCard(CardType::GreenHerb),
        CardBuilder::createCard(CardType::RedHerb),
        CardBuilder::createCard(CardType::RocketLauncher),
        CardBuilder::createCard(CardType::Licker),
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
    return 60; // prev 75
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
            CardBuilder::createCard(CardType::CrimsonHead),
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
    return 80; // prev 115
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
            CardBuilder::createCard(CardType::Licker),
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
    return 35; // prev 45
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

int NightmareScenarioBuilder::getInfectionLimit() const noexcept
{
    return 100; // prev 150
}

std::list<Card> NightmareScenarioBuilder::generateStartRoom()
{
    auto deck = std::vector {
        CardBuilder::createCard(CardType::RedHerb),
        CardBuilder::createCard(CardType::Cerberus),
        CardBuilder::createCard(CardType::Zombie),
        CardBuilder::createCard(CardType::Zombie),
        CardBuilder::createCard(CardType::DiamondDoor),
        CardBuilder::createCard(CardType::ShieldKey),
        CardBuilder::createCard(CardType::Crate),
        CardBuilder::createCard(CardType::DiamondKey),
        CardBuilder::createCard(CardType::CrimsonHead),
        CardBuilder::createCard(CardType::RedJewelBox),
    };

    shuffleVec(deck);

    return deck | uniranges::to<std::list>();
}

std::list<Card> NightmareScenarioBuilder::generateRoomDeck(const int linkID)
{
    if (linkID == SPECIAL_DIAMOND_KEYDOOR)
    {
        auto deck = std::vector {
            CardBuilder::createCard(CardType::RedJewel),
            CardBuilder::createCard(CardType::Licker),
            CardBuilder::createCard(CardType::ShieldDoor),
            CardBuilder::createCard(CardType::CrestDoorEmpty),
            CardBuilder::createCard(CardType::MoonCrestLeft),
            CardBuilder::createCard(CardType::YellowJewelBox),
            CardBuilder::createCard(CardType::WeaponLockerKey),
            CardBuilder::createCard(CardType::Cerberus),
            CardBuilder::createCard(CardType::Zombie),
            CardBuilder::createCard(CardType::Ammo),
            CardBuilder::createCard(CardType::GreenHerb),
        };

        shuffleVec(deck);

        return deck | uniranges::to<std::list>();
    }
    else if (linkID == SPECIAL_SHIELD_KEYDOOR)
    {
        auto deck = std::vector {
            CardBuilder::createCard(CardType::Ammo),
            CardBuilder::createCard(CardType::SunCrest),
            CardBuilder::createCard(CardType::Tyrant),
            CardBuilder::createCard(CardType::LockedWeaponLocker),
            CardBuilder::createCard(CardType::YellowJewel),
            CardBuilder::createCard(CardType::Zombie),
            CardBuilder::createCard(CardType::FirstAid),
        };

        shuffleVec(deck);

        return deck | uniranges::to<std::list>();
    }
    else if (linkID == SPECIAL_CREST_DOOR)
    {
        auto deck = std::vector {
            CardBuilder::createCard(CardType::Crate),
            CardBuilder::createCard(CardType::Licker),
            CardBuilder::createCard(CardType::Crate),
            CardBuilder::createCard(CardType::Tyrant),
            CardBuilder::createCard(CardType::Vaccine),
        };

        // Not shuffling on purpose

        return deck | uniranges::to<std::list>();
    }

    return std::list<Card>();
}

int OneRoomScenarioBuilder::getInfectionLimit() const noexcept
{
    return 100;
}

std::list<Card> OneRoomScenarioBuilder::generateStartRoom()
{
    auto deck = std::vector {
        CardBuilder::createCard(CardType::GreenHerb),
        CardBuilder::createCard(CardType::RedHerb),
        CardBuilder::createCard(CardType::FirstAid),
        CardBuilder::createCard(CardType::WeaponLockerKey),
        CardBuilder::createCard(CardType::LockedWeaponLocker),
        CardBuilder::createCard(CardType::Crate),
        CardBuilder::createCard(CardType::Crate),
        CardBuilder::createCard(CardType::SunCrest),
        CardBuilder::createCard(CardType::CrestDoorEmpty),
        CardBuilder::createCard(CardType::RedJewel),
        CardBuilder::createCard(CardType::RedJewelBox),
        CardBuilder::createCard(CardType::YellowJewel),
        CardBuilder::createCard(CardType::YellowJewelBox),
        CardBuilder::createCard(CardType::MoonCrestLeft),
        CardBuilder::createCard(CardType::Zombie),
        CardBuilder::createCard(CardType::Zombie),
        CardBuilder::createCard(CardType::Zombie),
        CardBuilder::createCard(CardType::Zombie),
        CardBuilder::createCard(CardType::Zombie),
        CardBuilder::createCard(CardType::CrimsonHead),
        CardBuilder::createCard(CardType::Cerberus),
        CardBuilder::createCard(CardType::Cerberus),
        CardBuilder::createCard(CardType::Cerberus),
        CardBuilder::createCard(CardType::Licker),
        CardBuilder::createCard(CardType::Licker),

    };

    shuffleVec(deck);

    return deck | uniranges::to<std::list>();
}

std::list<Card> OneRoomScenarioBuilder::generateRoomDeck(const int linkID)
{
    if (linkID == SPECIAL_CREST_DOOR)
    {
        auto deck = std::vector {
            CardBuilder::createCard(CardType::Crate),
            CardBuilder::createCard(CardType::Tyrant),
            CardBuilder::createCard(CardType::Vaccine),
        };

        // Not shuffling on purpose

        return deck | uniranges::to<std::list>();
    }

    return std::list<Card>();
}
