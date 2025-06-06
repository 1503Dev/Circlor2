//
// Created by TheChuan1503 on 2025/5/1.
//

#ifndef CIRCLOR2_ACTOR_H
#define CIRCLOR2_ACTOR_H


#include "../structs.h"

class Actor {
public:
    typedef void (*setPos_t)(void* actor, const Vec3* vec3);
    typedef void (*teleportTo_t)(void* actor, Vec3 const& pos, bool shouldStopRiding, int cause, int sourceEntityType, bool keepVelocity);
    void setPos(Vec3 &vec3);
    void setPos(float x, float y, float z);
    void teleportTo(Vec3 pos, bool shouldStopRiding, int cause, int sourceEntityType, bool keepVelocity);
};


#endif //CIRCLOR2_ACTOR_H
