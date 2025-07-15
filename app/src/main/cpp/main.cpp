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
#include <shadowhook.h>
#include "bytehook.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "Circlor.h"


#include "mc/GuiData.h"
#include "mc/ClientInstance.h"
#include "mc/GameMode.h"
#include "mc/Actor.h"
#include "mc/MinecraftGame.h"
#include "mc/Player.h"
#include "mc/ActorDamageSource.h"

#define TAG "Native/main"
#include "global.h"
#include "structs.h"
#include <mutex>
static std::mutex hook_mutex;

static void (*android_main_minecraft)(struct android_app *app);
static void (*ANativeActivity_onCreate_minecraft)(ANativeActivity *activity, void *savedState, size_t savedStateSize);

static void initHook();

static Actor::setPos_t O_Actor_setPos = nullptr;
static GameMode::destroyBlock_t O_GameMode_destroyBlock = nullptr;
static void *O_Level_startLeaveGame = nullptr;
static void *O_MinecraftGame_onClientLevelExit = nullptr;
static void *O_Player_die = nullptr;

void H_Actor_setPos(Actor *actor, const Vec3& vec3) {
    LOGD("[G] Actor::setPos: %f, %f, %f", vec3.x, vec3.y, vec3.z);
    if (O_Actor_setPos) {
        O_Actor_setPos(actor, &vec3);
    }
    initHook();
}
void H_Actor_teleportTo(Vec3 const& pos, bool shouldStopRiding, int cause, int sourceEntityType, bool keepVelocity){
    LOGD("[G] Actor::teleportTo: %f, %f, %f", pos.x, pos.y, pos.z);
}
void H_ClientInstance__startLeaveGame(ClientInstance *clientInstance) {
    mc_isInGame = false;
    LOGD("[G] 离开存档 ClientInstance::_startLeaveGame");
    initHook();
}
void H_GameMode_attack(GameMode *gameMode, Actor *actor) {
    LOGD("[G] 攻击 Actor::attack");
}
void H_GameMode_destroyBlock(GameMode *gameMode, BlockPos *blockPos, unsigned char flag) {
    LOGD("[G] 破坏方块 GameMode::destroyBlock, flag: %x", flag);
    mc_gameMode = gameMode;
    mc_gameMode_destroyBlock_flag = flag;
    if (O_GameMode_destroyBlock) {
        O_GameMode_destroyBlock(gameMode, blockPos, flag);
    }
    initHook();
}
float H_LocalPlayer_getPickRange(LocalPlayer *localPlayer) {
    LOGD("[G] 获取触摸距离 LocalPlayer::getPickRange");
    initHook();
    return 100.0f;
}
void H_LocalPlayer_setPos(LocalPlayer *localPlayer, const Vec3& vec3) {
    mc_localPlayer = localPlayer;
    mc_isInGame = true;
    LOGD("[G] 初始化坐标(进入游戏): %f, %f, %f", vec3.x, vec3.y, vec3.z);
    initHook();
    // LOGD("range: %f", mc_localPlayer->getPickRange());
}
void H_MinecraftGame_onClientLevelExit(ClientInstance *clientInstance, unsigned int uInt){
    LOGD("[G] 离开存档 MinecraftGame::onClientLevelExit");
    mc_isInGame = false;
    if (O_MinecraftGame_onClientLevelExit) {
        ((void (*)(ClientInstance *, unsigned int)) O_MinecraftGame_onClientLevelExit)(clientInstance, uInt);
    }
    initHook();
}
void H_MinecraftGame_startLeaveGame(MinecraftGame *minecraftGame) {
    mc_isInGame = false;
    LOGD("[G] 离开存档 MinecraftGame::startLeaveGame");
    initHook();
}
void H_MinecraftGame_update(ClientInstance *clientInstance) {
    mc_clientInstance = clientInstance;
    LOGD("[G] 进入游戏 MinecraftGame::update");
    initHook();
}
void H_Player_die(Player *player, ActorDamageSource *damageSource) {
    LOGD("[G] 玩家死亡 Player::die");
    if (O_Player_die) {
        ((void (*)(Player *, ActorDamageSource *)) O_Player_die)(player, damageSource);
    }
    initHook();
}
void H_Player_remove(Player *player){
    LOGD("[G] 玩家移除 Player::remove");
    initHook();
}
void H_Level_startLeaveGame(void *level){
    LOGD("[G] 存档开始退出 Level::startLeaveGame");
    mc_isInGame = false;
    if (O_Level_startLeaveGame) {
        ((void (*)(void *)) O_Level_startLeaveGame)(level);
    } else {
        LOGD("O_Level_startLeaveGame is null");
    }
    initHook();
}
void H_LocalPlayer_神秘初始化函数(LocalPlayer *localPlayer){
    LOGD("[G] LocalPlayer 被神秘初始化函数初始化 (进入游戏)");
    mc_localPlayer = localPlayer;
    mc_isInGame = true;
    initHook();
}
void H_test(void *clazz){
    LOGD("test hook was called");
}
void H_test_FFF(void *clazz, float x, float y, float z){
    LOGD("test hook (FFF) was called: %f, %f, %f", x, y, z);
    mc_localPlayer = (LocalPlayer*)clazz;
}
void H_test_Vec3(void *clazz, Vec3 *vec3){
    LOGD("test hook (Vec3) was called: %f, %f, %f", vec3->x, vec3->y, vec3->z);
}

