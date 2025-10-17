#include "Helper.hpp"
#include "Paths.hpp"
#include <catch_amalgamated.hpp>
#include <filesystem/TiledLoader.hpp>
#include <nlohmann/json.hpp>

TEST_CASE("[TiledLoader]")
{
    SECTION("Can load demo file")
    {
        tiled::FiniteMapModel model =
            TiledLoader::loadLevel(TESTFILES_PATH / "tiled-map-01.json");

        REQUIRE(model.width == 64u);
        REQUIRE(model.height == 64u);
        REQUIRE(model.tilewidth == 64u);
        REQUIRE(model.tileheight == 64u);

        REQUIRE(model.layers.size() == 1u);
    }

    SECTION("Can load demo file #2")
    {
        tiled::FiniteMapModel model =
            TiledLoader::loadLevel(TESTFILES_PATH / "tiled-map-02.json");

        REQUIRE(model.layers.size() == 2u);
        REQUIRE(
            std::holds_alternative<tiled::ObjectGroupModel>(model.layers[1]));

        auto& group = std::get<tiled::ObjectGroupModel>(model.layers[1]);
        REQUIRE(group.objects.size() == 11u);
    }
}