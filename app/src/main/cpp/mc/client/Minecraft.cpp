//
// Created by TheChuan1503 on 2025/7/16.
//

#include "Minecraft.h"

#ifndef GLOBAL_H
#define TAG "Native/Minecraft"
#include "../global.h"
#endif

void Minecraft::setGameModeReal(GameType gt) {
    setGameModeReal_t setGameModeReal = (setGameModeReal_t) getMCFuncPtr("Minecraft::setGameModeReal");
    if (setGameModeReal) {
        setGameModeReal(this, gt);
    }
}
