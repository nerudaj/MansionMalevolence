#include "Helper.hpp"
#include <appstate/AppStateMainMenu.hpp>
#include <catch_amalgamated.hpp>

TEST_CASE("[AppStateMainMenu]")
{
    auto settings = AppSettings {};
    auto window = Helper::createDummyWindow();
    auto app = dgm::App(window);
    auto dic = Helper::createDummyDependencies(window);

    SECTION("Can exit")
    {
        app.pushState<AppStateMainMenu>(dic, settings);

        auto button = dic.gui.get<tgui::Button>("MainMenu_Button_Exit");
        GuiHelper::clickButton(button);

        app.run(); // should consume the event and exit immediately
        REQUIRE(true);
    }
}
