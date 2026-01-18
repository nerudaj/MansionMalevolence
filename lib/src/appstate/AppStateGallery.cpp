#include "appstate/AppStateGallery.hpp"
#include "appstate/CommonHandler.hpp"
#include "gui/Builders.hpp"

AppStateGallery::AppStateGallery(dgm::App& app, DependencyContainer& dic)
    : dgm::AppState(app)
    , dic(dic)
    , scene(Scene {})
    , renderer(app.window, dic.resmgr, scene, dic.settings, dic.touchController)
    , canvas(tgui::CanvasSFML::create())
{
    buildLayout();
    dic.touchController.clearInputs();
}

void AppStateGallery::input()
{
    CommonHandler::handleInput(app, dic, dic.settings.input);
}

void AppStateGallery::update() {}

void AppStateGallery::draw()
{
    renderer.renderCard(*canvas, currentCard, { 0.f, 0.f }, { 1.f, 1.f });
    canvas->display();
    dic.gui.draw();
    dic.virtualCursor.draw();
}

void AppStateGallery::buildLayout()
{
    dic.gui.rebuildWith(
        DefaultLayoutBuilder(dic.sizer)
            .withNoBackgroundImage()
            .withTitle(
                dic.strings.getString(StringId::CardGallery), HeadingLevel::H2)
            .withContent(WidgetBuilder::createCarousel(
                std::to_underlying(CardType::Max) - 1,
                [this](tgui::Container::Ptr content, size_t pageIdx)
                { onPageChanged(content, pageIdx); },
                dic.sizer))
            .withNoTopLeftButton()
            .withNoTopRightButton()
            .withBottomLeftButton(WidgetBuilder::createButton(
                dic.strings.getString(StringId::Back),
                [this] { onBack(); },
                dic.sizer))
            .withNoBottomRightButton()
            .build());

    updateView();
}

void AppStateGallery::onBack()
{
    app.popState();
}

void AppStateGallery::onPageChanged(tgui::Container::Ptr content, size_t idx)
{
    currentCard = CardBuilder::createCard(static_cast<CardType>(idx + 1));
    content->add(canvas);
    updateView();
}

void AppStateGallery::updateView()
{
    const auto camera =
        RenderingEngine::createFullscreenCamera(canvas->getSize(), CARD_SIZE);
    canvas->setView(camera.getCurrentView());
}
