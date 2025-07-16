//
// Created by TheChuan1503 on 2025/4/26.
//
#include <dlfcn.h>
#include <bytehook.h>
#include <shadowhook.h>

#define TAG "Native/Global"
#include "global.h"
JavaVM *jvm = nullptr;
std::map<std::string, long> mcFunctions;
std::map<std::string, double> circlor2FunctionsSave;
std::map<std::string, std::string> circlor2FunctionsStringSave;
std::vector<void*> hookStubs = {};

ClientInstance *clientInstance = nullptr;

uintptr_t getMinecraftFunction(std::string name) {
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

long getOffset(const char *name) {
    if (mcFunctions.find(name)!= mcFunctions.end()) {
        return mcFunctions[name] - getBaseOffset();
    }
    return 0;
}
static uintptr_t s_base_offset;
uintptr_t getBaseOffset() {
    if (s_base_offset) {
        return s_base_offset;
    }
    s_base_offset = getMinecraftBase() + mcFunctions["JNI_OnLoad"] - getMinecraftFunction("JNI_OnLoad");
    return s_base_offset;
}
uintptr_t getMCFuncAddr(const std::string& name) {
    int offset = getOffset(name.c_str());
    return getMinecraftBase() + offset;
}
void *getMCFuncPtr(const std::string& name) {
    return reinterpret_cast<void *>(getMCFuncAddr(name));
}

bool isInGame() {
    if (clientInstance == nullptr) {
        return false;
    }
    return clientInstance->isInGame();
}