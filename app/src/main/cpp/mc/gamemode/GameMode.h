//
// Created by TheChuan1503 on 2025/5/1.
//

#ifndef CIRCLOR2_GAMEMODE_H
#define CIRCLOR2_GAMEMODE_H

#include "../level/BlockPos.h"

class GameMode {
public:
    typedef void (*destroyBlock_t)(GameMode*, const BlockPos *, unsigned char);
    typedef float (*getMaxPickRange_t)(GameMode*);

    void destroyBlock(BlockPos pos, unsigned char flag);
    float getMaxPickRange();
};

#endif //CIRCLOR2_GAMEMODE_H
