//
// Created by TheChuan1503 on 2025/4/26.
//

#include <string>
#include <dlfcn.h>
#include "GuiData.h"

#define TAG "Native/objects/GuiData"

#include "../global.h"

typedef void* (*displayClientMessage_t)(GuiData*, const std::string&);

void GuiData::displayClientMessage(const std::string &message) {

    void *handle = dlopen("libminecraftpe.so", RTLD_LAZY);
    dlerror();

    displayClientMessage_t _displayClientMessage = (displayClientMessage_t) dlsym(handle, "_ZN7GuiData20displayClientMessageERKNSt6__ndk112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE");
    _displayClientMessage(this, message);

    LOGD("displayClientMessage: %s", message.c_str());
}
