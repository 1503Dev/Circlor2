//
// Created by TheChuan1503 on 2025/5/1.
//

#include "GameMode.h"

#ifndef GLOBAL_H
#define TAG "Native/GameMode"
#include "../global.h"
#endif

void GameMode::destroyBlock(BlockPos pos, unsigned char flag) {
    destroyBlock_t func = (destroyBlock_t) getMCFuncPtr("GameMode::destroyBlock");
    if (func && isInGame()) {
        func(this, &pos, flag);
    }
}
