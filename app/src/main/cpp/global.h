//
// Created by TheChuan1503 on 2025/4/24.
//

#ifndef CIRCLOR2_GLOBAL_H
#define CIRCLOR2_GLOBAL_H

#include <jni.h>
#include <android/log.h>
#include <string>
#include "mc/client/ClientInstance.h"
#include <map>

extern JavaVM *jvm;
extern std::string MC_VERSION;
extern std::map<std::string, long> mcFunctions;
extern std::map<std::string, double> circlor2FunctionsSave;
extern std::map<std::string, std::string> circlor2FunctionsStringSave;
extern bool isFirstHook;
extern std::vector<void*> hookStubs;

extern ClientInstance *clientInstance;

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

uintptr_t getMinecraftFunction(std::string name);
void *getMinecraftHandle();
uintptr_t getMinecraftBase();
long getOffset(const char *name);
uintptr_t getBaseOffset();
uintptr_t getMCFuncAddr(const std::string& name);
void *getMCFuncPtr(const std::string& name);

bool isInGame();

#endif //CIRCLOR2_GLOBAL_H