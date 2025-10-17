#pragma once

#ifdef ANDROID

#include <jni/DisplayMetrics.hpp>

#include <android/native_activity.h>

namespace jni
{

class [[nodiscard]] Resources final
{
private:
    Resources(
        JNIEnv* env,
        jobject resources,
        jmethodID getDisplayMetricsMethod)
        : env(env)
        , resources(resources)
        , getDisplayMetricsMethod(getDisplayMetricsMethod)
    {
    }

public:
    static Resources getSystem(JNIEnv* env);

    DisplayMetrics getDisplayMetrics() const
    {
        return DisplayMetrics(
            env,
            env->CallObjectMethod(resources, getDisplayMetricsMethod));
    }

private:
    JNIEnv* env;
    jobject resources;
    jmethodID getDisplayMetricsMethod;
};

}

#endif
