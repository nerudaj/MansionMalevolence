#pragma once

#include "game/definitions/Scene.hpp"
#include "game/enums/GameScenario.hpp"

class [[nodiscard]] SceneBuilder final
{
public:
    static Scene createScene(const GameScenario scenario);

    static std::array<CardType, 3u> generateBooster(CardImage boosterCardImage);
};

class [[nodiscard]] TutorialScenarioBuilder final
    : public ScenarioBuilderInterface
{
public:
    int getInfectionLimit() const noexcept override;

    std::list<Card> generateStartRoom() override;

    std::list<Card> generateRoomDeck(const int linkID) override;
};

class [[nodiscard]] EasyScenarioBuilder final : public ScenarioBuilderInterface
{
public:
    int getInfectionLimit() const noexcept override;

    std::list<Card> generateStartRoom() override;

    std::list<Card> generateRoomDeck(const int linkID) override;
};

class [[nodiscard]] NormalScenarioBuilder final
    : public ScenarioBuilderInterface
{
public:
    int getInfectionLimit() const noexcept override;

    std::list<Card> generateStartRoom() override;

    std::list<Card> generateRoomDeck(const int linkID) override;
};

class [[nodiscard]] HardScenarioBuilder final : public ScenarioBuilderInterface
{
public:
    int getInfectionLimit() const noexcept override;

    std::list<Card> generateStartRoom() override;

    std::list<Card> generateRoomDeck(const int linkID) override;
};

class [[nodiscard]] NightmareScenarioBuilder final
    : public ScenarioBuilderInterface
{
public:
    int getInfectionLimit() const noexcept override;

    std::list<Card> generateStartRoom() override;

    std::list<Card> generateRoomDeck(const int linkID) override;
};
