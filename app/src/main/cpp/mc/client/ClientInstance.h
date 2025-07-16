//
// Created by TheChuan1503 on 2025/4/26.
//

#ifndef CIRCLOR_CLIENTINSTANCE_H
#define CIRCLOR_CLIENTINSTANCE_H

#include "../entity/actor/player/LocalPlayer.h"

class ClientInstance {
public:
    typedef LocalPlayer* (*getLocalPlayer_t)(void* clientInstance);
    typedef bool (*isInGame_t)(void* clientInstance);
    typedef void (*requestLeaveGame_t)(void* clientInstance, bool p1, bool p2);
    typedef void (*update_t)(void* clientInstance, bool p1);

    LocalPlayer* getLocalPlayer();
    bool isInGame();
    void requestLeaveGame(bool, bool);
    void update(bool);
};


#endif //CIRCLOR_CLIENTINSTANCE_H
