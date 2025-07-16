//
// Created by TheChuan1503 on 2025/7/16.
//

#include "HookManager.h"
#ifndef GLOBAL_H
#define TAG "Native/HookManager"
#include "../global.h"
#endif
#include "Circlor.h"

ClientInstance::update_t O_ClientInstance_update = nullptr;
ClientInstance::requestLeaveGame_t O_ClientInstance_requestLeaveGame = nullptr;

void H_ClientInstance_update(ClientInstance *ci, bool b) {
    clientInstance = ci;
    O_ClientInstance_update(ci, b);
}
void H_ClientInstance_requestLeaveGame(ClientInstance *ci, bool b1, bool b2) {
    LOGD("ClientInstance::requestLeaveGame(%d, %d)", b1, b2);
    O_ClientInstance_requestLeaveGame(ci, b1, b2);
}

void HookManager::init() {
    Circlor::hook("ClientInstance::requestLeaveGame", (void*)&H_ClientInstance_requestLeaveGame, (void**)&O_ClientInstance_requestLeaveGame);
    Circlor::hook("ClientInstance::update", (void*)&H_ClientInstance_update, (void**)&O_ClientInstance_update);
}
