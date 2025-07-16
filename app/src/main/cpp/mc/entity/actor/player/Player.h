//
// Created by TheChuan1503 on 2025/5/2.
//

#ifndef CIRCLOR2_PLAYER_H
#define CIRCLOR2_PLAYER_H

#include "../../../math/Vec3.h"
#include "../Actor.h"

class Player: public Actor {
public:
    typedef void (*teleportTo_t)(Player *player, Vec3&, bool, int, int, bool);
    void teleportTo(Vec3 &, bool, int, int, bool);
};

#endif //CIRCLOR2_PLAYER_H
