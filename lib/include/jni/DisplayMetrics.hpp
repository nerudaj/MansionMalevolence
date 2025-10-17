#pragma once

#ifdef ANDROID

#include <android/native_activity.h>

namespace jni
{
class Resources;

class [[nodiscard]] DisplayMetrics final
{
private:
    DisplayMetrics(JNIEnv* env, jobject displayMetrics) 
        : env(env)
        , displayMetrics(displayMetrics)
    {
        jclass metricsClass = env->FindClass("android/util/DisplayMetrics");
        densityField = env->GetFieldID(metricsClass, "density", "F");
    }

    friend class Resources;

public:
    [[nodiscard]] float getDensity() const
    {
        return env->GetFloatField(displayMetrics, densityField);
    }

private:
    JNIEnv* env;
    jobject displayMetrics;
    jfieldID densityField;
};

}

#endif
