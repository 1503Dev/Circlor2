//
// Created by TheChuan1503 on 2025/7/17.
//

#include "Level.h"

#define TAG "Native/Level"
#include "../../global.h"

std::vector<::Actor *> Level::getRuntimeActorList() {
    getRuntimeActorList_t getRuntimeActorList = (getRuntimeActorList_t) getMCFuncPtr("Level::getRuntimeActorList");
    return getRuntimeActorList(this);
}
