//
// Created by TheChuan1503 on 2025/4/26.
//

#include <cstddef>
#include <string>
#include <bytehook.h>
#include <shadowhook.h>
#include "Gloss.h"
#include "Circlor2.h"

#define TAG "Native/Circlor2"
#include "global.h"

void Circlor2::hook(const char *name, void *hook) {
    if (((std::string )(name)).find("::") != std::string::npos) {
        name = getMangledName(name).c_str();
    }
    bytehook_stub_t rez = bytehook_hook_single(
            "libminecraftpe.so",
            nullptr,
            name,
            hook,
            nullptr,
            nullptr
    );
    if (!isFirstHook) return;
    if (rez) {
        LOGD("ByteHook: Hooked %s", name);
    } else {
        LOGE("ByteHook: Failed to hook %s", name);
    }
}
void Circlor2::hookAndOverride(const char *name, void *hook) {
    if (((std::string )(name)).find("::") != std::string::npos) {
        name = getMangledName(name).c_str();
    }
    bytehook_stub_t rez = bytehook_hook_single(
            "libminecraftpe.so",
            nullptr,
            name,
            reinterpret_cast<void *>(hook),
            nullptr,
            nullptr
    );
    if (!isFirstHook) return;
    if (rez) {
        LOGD("ByteHook: Hooked %s", name);
    } else {
        LOGE("ByteHook: Failed to hook %s", name);
    }
}

void Circlor2::hook(long target, void *hook) {
    Circlor2::hook(target, hook, nullptr);
}

void Circlor2::hook(long target, void *hook, void **original) {
    void *targetAddr = (void *)((uintptr_t)getMinecraftBase() + target);
    void *stub = shadowhook_hook_func_addr(targetAddr, hook, original);// GlossHook(targetAddr, hook, original);
    if (!isFirstHook) return;
    if (stub) {
        LOGD("Hook: Hooked %p", targetAddr);
        hookStubs.push_back(stub);
    } else {
        LOGE("Hook: Failed to hook %p", targetAddr);
    }
}

double Circlor2::getFunctionValue(const char *path) {
    if (circlor2FunctionsSave.find(path) != circlor2FunctionsSave.end()) {
        return circlor2FunctionsSave[path];
    }
    return 0;
}

std::string Circlor2::getFunctionStringValue(const char *path) {
    if (circlor2FunctionsStringSave.find(path)!= circlor2FunctionsStringSave.end()) {
        return circlor2FunctionsStringSave[path];
    }
    return "";
}

void Circlor2::setFunctionValue(const char *path, double value) {
    circlor2FunctionsSave[path] = value;
    LOGD("Set %s to %f", path, value);
}

void Circlor2::setFunctionStringValue(const char *path, const char *value) {
    circlor2FunctionsStringSave[path] = value;
    LOGD("Set %s to %s", path, value);
}
