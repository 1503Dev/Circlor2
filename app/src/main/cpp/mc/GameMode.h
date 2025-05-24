//
// Created by TheChuan1503 on 2025/5/1.
//

#ifndef CIRCLOR2_GAMEMODE_H
#define CIRCLOR2_GAMEMODE_H

#include "../structs.h"

class GameMode {
public:
    typedef void (*destroyBlock_t)(GameMode *gameMode, const BlockPos *, unsigned char);
    void destroyBlock(BlockPos pos, unsigned char flag);
};


#endif //CIRCLOR2_GAMEMODE_H
