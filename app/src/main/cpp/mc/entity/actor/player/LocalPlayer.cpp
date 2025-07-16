//
// Created by TheChuan1503 on 2025/4/27.
//

#include "LocalPlayer.h"
#ifndef GLOBAL_H
#define TAG "Native/LocalPlayer"
#include "../../../../global.h"
#endif

float LocalPlayer::getPickRange() {
    return 0;
}

void LocalPlayer::addLevels(int levels) {
    addLevels_t addLevels = (addLevels_t) getMCFuncPtr("LocalPlayer::addLevels");
    if (addLevels) {
        addLevels(this, levels);
    }
}

void LocalPlayer::setPos(const Vec3 *position) {
    setPos_t setPos = (setPos_t) getMCFuncPtr("LocalPlayer::setPos");
    if (setPos) {
        setPos(this, position);
    }
}

void LocalPlayer::setPlayerGameType(GameType gameType) {
    setPlayerGameType_t setPlayerGameType = (setPlayerGameType_t) getMCFuncPtr("LocalPlayer::setPlayerGameType");
    if (setPlayerGameType) {
        setPlayerGameType(this, gameType);
    }
}
