//
// Created by TheChuan1503 on 2025/5/1.
//

#ifndef CIRCLOR2_ACTOR_H
#define CIRCLOR2_ACTOR_H


#include "../../math/Vec3.h"
#include "MobEffectInstance.h"
#include "../../math/AABB.h"

class Actor {
public:
    typedef void (*addEffect_t)(Actor* actor, const MobEffectInstance*);
    typedef bool (*canFly_t)(Actor* actor);
    typedef float (*distanceTo_t) (Actor* actor, Actor* target);
    typedef AABB* (*getAABBShapeComponent_t) (Actor*);
    typedef float (*getCameraDistance_t) (Actor*);
    typedef Vec3* (*getPos_t)(Actor* actor);
    typedef bool (*isAlive_t)(Actor* actor);
    typedef void (*setCanFly_t)(Actor* actor, bool canFly);
    typedef void (*setPos_t)(Actor* actor, const Vec3* vec3);
    typedef void (*teleportTo_t)(Actor* actor, Vec3 const& pos, bool shouldStopRiding, int cause, int sourceEntityType, bool keepVelocity);

    void addEffect (const MobEffectInstance*);
    float distanceTo(Actor* target);
    float getCameraDistance();
    Vec3 *getPos();
    bool isAlive();
    virtual bool isLocalPlayer() const {
        return false;
    };
    virtual bool isPlayer() const {
        return false;
    };
    void setPos(Vec3 &vec3);
    void setPos(float x, float y, float z);
    void setCanFly(bool canFly);
    void teleportTo(Vec3 pos, bool shouldStopRiding, int cause, int sourceEntityType, bool keepVelocity);
};


#endif //CIRCLOR2_ACTOR_H
