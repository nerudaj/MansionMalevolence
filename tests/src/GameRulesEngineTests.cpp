#include "game/builders/CardBuilder.hpp"
#include "game/builders/SceneBuilder.hpp"
#include "game/engine/GameRulesEngine.hpp"
#include <catch_amalgamated.hpp>
#include <thread>

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

        SECTION("When deck is empty")
        {
            dgm::Time time;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            time.reset();

            SECTION("Moves discard back to deck if there is any")
            {
                scene.deck.clear();
                scene.discard = scene.builder->generateStartRoom();

                engine.update(time);
                events.processEvents(engine);
                REQUIRE(scene.discard.empty());
                REQUIRE(!scene.cardsToAdd.empty());
                REQUIRE(scene.activeAnimation);

                engine.update(time);
                events.processEvents(engine);
                REQUIRE(!scene.activeAnimation);
                REQUIRE(scene.cardsToAdd.empty());
                REQUIRE(!scene.deck.empty());
            }

            SECTION("Does nothing when there is no discard")
            {
                scene.deck.clear();
                engine.update(time);
                engine.handleTake();
                events.processEvents(engine);
                engine.update(time);
                engine.handleSkip();
                events.processEvents(engine);
                engine.update(time);
                events.processEvents(engine);
            }
        }
    }
}
