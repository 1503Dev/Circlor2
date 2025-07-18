//
// Created by TheChuan1503 on 2025/5/1.
//

#ifndef CIRCLOR2_MINECRAFTGAME_H
#define CIRCLOR2_MINECRAFTGAME_H


class MinecraftGame {
public:
    typedef void* (*getInput_t) (MinecraftGame*);
    typedef void (*tickInput_t) (MinecraftGame*);

    void* getInput();
};


#endif //CIRCLOR2_MINECRAFTGAME_H
