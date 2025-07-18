//
// Created by TheChuan1503 on 2025/4/26.
//

#ifndef CIRCLOR_CLIENTINSTANCE_H
#define CIRCLOR_CLIENTINSTANCE_H

#include "../entity/actor/player/LocalPlayer.h"
#include "gui/GuiData.h"

class ClientInstance {
public:
    typedef GuiData* (*getGuiData_t)(ClientInstance* clientInstance);
    typedef LocalPlayer* (*getLocalPlayer_t)(ClientInstance* clientInstance);
    typedef bool (*isInGame_t)(ClientInstance* clientInstance);
    typedef void (*requestLeaveGame_t)(ClientInstance* clientInstance, bool p1, bool p2);
    typedef void (*tick_t)(ClientInstance*);
    typedef void (*update_t)(ClientInstance* clientInstance, bool p1);

    GuiData* getGuiData();
    LocalPlayer* getLocalPlayer();
    bool isInGame();
    void requestLeaveGame(bool, bool);
    void update(bool);
};


#endif //CIRCLOR_CLIENTINSTANCE_H
