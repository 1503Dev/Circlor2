//
// Created by TheChuan1503 on 2025/7/17.
//

#ifndef CIRCLORALPHA_MOBEFFECTINSTANCE_H
#define CIRCLORALPHA_MOBEFFECTINSTANCE_H


class MobEffectInstance {
public:
    typedef void (*MobEffectInstance_t) (MobEffectInstance*, unsigned int, int, int, bool, bool, bool);

    MobEffectInstance (unsigned int, int, int, bool, bool, bool);
};


#endif //CIRCLORALPHA_MOBEFFECTINSTANCE_H
