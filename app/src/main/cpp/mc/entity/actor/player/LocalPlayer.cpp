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

}

void LocalPlayer::setPos(const Vec3 *position) {
    setPos_t setPos = (setPos_t) getMCFuncPtr("LocalPlayer::setPos");
    if (setPos) {
        setPos(this, position);
    }
}
