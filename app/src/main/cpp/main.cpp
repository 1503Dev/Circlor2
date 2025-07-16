#include <jni.h>
#include <string>
#include <future>
#include <chrono>
#include <dlfcn.h>
#include <android/log.h>
#include <android/native_activity.h>
#include <sys/mman.h>
#include <unistd.h>
#include <mutex>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "Circlor.h"


#include "mc/client/gui/GuiData.h"
#include "mc/client/ClientInstance.h"
#include "mc/gamemode/GameMode.h"
#include "mc/entity/actor/Actor.h"
#include "mc/client/MinecraftGame.h"
#include "mc/entity/actor/player/Player.h"
#include "mc/entity/actor/ActorDamageSource.h"

#define TAG "Native/main"
#include "global.h"
#include "HookManager.h"
#include <mutex>
static std::mutex hook_mutex;

static void (*android_main_minecraft)(struct android_app *app);
static void (*ANativeActivity_onCreate_minecraft)(ANativeActivity *activity, void *savedState, size_t savedStateSize);

static void initHook();
static void initFunctionTable(JNIEnv*);
static void setFunctionTable(JNIEnv *env, jobjectArray function_name,
                             jlongArray function_address);
static void initHookLib(JNIEnv*);

void initHook() {
    hookStubs.clear();
    HookManager::init();
}

extern "C" void ANativeActivity_onCreate(ANativeActivity *activity, void *savedState, size_t savedStateSize) {
    LOGD("+ANativeActivity_onCreate+");
    LOGD("minecraftpe.so: %p; %lx", getMinecraftHandle(), getMinecraftBase());
    initHook();
    ANativeActivity_onCreate_minecraft(activity, savedState, savedStateSize);
    LOGD("-ANativeActivity_onCreate-");
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    LOGD("+JNI_OnLoad+");
    jvm = vm;
    void *handle = getMinecraftHandle();
    android_main_minecraft = (void (*)(struct android_app *)) (dlsym(handle, "android_main"));
    ANativeActivity_onCreate_minecraft = (void (*)(ANativeActivity *, void *, size_t)) (dlsym(handle, "ANativeActivity_onCreate"));
    JNIEnv *env;
    vm->GetEnv((void **) &env, JNI_VERSION_1_6);
    initFunctionTable(env);
    LOGD("-JNI_OnLoad-");
    return JNI_VERSION_1_6;
}
JNIEXPORT jint android_main(struct android_app *state) {
    LOGD("+android_main+");
    android_main_minecraft(state);
    LOGD("-android_main-");
    return JNI_VERSION_1_6;
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_dev1503_circlor_JNI_isMinecraftFunctionExists(JNIEnv *env, jclass clazz, jstring name) {
    const char *functionName = env->GetStringUTFChars(name, nullptr);
    if (functionName == nullptr) {
        return JNI_FALSE;
    }

    void* handle = dlopen("libminecraftpe.so", RTLD_LAZY | RTLD_NOLOAD);
    if (!handle) {
        return JNI_FALSE;
    }

    jboolean result = JNI_FALSE;
    void *symbol = dlsym(handle, functionName);
    if (symbol) {
        result = JNI_TRUE;
    }
    dlclose(handle);

    env->ReleaseStringUTFChars(name, functionName);
    return result;
}
extern "C"
JNIEXPORT jstring JNICALL
Java_dev1503_circlor_JNI_invokeFunction(JNIEnv *env, jclass clazz, jstring path) {
    std::string funcPath = env->GetStringUTFChars(path, nullptr);
    return env->NewStringUTF(Circlor::invoke(funcPath).c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_dev1503_circlor_JNI_setMinecraftVersion(JNIEnv *env, jclass clazz, jstring version) {
//    MC_VERSION = env->GetStringUTFChars(version, nullptr);
}
extern "C"
JNIEXPORT void JNICALL
Java_dev1503_circlor_JNI_setAssetManager(JNIEnv *env, jclass clazz, jobject assetManagerObj) {
    // assetManager = AAssetManager_fromJava(env, assetManagerObj);
    // drawText("114514",100,100);
}
extern "C"
JNIEXPORT jdouble JNICALL
Java_dev1503_circlor_JNI_getFunctionValue(JNIEnv *env, jclass clazz, jstring path) {
    return Circlor::getFunctionValue(env->GetStringUTFChars(path, nullptr));
}
extern "C"
JNIEXPORT jstring JNICALL
Java_dev1503_circlor_JNI_getFunctionValueString(JNIEnv *env, jclass clazz, jstring path) {
    return env->NewStringUTF(Circlor::getFunctionStringValue(env->GetStringUTFChars(path, nullptr)).c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_dev1503_circlor_JNI_setFunctionValue(JNIEnv *env, jclass clazz, jstring path, jdouble value) {
    Circlor::setFunctionValue(env->GetStringUTFChars(path, nullptr), value);
}
extern "C"
JNIEXPORT void JNICALL
Java_dev1503_circlor_JNI_setFunctionValueString(JNIEnv *env, jclass clazz, jstring path,
                                                 jstring value) {
    Circlor::setFunctionStringValue(env->GetStringUTFChars(path, nullptr),
                                    env->GetStringUTFChars(value, nullptr));
}


static void setFunctionTable(JNIEnv *env, jobjectArray function_name,
                                          jlongArray function_address) {
    jsize size = env->GetArrayLength(function_name);

    // 获取 long 数组指针
    jlong* valuesPtr = env->GetLongArrayElements(function_address, nullptr);

    for (jsize i = 0; i < size; i++) {
        // 获取 key
        jstring key = (jstring)env->GetObjectArrayElement(function_name, i);
        const char* keyStr = env->GetStringUTFChars(key, nullptr);

        // 添加到 map
        mcFunctions[keyStr] = valuesPtr[i];

        LOGD("%s: %lx", keyStr, valuesPtr[i]);

        // 释放资源
        env->ReleaseStringUTFChars(key, keyStr);
        env->DeleteLocalRef(key);

    }

    // 释放数组
    env->ReleaseLongArrayElements(function_address, valuesPtr, 0);

    LOGD("Function table set success");
}

static void initFunctionTable(JNIEnv *env) {
    LOGD("initFunctionTable");
    jclass jniClass = env->FindClass("dev1503/circlor/JNI");
    if (jniClass == nullptr) {
        LOGE("JNI class not found");
        return;
    }
    jmethodID getFunctionNames = env->GetStaticMethodID(jniClass, "getFunctionNames", "()[Ljava/lang/String;");
    if (getFunctionNames == nullptr) {
        LOGE("getFunctionNames method not found");
        return;
    }
    jobjectArray functionNames = (jobjectArray)env->CallStaticObjectMethod(jniClass, getFunctionNames);

    jmethodID getFunctionAddresses = env->GetStaticMethodID(jniClass, "getFunctionAddresses", "()[J");
    if (getFunctionAddresses == nullptr) {
        LOGE("getFunctionAddresses method not found");
        return;
    }
    jlongArray functionAddresses = (jlongArray)env->CallStaticObjectMethod(jniClass, getFunctionAddresses);

    setFunctionTable(env, functionNames, functionAddresses);
}