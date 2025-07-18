//
// Created by TheChuan1503 on 2025/5/1.
//

#include "MinecraftGame.h"

#ifndef GLOBAL_H
#define TAG "Native/MinecraftGame"
#include "../global.h"
#endif

void *MinecraftGame::getInput() {
    getInput_t getInput = (getInput_t) getMCFuncPtr("MinecraftGame::getInput");
    if (getInput) {
        return getInput(this);
    }
    return nullptr;
}
