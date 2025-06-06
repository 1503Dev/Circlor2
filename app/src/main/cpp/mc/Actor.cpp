//
// Created by TheChuan1503 on 2025/5/1.
//

#include "Actor.h"

#ifndef GLOBAL_H
#define TAG "Native/Actor"
#include "../global.h"
#endif

void Actor::setPos(Vec3 &vec3) {
    if (isMinecraftVersion1_16()) {
        setPos_t func = (setPos_t) getMinecraftFunction("Actor::setPos");
        if (func && mc_isInGame) {
            func(this, &vec3);
        }
    } else {
        setPos_t func = reinterpret_cast<setPos_t>(getMCFuncAddr("Actor::setPos"));
        if (mc_isInGame) {
            func(this, &vec3);
            LOGD("setPos: %f, %f, %f", vec3.x, vec3.y, vec3.z);
        }
    }
}

void Actor::setPos(float x, float y, float z) {
    Vec3 vec3(x, y, z);
    setPos(vec3);
}
void Actor::teleportTo(Vec3 pos, bool shouldStopRiding, int cause, int sourceEntityType, bool keepVelocity) {
    teleportTo_t func = reinterpret_cast<teleportTo_t>(getMCFuncAddr("Actor::teleportTo"));
    if (mc_isInGame) {
        func(this, pos, shouldStopRiding, cause, sourceEntityType, keepVelocity);
        LOGD("[C] teleportTo: %f, %f, %f, SPR: %b, C: %x, SET: %x, KV: %b", pos.x, pos.y, pos.z, shouldStopRiding, cause, sourceEntityType, keepVelocity);
    }
}