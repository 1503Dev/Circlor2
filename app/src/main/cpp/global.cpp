//
// Created by TheChuan1503 on 2025/4/26.
//
#include <dlfcn.h>
#include <bytehook.h>
#include <shadowhook.h>

#define TAG "Native/Global"
#include "global.h"
JavaVM *jvm = nullptr;
jclass JNI = nullptr;
std::string MC_VERSION;
std::map<std::string, std::string> mcFunctions;
std::map<std::string, std::map<std::string, long>> mcFunctionsOffset;
std::map<std::string, long> mcBaseOffset;
std::map<std::string, double> circlor2FunctionsSave;
std::map<std::string, std::string> circlor2FunctionsStringSave;
bool isFirstHook = true;
std::vector<void*> hookStubs = {};

ClientInstance *mc_clientInstance = nullptr;
GameMode *mc_gameMode = nullptr;
LocalPlayer *mc_localPlayer = nullptr;

bool mc_isInGame = false;
unsigned char mc_gameMode_destroyBlock_flag = 0;

void *getMinecraftFunction(char *name) {
    if (((std::string)name).find("::") != std::string::npos) {
        name = (char *)getMangledName(name).c_str();
    }
    void *handle = dlopen("libminecraftpe.so", RTLD_LAZY | RTLD_NOLOAD);
    if (!handle) {
        return nullptr;
    }
    void *result;
    void *symbol = dlsym(handle, name);
    result = symbol;
    dlclose(handle);
    return result;
}
uintptr_t getMinecraftFunction(std::string name) {
    if (name.find("::") != std::string::npos) {
        name = getMangledName(name);
    }
    void *handle = dlopen("libminecraftpe.so", RTLD_LAZY | RTLD_NOLOAD);
    if (!handle) {
        return 0;
    }
    uintptr_t result;
    void *symbol = dlsym(handle, name.c_str());
    result = (uintptr_t) symbol;
    dlclose(handle);
    return result;
}
static void *s_minecraft_handle = nullptr;
void *getMinecraftHandle() {
    if (!s_minecraft_handle) {
        s_minecraft_handle = dlopen("libminecraftpe.so", RTLD_NOLOAD);
    }
    return s_minecraft_handle;
}

bool isMinecraftVersion1_16() {
    if (MC_VERSION.empty()) {
        return false;
    }
    if (MC_VERSION == "1.16.0.2" || MC_VERSION == "1.16.1.02" || MC_VERSION == "1.16.10.02" || MC_VERSION == "1.16.20.03" || MC_VERSION == "1.16.40.02" || MC_VERSION == "1.16.100.04" || MC_VERSION == "1.16.101.01") {
        return true;
    }
    return false;
}
static unsigned long s_minecraft_base;
uintptr_t getMinecraftBase() {
    if (!s_minecraft_base) {
        FILE *maps = fopen("/proc/self/maps", "r");
        char line[512];
        while (fgets(line, sizeof(line), maps)) {
            if (strstr(line, "libminecraftpe.so")) {
                s_minecraft_base = (uintptr_t)strtoul(strtok(line, "-"), NULL, 16);
            }
        }
        fclose(maps);
    }
    return s_minecraft_base;
}

std::string getMangledName(const char *name){
    if (mcFunctions.find(name) != mcFunctions.end()) {
        std::string rez = mcFunctions[name];
        LOGD("getMangledName: %s -> %s", name, rez.c_str());
        return rez;
    }
    LOGE("getMangledName: %s -> not found", name);
    return "";
}
std::string getMangledName(std::string aName){
    char *name = (char *)aName.c_str();
    if (mcFunctions.find(name) != mcFunctions.end()) {
        std::string rez = mcFunctions[name];
        LOGD("getMangledName: %s -> %s", name, rez.c_str());
        return rez;
    }
    LOGE("getMangledName: %s -> not found", name);
    return "";
}

long getOffset(const char *name) {
    if (mcFunctionsOffset.find(name)!= mcFunctionsOffset.end()) {
        if (mcFunctionsOffset[name].find(MC_VERSION)!= mcFunctionsOffset[name].end()) {
            LOGD("getOffset: %s", name);
            return mcFunctionsOffset[name][MC_VERSION] - getBaseOffset();
        }
    }
    return 0;
}
static uintptr_t s_base_offset;
uintptr_t getBaseOffset() {
    if (s_base_offset) {
        return s_base_offset;
    }
    if (mcFunctionsOffset["JNI_OnLoad"].find(MC_VERSION) != mcFunctionsOffset["JNI_OnLoad"].end()) {
        s_base_offset = getMinecraftBase() + mcFunctionsOffset["JNI_OnLoad"][MC_VERSION] - getMinecraftFunction("JNI_OnLoad");
        LOGD("MC base offset: %lx", s_base_offset);
    }
    return s_base_offset;
}
uintptr_t getMCFuncAddr(const std::string& name) {
    int offset = getOffset(name.c_str());
    LOGD("getMCFuncAddr: %s", name.c_str());
    return getMinecraftBase() + offset;
}
void *getMCFuncPtr(const std::string& name) {
    return reinterpret_cast<void *>(getMCFuncAddr(name));
}

