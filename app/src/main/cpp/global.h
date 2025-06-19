//
// Created by TheChuan1503 on 2025/4/24.
//

#ifndef CIRCLOR2_GLOBAL_H
#define CIRCLOR2_GLOBAL_H

#include <jni.h>
#include <android/log.h>
#include <string>
#include "mc/LocalPlayer.h"
#include "mc/ClientInstance.h"
#include "mc/GameMode.h"
#include <map>

extern JavaVM *jvm;
extern jclass JNI;
extern std::string MC_VERSION;
extern std::map<std::string, long> mcFunctions;
extern std::map<std::string, long> mcBaseOffset;
extern std::map<std::string, double> circlor2FunctionsSave;
extern std::map<std::string, std::string> circlor2FunctionsStringSave;
extern bool isFirstHook;
extern std::vector<void*> hookStubs;

extern bool mc_isInGame;
extern unsigned char mc_gameMode_destroyBlock_flag;

extern ClientInstance *mc_clientInstance;
extern GameMode *mc_gameMode;
extern LocalPlayer *mc_localPlayer;

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)

void *getMinecraftFunction(char *name);
uintptr_t getMinecraftFunction(std::string name);
void *getMinecraftHandle();
bool isMinecraftVersion1_16();
uintptr_t getMinecraftBase();
void initGlobal();
std::string getMangledName(const char *name);
std::string getMangledName(std::string aName);
long getOffset(const char *name);
uintptr_t getBaseOffset();
uintptr_t getMCFuncAddr(const std::string& name);
void *getMCFuncPtr(const std::string& name);

#endif //CIRCLOR2_GLOBAL_H