bool mcVersionIs(std::string version) {
    return MC_VERSION == version;
}

void initHook() {
    LOGD("Rehooking");
    /*std::lock_guard<std::mutex> lock(hook_mutex); // 加锁
    for (auto & hookStub : hookStubs) {
        if (hookStub) {
            bytehook_unhook(hookStub);
            hookStub = nullptr;
        }
    }*/
    hookStubs.clear();
//    Circlor::hook(getOffset("Actor::setPos"), (void*)&H_Actor_setPos, (void**)&O_Actor_setPos);
//    Circlor::hook(getOffset("Actor::teleportTo"), (void*)&H_Actor_teleportTo);
//    Circlor::hook(getOffset("GameMode::destroyBlock"), (void*)&H_GameMode_destroyBlock, (void**)&O_GameMode_destroyBlock);
//    Circlor2::hook(getOffset("Level::startLeaveGame"), (void*)&H_Level_startLeaveGame, (void**)&O_Level_startLeaveGame);
//    Circlor2::hook(getOffset("LocalPlayer::神秘初始化函数"), (void*)&H_LocalPlayer_神秘初始化函数);
//    Circlor::hook(getOffset("MinecraftGame::onClientLevelExit"), (void*)&H_MinecraftGame_onClientLevelExit, (void**)&O_MinecraftGame_onClientLevelExit);
//    Circlor2::hook(getOffset("Player::die"), (void*)&H_Player_die, (void**)&O_Player_die);

        // Circlor2::hook(0xA722944 - getBaseOffset(), (void*)&H_test);
    isFirstHook = false;
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
    initGlobal();
    void *handle = getMinecraftHandle();
    android_main_minecraft = (void (*)(struct android_app *)) (dlsym(handle, "android_main"));
    ANativeActivity_onCreate_minecraft = (void (*)(ANativeActivity *, void *, size_t)) (dlsym(handle, "ANativeActivity_onCreate"));
    LOGD("-JNI_OnLoad-");
    return JNI_VERSION_1_6;
}
JNIEXPORT jint android_main(struct android_app *state) {
    LOGD("+android_main+");
    android_main_minecraft(state);
    LOGD("-android_main-");
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
    if (funcPath == "exp_level_change") {
        if (mc_isInGame){
            if (mc_localPlayer) {
                mc_localPlayer->addLevels((int)Circlor::getFunctionValue("exp_level_change_value"));
            }
        } else {
            return env->NewStringUTF("not_in_game");
        }
    } else if (funcPath == "testcall") {
        if (mc_gameMode) {
            mc_gameMode->destroyBlock(BlockPos(0, 100, 0), mc_gameMode_destroyBlock_flag);
        }
        return env->NewStringUTF("testcalled");
    }
    return env->NewStringUTF("");
}
extern "C"
JNIEXPORT void JNICALL
Java_dev1503_circlor_JNI_setMinecraftVersion(JNIEnv *env, jclass clazz, jstring version) {
    MC_VERSION = env->GetStringUTFChars(version, nullptr);
}
extern "C"
JNIEXPORT void JNICALL
Java_dev1503_circlor_JNI_setAssetManager(JNIEnv *env, jclass clazz, jobject assetManagerObj) {
    // assetManager = AAssetManager_fromJava(env, assetManagerObj);
    // drawText("114514",100,100);
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_dev1503_circlor_JNI_isInGame(JNIEnv *env, jclass clazz) {
    return mc_isInGame;
}
extern "C"
JNIEXPORT void JNICALL
Java_dev1503_circlor_JNI_mc_1teleport(JNIEnv *env, jclass clazz, jfloat x, jfloat y, jfloat z) {
    y = y + 2;
    if (mc_isInGame) {
        /*if (mcVersionIs("1.21.51.02")) {
            mc_localPlayer->teleportTo(Vec3(x, y, z), true, 0, 0, false);
            return;
        }*/
        mc_localPlayer->setPos(x, y, z);
        mc_localPlayer->setPos(x, y, z);
    }
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