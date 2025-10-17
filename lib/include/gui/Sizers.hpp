#pragma once

#include "settings/VideoSettings.hpp"

class [[nodiscard]] Sizer final
{
public:
    constexpr explicit Sizer(const VideoSettings& settings) noexcept
        : settings(settings)
    {
    }

    Sizer(Sizer&&) = default;
    Sizer(const Sizer&) = delete;

public:
    [[nodiscard]] unsigned getBaseFontSize() const;

    [[nodiscard]] unsigned getBaseContainerHeight() const;

private:
    const VideoSettings& settings;
};
