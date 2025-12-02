#include "game/builders/CardBuilder.hpp"
#include "game/builders/SceneBuilder.hpp"
#include "game/engine/GameRulesEngine.hpp"
#include <catch_amalgamated.hpp>

TEST_CASE("[GameRulesEngine]")
{
    auto&& scene = SceneBuilder::createScene(GameScenario::Tutorial_1);
    auto&& events = EventQueue<GameEvent>();
    auto&& touch = TouchController(sf::Vector2u(INTERNAL_GAME_RESOLUTION));
    auto&& resmgr = dgm::ResourceManager();
    auto&& audioEngine = AudioEngine(resmgr);
    auto&& input = Input({}, touch);
    auto&& engine =
        GameRulesEngine(events, audioEngine, scene, input, VideoSettings {});

    SECTION("getUsableInventorySlot")
    {
        SECTION("Returns first slot if inventory is empty")
        {
            scene.inventory[0].reset();
            scene.inventory[1].reset();
            scene.inventory[2].reset();

            auto result = engine.getUsableInventorySlot(Card {});
            REQUIRE(result.has_value());
            REQUIRE(result.value() == 0u);
        }

        SECTION("Returns first free slot if first one is used")
        {
            scene.inventory[0] = CardBuilder::createCard(CardType::Ammo);
            auto result = engine.getUsableInventorySlot(Card {});
            REQUIRE(result.has_value());
            REQUIRE(result.value() == 1u);
        }

        SECTION("Returns index of herb card when it can mix with incoming card")
        {
            scene.inventory[0] = CardBuilder::createCard(CardType::Empty);
            scene.inventory[1] = CardBuilder::createCard(CardType::Empty);
            scene.inventory[2] = CardBuilder::createCard(CardType::GreenHerb);
            auto result = engine.getUsableInventorySlot(
                CardBuilder::createCard(CardType::RedHerb));
            REQUIRE(result.has_value());
            REQUIRE(result.value() == 2u);
        }
    }
}
