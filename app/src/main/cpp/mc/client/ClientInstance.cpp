//
// Created by TheChuan1503 on 2025/4/26.
//

#include "ClientInstance.h"

#ifndef GLOBAL_H
#define TAG "Native/ClientInstance"
#include "../global.h"
#endif

void ClientInstance::requestLeaveGame(bool b1, bool b2) {
    requestLeaveGame_t requestLeaveGame = (requestLeaveGame_t) getMCFuncPtr("ClientInstance::requestLeaveGame");
    if (requestLeaveGame) {
        requestLeaveGame(this, b1, b2);
    }
}

bool ClientInstance::isInGame() {
    isInGame_t isInGame = (isInGame_t) getMCFuncPtr("ClientInstance::isInGame");
    if (isInGame) {
        return isInGame(this);
    }
    return false;
}

LocalPlayer *ClientInstance::getLocalPlayer() {
    getLocalPlayer_t getLocalPlayer = (getLocalPlayer_t) getMCFuncPtr("ClientInstance::getLocalPlayer");
    if (getLocalPlayer) {
        return getLocalPlayer(this);
    }
    return nullptr;
}
