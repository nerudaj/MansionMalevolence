#include "misc/DependencyContainer.hpp"
#include "settings/AppSettings.hpp"
#include <DGM/dgm.hpp>

class [[nodiscard]] AppStateMainMenu final : public dgm::AppState
{
public:
    AppStateMainMenu(
        dgm::App& app,
        DependencyContainer& dic,
        AppSettings& settings) noexcept;

public:
    void input() override;
    void update() override;
    void draw() override;

private:
    void restoreFocusImpl(const std::string& message = "") override;

private:
    void buildLayout();

    void onPlay();

    void onOptions();

    void onExit();

private:
    DependencyContainer& dic;
    AppSettings& settings;
};
