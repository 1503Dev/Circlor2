//
// Created by TheChuan1503 on 2025/7/16.
//

#include "HookManager.h"
#ifndef GLOBAL_H
#define TAG "Native/HookManager"
#include "../global.h"
#endif
#include "Circlor.h"
#include "Gloss.h"
#include "mc/client/Minecraft.h"
#include "mc/gamemode/GameMode.h"

ClientInstance::requestLeaveGame_t O_ClientInstance_requestLeaveGame = nullptr;
ClientInstance::update_t O_ClientInstance_update = nullptr;
GameMode::destroyBlock_t O_GameMode_destroyBlock = nullptr;
Minecraft::update_t O_Minecraft_update = nullptr;

void H_ClientInstance_update(ClientInstance *ci, bool b) {
    clientInstance = ci;
    O_ClientInstance_update(ci, b);
}
void H_ClientInstance_requestLeaveGame(ClientInstance *ci, bool b1, bool b2) {
    LOGD("ClientInstance::requestLeaveGame(%d, %d)", b1, b2);
    O_ClientInstance_requestLeaveGame(ci, b1, b2);
}
void H_GameMode_destroyBlock(GameMode *gm, const BlockPos *pos, unsigned char flag) {
    LOGD("GameMode::destroyBlock(BlockPos<%d, %d, %d>, %d)", pos->x, pos->y, pos->z, flag);
    if (Circlor::getFunctionBoolValue("destroy_block_disabled")) {
        return;
    }
    O_GameMode_destroyBlock(gm, pos, flag);
}
void H_Minecraft_update(Minecraft *mc) {
    minecraft = mc;
    O_Minecraft_update(mc);
}

void HookManager::init() {
    hook("ClientInstance::requestLeaveGame", (void*)&H_ClientInstance_requestLeaveGame, (void**)&O_ClientInstance_requestLeaveGame);
    hook("ClientInstance::update", (void*)&H_ClientInstance_update, (void**)&O_ClientInstance_update);
    hook("GameMode::destroyBlock", (void*)&H_GameMode_destroyBlock, (void**)&O_GameMode_destroyBlock);
    hook("Minecraft::update", (void*)&H_Minecraft_update, (void**)&O_Minecraft_update);
}

void HookManager::hook(const char *name, void *hook) {
    HookManager::hook(getOffset(name), hook);
}
void HookManager::hook(const char *name, void *hook, void **original) {
    HookManager::hook(getOffset(name), hook, original);
}
void HookManager::hook(long target, void *hook) {
    HookManager::hook(target, hook, nullptr);
}
void HookManager::hook(long target, void *hook, void **original) {
    void *targetAddr = (void *)((uintptr_t)getMinecraftBase() + target);
    GlossHook(targetAddr, hook, original);
    LOGD("Hooked %p", targetAddr);
}