//
// Created by TheChuan1503 on 2025/4/27.
//

#include "LocalPlayer.h"
#ifndef GLOBAL_H
#define TAG "Native/LocalPlayer"
#include "../global.h"
#endif

float LocalPlayer::getPickRange() {
    if (isMinecraftVersion1_16()) {
        getPickRange_t func = (getPickRange_t) getMinecraftFunction("LocalPlayer::getPickRange");
        if (func && mc_isInGame) {
            return func(this);
        }
    }
    return 0;
}

void LocalPlayer::addLevels(int levels) {
    if (isMinecraftVersion1_16()) {
        addLevels_t func = (addLevels_t) getMinecraftFunction("LocalPlayer::addLevels");
        if (func && mc_isInGame) {
            func(this, levels);
        }
    } else {
        addLevels_t func = (addLevels_t) getMCFuncAddr("LocalPlayer::addLevels");
        if (func && mc_isInGame) {
            func(this, levels);
        }
    }
}
