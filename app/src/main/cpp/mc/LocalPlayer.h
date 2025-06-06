//
// Created by TheChuan1503 on 2025/4/27.
//

#ifndef CIRCLOR2_LOCALPLAYER_H
#define CIRCLOR2_LOCALPLAYER_H


#include "../structs.h"
#include "Actor.h"

class LocalPlayer: public Actor{
public:
    typedef void (*addLevels_t)(void* localPlayer, int levels);
    typedef float (*getPickRange_t)(void* localPlayer);
    void addLevels(int levels);
    float getPickRange();
};


#endif //CIRCLOR2_LOCALPLAYER_H
