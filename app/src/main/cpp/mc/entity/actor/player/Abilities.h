//
// Created by TheChuan1503 on 2025/7/17.
//

#ifndef CIRCLORALPHA_ABILITIES_H
#define CIRCLORALPHA_ABILITIES_H


#include "AbilitiesIndex.h"

class Abilities {
public:
    typedef void (*Abilities_t) (Abilities*);
    typedef bool (*getBool_t) (Abilities*, AbilitiesIndex);
};


#endif //CIRCLORALPHA_ABILITIES_H