void initGlobal() {
    isFirstHook = true;

    // JNI_OnLoad (JavaVM *vm, void *reserved)
    mcFunctions["JNI_OnLoad"] = "JNI_OnLoad";
    mcFunctionsOffset["JNI_OnLoad"]["1.21.1.03"] = 0xC6ED88C;
    mcFunctionsOffset["JNI_OnLoad"]["1.21.2.02"] = 0xC6F7C9C;
    mcFunctionsOffset["JNI_OnLoad"]["1.21.51.02"] = 0xCCAA658;

    // 设置坐标 (Vec3 const& pos)
    mcFunctions["Actor::setPos"] = "_ZN5Actor6setPosERK4Vec3";
    mcFunctionsOffset["Actor::setPos"]["1.21.1.03"] = 0xAAF1F84;
    mcFunctionsOffset["Actor::setPos"]["1.21.2.02"] = 0xAAF6514;
    mcFunctionsOffset["Actor::setPos"]["1.21.51.02"] = 0xC420F34;

    mcFunctionsOffset["Actor::setPosDirectLegacy"]["1.21.51.02"] = 0xAFA7818;

    // 传送 (Vec3 const& pos, bool shouldStopRiding, int cause, int sourceEntityType, bool keepVelocity)
    mcFunctionsOffset["Actor::teleportTo"]["1.21.51.02"] = 0xAFA7754;

    // 开始退出存档 (void)
    mcFunctions["ClientInstance::_startLeaveGame"] = "_ZN14ClientInstance15_startLeaveGameEv";
    mcFunctionsOffset["ClientInstance::_startLeaveGame"]["1.16.100.04"] = 0x3DFE480;

    // (Actor &)
    mcFunctions["GameMode::attack"] = " _ZN8GameMode6attackER5Actor";
    mcFunctionsOffset["GameMode::attack"]["1.16.100.04"] = 0x6253DB4;
    mcFunctionsOffset["GameMode::attack"]["1.16.221.01"] = 0x21CD668;

    mcFunctionsOffset["GameMode::destroyBlock"]["1.21.2.02"] = 0xA722944;

    // 退出存档 (void)
    mcFunctionsOffset["Level::startLeaveGame"]["1.21.1.03"] = 0xB14CB08;
    mcFunctionsOffset["Level::startLeaveGame"]["1.21.2.02"] = 0xB156A50;

    // (int)
    mcFunctions["LocalPlayer::addExperience"] = "_ZN11LocalPlayer13addExperienceEi";
    mcFunctionsOffset["LocalPlayer::addExperience"]["1.16.100.04"] = 0x5ABC848;

    // 增加经验等级(int)
    mcFunctions["LocalPlayer::addLevels"] = "_ZN11LocalPlayer9addLevelsEi";
    mcFunctionsOffset["LocalPlayer::addLevels"]["1.16.100.04"] = 0x5ABC884;
    // mcFunctionsOffset["LocalPlayer::addLevels"]["1.21.1.03"] = 0x0;

    //本地玩家获取触摸距离 (void)float
    mcFunctions["LocalPlayer::getPickRange"] = "_ZNK11LocalPlayer12getPickRangeEv";
    mcFunctionsOffset["LocalPlayer::getPickRange"]["1.16.100.04"] = 0x5ABCD88;

    // 本地玩家移动 (IActorMovementProxy &,Vec3 const&)
    mcFunctions["LocalPlayer::move"] = "_ZNK11LocalPlayer4moveER19IActorMovementProxyRK4Vec3";
    mcFunctionsOffset["LocalPlayer::move"]["1.16.100.04"] = 0x5AB6408;

    // 本地玩家设置坐标 (Vec3 const&)
    mcFunctions["LocalPlayer::setPos"] = "_ZN11LocalPlayer6setPosERK4Vec3";
    mcFunctionsOffset["LocalPlayer::setPos"]["1.16.100.04"] = 0x5AB8054;
    mcFunctionsOffset["LocalPlayer::setPos"]["1.16.221.01"] = 0x2937464;
    // mcFunctionsOffset["LocalPlayer::setPos"]["1.21.51.02"] = 0x68D2474;

    // (bool)
    mcFunctions["LocalPlayer::swing"] = "_ZN11LocalPlayer18applyFinalFrictionEfb";
    mcFunctionsOffset["LocalPlayer::swing"]["1.16.100.04"] = 0x5A980C8;

    // (void)
    mcFunctions["LocalPlayer::神秘初始化函数"] = "我看着呢";
    mcFunctionsOffset["LocalPlayer::神秘初始化函数"]["1.21.1.03"] = 0x66048F0;
    mcFunctionsOffset["LocalPlayer::神秘初始化函数"]["1.21.2.02"] = 0xAB422D0;
    mcFunctionsOffset["LocalPlayer::神秘初始化函数"]["1.21.51.02"] = 0xAFA7758;

    // 开始退出存档(一次性) (IClientInstance *, unsigned int)
    mcFunctions["MinecraftGame::onClientLevelExit"] = "_ZN13MinecraftGame17onClientLevelExitER15IClientInstancej";
    mcFunctionsOffset["MinecraftGame::onClientLevelExit"]["1.16.100.04"] = 0x54C8FF0;
    mcFunctionsOffset["MinecraftGame::onClientLevelExit"]["1.21.1.03"] = 0x61ECE70;
    mcFunctionsOffset["MinecraftGame::onClientLevelExit"]["1.21.51.02"] = 0x63D2C28;

    // 开始退出存档 (void)
    mcFunctions["MinecraftGame::startLeaveGame"] = "_ZN13MinecraftGame14startLeaveGameEv";
    mcFunctionsOffset["MinecraftGame::startLeaveGame"]["1.16.100.04"] = 0x54D2A8C;

    // 进入游戏 (void)
    mcFunctions["MinecraftGame::update"] = "_ZN13MinecraftGame6updateEv";
    mcFunctionsOffset["MinecraftGame::update"]["1.16.100.04"] = 0x54B9A68;

    // 玩家死亡 (ActorDamageSource const&)
    mcFunctions["Player::die"] = "_ZN6Player3dieERK17ActorDamageSource";
    mcFunctionsOffset["Player::die"]["1.21.1.03"] = 0xA84C82C;

    mcFunctions["Player::teleportTo"] = "_ZN6Player10teleportToERK4Vec3biib";
}