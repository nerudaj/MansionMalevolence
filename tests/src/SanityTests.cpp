#include "Helper.hpp"
#include "game/engine/GameRulesEngine.hpp"
#include <catch_amalgamated.hpp>

TEST_CASE("[Sanity]")
{
    SECTION("Can create dummy window and dependencies")
    {
        auto window = Helper::createDummyWindow();
        auto dic = Helper::createDummyDependencies(window);
        REQUIRE(true);
    }

    SECTION("Can default-create settings and serialize-deserialize them")
    {
        const std::string& json = nlohmann::json(AppSettings {}).dump(4);
        const AppSettings settings = nlohmann::json::parse(json);
    }

    SECTION("Red and green herbs can mix")
    {
        REQUIRE(GameRulesEngine::canInventoryCardCombineWithIncoming(
            CardBuilder::createCard(CardType::RedHerb),
            CardBuilder::createCard(CardType::GreenHerb)));
        REQUIRE(GameRulesEngine::canInventoryCardCombineWithIncoming(
            CardBuilder::createCard(CardType::GreenHerb),
            CardBuilder::createCard(CardType::RedHerb)));
    }
}
