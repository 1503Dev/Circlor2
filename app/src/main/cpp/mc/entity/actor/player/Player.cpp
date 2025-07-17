//
// Created by TheChuan1503 on 2025/5/2.
//

#ifndef GLOBAL_H
#define TAG "Native/Player"
#include "../../../../global.h"
#endif

#include "Player.h"
#include "../../../math/Vec3.h"

GameType Player::getPlayerGameType() {
    getPlayerGameType_t getPlayerGameType = (getPlayerGameType_t) getMCFuncPtr("Player::getPlayerGameType");
    if (getPlayerGameType) {
        return getPlayerGameType(this);
    }
    return GameType::Survival;
}
void Player::teleportTo(Vec3& vec3, bool bool1, int int1, int int2, bool bool2) {
    teleportTo_t teleportTo = (teleportTo_t) getMCFuncPtr("Player::teleportTo");
    if (teleportTo) {
        teleportTo(this, vec3, bool1, int1, int2, bool2);
    }
}
