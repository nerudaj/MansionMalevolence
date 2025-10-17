#pragma once

#include "input/InputDetector.hpp"
#include "misc/DependencyContainer.hpp"
#include "settings/AppSettings.hpp"
#include "strings/HwInputToStringMapper.hpp"
#include <DGM/dgm.hpp>

class [[nodiscard]] AppStateOptions final : public dgm::AppState
{
public:
    AppStateOptions(
        dgm::App& app,
        DependencyContainer& dic,
        AppSettings& settings) noexcept;

public:
    void input() override;
    void update() override;
    void draw() override;

private:
    void buildLayout();

    // Rebuilds layout if there is an appropriate change, such as ui scale or
    // resolution change
    void refresh();

    void onVideoTabSelected(tgui::Container::Ptr content);

    void onAudioTabSelected(tgui::Container::Ptr content);

    void onInputTabSelected(tgui::Container::Ptr content);

    void onBindingsTabSelected(tgui::Container::Ptr content);

    void onBack();

    void onResolutionSelected(const sf::Vector2u& resolution);

    template<class BindType>
#ifndef ANDROID
        requires std::is_same_v<BindType, KmbBinding>
                 || std::is_same_v<BindType, GamepadBinding>
#endif
    void onInputDetected(
        InputKind action,
        BindType newBinding,
        std::map<InputKind, Binding>& target)
    {
        std::get<BindType>(target[action]) = newBinding;
        auto btn = dic.gui.get<tgui::Button>(getBindButtonId<BindType>(action));
        btn->setText(HwInputToStringMapper {}.getMapping(newBinding));
        dic.input.updateBindings(settings.bindings);
        dic.input.forceRelease(action);
    }

    template<class BindType>
#ifndef ANDROID
        requires std::is_same_v<BindType, KmbBinding>
                 || std::is_same_v<BindType, GamepadBinding>
#endif
    static std::string getBindButtonId(InputKind action)
    {
        if constexpr (std::is_same_v<BindType, KmbBinding>)
            return uni::format("BindButton_{}_KMB", std::to_underlying(action));
        else
            return uni::format("BindButton_{}_GMP", std::to_underlying(action));
    }

private:
    DependencyContainer& dic;
    AppSettings& settings;
};
