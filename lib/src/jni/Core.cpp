#ifdef ANDROID

#include <jni/Core.hpp>

jni::Core jni::Core::attachCurrentThread()
{
    auto& activity = *sf::getNativeActivity();

    JavaVM& vm  = *activity.vm;
    JNIEnv* env = activity.env;

    JavaVMAttachArgs lJavaVMAttachArgs;
    lJavaVMAttachArgs.version = JNI_VERSION_1_6;
    lJavaVMAttachArgs.name = "NativeThread";
    lJavaVMAttachArgs.group = nullptr;

    jint lResult = vm.AttachCurrentThread(
        &env, &lJavaVMAttachArgs);

    if (lResult == JNI_ERR)
        throw std::runtime_error("JNI: Could not attach current thread!");

    return Core(&vm, env);
}

#endif
