//
// Created by TheChuan1503 on 2025/5/2.
//

#ifndef GLOBAL_H
#define TAG "Native/Player"
#include "../../../../global.h"
#endif

#include "Player.h"
#include "../../../math/Vec3.h"

void Player::teleportTo(Vec3& vec3, bool bool1, int int1, int int2, bool bool2) {
    teleportTo_t func = reinterpret_cast<teleportTo_t>(getMCFuncPtr("Player::teleportTo"));
    if (func) {
        func(this, vec3, bool1, int1, int2, bool2);
        LOGD("teleportTo: %f, %f, %f", vec3.x, vec3.y, vec3.z);
    } else {
        LOGE("Failed to teleportTo: %f, %f, %f", vec3.x, vec3.y, vec3.z);
    }
}
