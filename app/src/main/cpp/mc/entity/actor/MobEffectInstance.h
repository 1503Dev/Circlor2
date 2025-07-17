//
// Created by TheChuan1503 on 2025/7/17.
//

#ifndef CIRCLORALPHA_MOBEFFECTINSTANCE_H
#define CIRCLORALPHA_MOBEFFECTINSTANCE_H


#include <cstdint>

class MobEffectInstance {
public:
    typedef void (*MobEffectInstance_t) (MobEffectInstance*, unsigned int, int, int, bool, bool, bool);

    MobEffectInstance (unsigned int effectId, int durationTicks, int level, bool b1, bool effectVisible, bool b2);

private:
    unsigned int mEffectId;
    int mDuration;
    int64_t mSomeIdOrTime;
    int mUnknownInt;
    int mAmplifier;
    bool mFlagA7;
    bool mFlagA5; // 可能是是否来自信标(?)
    bool mUnknownFlag;
    bool mFlagA6;
};


#endif //CIRCLORALPHA_MOBEFFECTINSTANCE_H
