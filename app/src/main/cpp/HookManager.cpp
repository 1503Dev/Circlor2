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
#include "mc/level/Level.h"
#include "mc/entity/actor/player/PlayerSnapshot.h"
#include "mc/entity/actor/player/Abilities.h"
#include "mc/entity/actor/MobEffectInstance.h"


bool nuking = false;



Abilities::getBool_t O_Abilities_getBool = nullptr;
ClientInstance::requestLeaveGame_t O_ClientInstance_requestLeaveGame = nullptr;
ClientInstance::update_t O_ClientInstance_update = nullptr;
GameMode::destroyBlock_t O_GameMode_destroyBlock = nullptr;
GameMode::getMaxPickRange_t O_GameMode_getMaxPickRange = nullptr;
Level::tick_t O_Level_tick = nullptr;
LocalPlayer::getPickRange_t O_LocalPlayer_getPickRange = nullptr;
MobEffectInstance::MobEffectInstance_t O_MobEffectInstance_MobEffectInstance = nullptr;



bool H_Abilities_getBool(Abilities* a, AbilitiesIndex ai) {
    bool rez = O_Abilities_getBool(a, ai);
    if (ai == AbilitiesIndex::MayFly) {
        if (clientInstance->getLocalPlayer() && clientInstance->getLocalPlayer()->getPlayerGameType() == GameType::Creative) {
            rez = true;
        } else {
            rez = Circlor::getBool("fly/enabled");
        }
    }
    if (ai == AbilitiesIndex::Flying) {
        if (Circlor::getBool("fly/enabled") && Circlor::getBool("fly/force/enabled")){
            rez = true;
        }
    }
    if (ai == AbilitiesIndex::NoClip) {
        rez = Circlor::getBool("no_clip/enabled");
    }
    return rez;
}
void H_ClientInstance_update(ClientInstance *ci, bool b) {
    clientInstance = ci;
    O_ClientInstance_update(ci, b);
}
void H_ClientInstance_requestLeaveGame(ClientInstance *ci, bool b1, bool b2) {
    LOGD("ClientInstance::requestLeaveGame(%d, %d)", b1, b2);
    O_ClientInstance_requestLeaveGame(ci, b1, b2);
}
void H_GameMode_destroyBlock(GameMode *gm, const BlockPos *pos, unsigned char flag) {
    if (Circlor::getFunctionBoolValue("destroy_block/disabled")) {
        return;
    }
    O_GameMode_destroyBlock(gm, pos, flag);
    if (Circlor::getFunctionBoolValue("nuker/enabled")) {
        int radius = (int) Circlor::getFunctionValue("nuker/radius");
        if (radius > 0) {
            for (int x = -radius; x <= radius; x++) {
                for (int y = -radius; y <= radius; y++) {
                    for (int z = -radius; z <= radius; z++) {
                        BlockPos pos2(pos->x + x, pos->y + y, pos->z + z);
                        O_GameMode_destroyBlock(gm, &pos2, flag);
                    }
                }
            }
        }
    }
}
float H_GameMode_getMaxPickRange(GameMode *gm) {
    if (Circlor::getBool("reach/enabled")) {
        return (float) Circlor::getValue("reach/range");
    }
    return O_GameMode_getMaxPickRange(gm);
}
void H_Level_tick(Level* l) {
    Circlor::onTick();
    O_Level_tick(l);
}
float H_LocalPlayer_getPickRange(LocalPlayer *lp) {
    if (Circlor::getBool("reach/enabled")) {
        return (float) Circlor::getValue("reach/range");
    }
    return O_LocalPlayer_getPickRange(lp);
}
void H_MobEffectInstance_MobEffectInstance(MobEffectInstance* mei, unsigned int p1, int p2, int p3, bool p4, bool p5, bool p6) {
    LOGD("MobEffectInstance::MobEffectInstance(%d, %d, %d, %d, %d, %d)", p1, p2, p3, p4, p5, p6);
    O_MobEffectInstance_MobEffectInstance(mei, p1, p2, p3, p4, p5, p6);
}



void HookManager::init() {
    hook("Abilities::getBool", (void*)&H_Abilities_getBool, (void**)&O_Abilities_getBool);
    hook("ClientInstance::requestLeaveGame", (void*)&H_ClientInstance_requestLeaveGame, (void**)&O_ClientInstance_requestLeaveGame);
    hook("ClientInstance::update", (void*)&H_ClientInstance_update, (void**)&O_ClientInstance_update);
    hook("GameMode::destroyBlock", (void*)&H_GameMode_destroyBlock, (void**)&O_GameMode_destroyBlock);
    hook("GameMode::getMaxPickRange", (void*)&H_GameMode_getMaxPickRange, (void**)&O_GameMode_getMaxPickRange);
    hook("MobEffectInstance::MobEffectInstance", (void*)&H_MobEffectInstance_MobEffectInstance, (void**)&O_MobEffectInstance_MobEffectInstance);
    hook("Level::tick", (void*)&H_Level_tick, (void**)&O_Level_tick);
    hook("LocalPlayer::getPickRange", (void*)&H_LocalPlayer_getPickRange, (void**)&O_LocalPlayer_getPickRange);
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