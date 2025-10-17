#ifdef ANDROID

#include <jni/Resources.hpp>

jni::Resources jni::Resources::getSystem(JNIEnv* env)
{
    jclass resourcesClass = env->FindClass("android/content/res/Resources");
    jmethodID getSystemMethod = env->GetStaticMethodID(resourcesClass, "getSystem", "()Landroid/content/res/Resources;");
    jmethodID getDisplayMetricsMethod = env->GetMethodID(resourcesClass, "getDisplayMetrics", "()Landroid/util/DisplayMetrics;");
    jobject resources = env->CallStaticObjectMethod(resourcesClass, getSystemMethod);
    return Resources(env, resources, getDisplayMetricsMethod);
}

#endif
