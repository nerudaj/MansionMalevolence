#include "gui/Sizers.hpp"

constexpr float CONTAINER_PADDING_MULTIPLIER = 2.55f;

#ifdef ANDROID

#include <jni/Jni.hpp>

constexpr float ANDROID_BASE_UNSCALED_FONT_SIZE = 14;

/**
 * Lazy-initialized base-size provider for Android.
 */
struct [[nodiscard]] BaseSizeProviderSingleton final
{
private:
    BaseSizeProviderSingleton()
    {
        auto core = jni::Core::attachCurrentThread();
        auto resources = jni::Resources::getSystem(core.getEnv());
        pixelDensity = resources.getDisplayMetrics().getDensity();
    }

public:
    BaseSizeProviderSingleton(BaseSizeProviderSingleton&&) = delete;
    BaseSizeProviderSingleton(const BaseSizeProviderSingleton&) = delete;

public:
    static BaseSizeProviderSingleton& getInstance()
    {
        static BaseSizeProviderSingleton instance;
        return instance;
    }

    [[nodiscard]] unsigned getBaseFontSize() const noexcept
    {
        return spToPx(ANDROID_BASE_UNSCALED_FONT_SIZE, pixelDensity);
    }

    [[nodiscard]] unsigned getBaseContainerHeight() const noexcept
    {
        return spToPx(
            static_cast<unsigned>(
                ANDROID_BASE_UNSCALED_FONT_SIZE * CONTAINER_PADDING_MULTIPLIER),
            pixelDensity);
    }

private:
    static unsigned spToPx(unsigned sp, float density)
    {
        return static_cast<unsigned>(sp * density);
    }

private:
    float pixelDensity = 0.f;
};
#else
#include <Windows.h>
#endif

unsigned Sizer::getBaseContainerHeight() const
{
#ifdef ANDROID
    return BaseSizeProviderSingleton::getInstance().getBaseContainerHeight()
           * settings.uiScale;
#else
    const unsigned dpi = GetDpiForSystem();
    return static_cast<unsigned>(
        (GetSystemMetricsForDpi(SM_CYCAPTION, dpi)
         + GetSystemMetricsForDpi(SM_CYSIZEFRAME, dpi)
         + GetSystemMetricsForDpi(SM_CYEDGE, dpi) * 2)
        * settings.uiScale);
#endif
}

unsigned Sizer::getBaseFontSize() const
{
#ifdef ANDROID
    return BaseSizeProviderSingleton::getInstance().getBaseFontSize()
           * settings.uiScale;
#else
    return static_cast<unsigned>(
        getBaseContainerHeight() / CONTAINER_PADDING_MULTIPLIER
        * settings.uiScale);
#endif
}
