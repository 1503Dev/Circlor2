//
// Created by TheChuan1503 on 2025/7/17.
//

#ifndef CIRCLORALPHA_LEVEL_H
#define CIRCLORALPHA_LEVEL_H

#include <vector>
#include "../entity/actor/Actor.h"

class Level {
public:
    typedef std::vector<::Actor*> (*getRuntimeActorList_t) (Level*);
    typedef void (*tick_t) (Level*);

    std::vector<::Actor*> getRuntimeActorList();
};


#endif //CIRCLORALPHA_LEVEL_H
